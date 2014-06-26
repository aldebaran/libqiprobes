/**
 * @author Sebastien Barthelemy
 * Aldebaran Robotics (c) 2014 All Rights Reserved.
 *
 */

#ifdef WITH_PROBES
// should be included first because of INT32_{MIN,MAX} compatibility macros
#include <lttng/lttng.h>
#include <lttng/snapshot.h>
#endif

#include <qiprobes/ctrl.hpp>
#include <boost/thread.hpp>
#include <boost/regex.hpp>

#include <boost/chrono.hpp>
#include <qi/os.hpp>

namespace qi {

static const char myLogCategory[] = "qiprobes.ctrl";
qiLogCategory(myLogCategory);

class ProbesCtrl
{
private:
  enum State {IDLE =0 , RECORD, STOP };
public:
  bool traceWhileRecording;
  boost::chrono::milliseconds gracePeriod;

  ProbesCtrl() :
    _state(IDLE),
    traceWhileRecording(false),
    gracePeriod(0),
    _thread(boost::bind(&ProbesCtrl::_run, this)) {}

  ~ProbesCtrl()
  {
    {
      boost::mutex::scoped_lock(_mutex);
      _state = STOP;
      _condition.notify_all();
    }
    _thread.join();
  }

  bool recordSnapshot()
  {
    // lock-less try-call
    boost::unique_lock<boost::mutex> lock(_mutex, boost::try_to_lock);
    if (lock.owns_lock() && _state == IDLE)
    {
      _state = RECORD;
      _condition.notify_all();
      return true;
    }
    return false;
  }

private:
  static void _recordSnapshot(bool traceWhileRecording)
  {
#ifdef WITH_PROBES
    qiLogVerbose() << "start _recordSnapshot("
                   << (traceWhileRecording ? "true" : "false") <<")";
    int ret = 0;
    lttng_snapshot_output_list *outputs = 0;
    lttng_snapshot_output *output = 0;
    lttng_session *sessions = 0;
    int nbSessions = lttng_list_sessions(&sessions);
    if (nbSessions == 0)
    {
      qiLogError() << "Cannot record snapshot: no LTTng session found";
      goto free_sessions;
    }
    if (nbSessions > 1)
    {
      qiLogWarning()
          << "Several (" << nbSessions << ") LTTng sessions found."
          << " Using the one named \"" << sessions[0].name << "\"";
    }
    if (!traceWhileRecording)
    {
      ret = lttng_stop_tracing_no_wait(sessions[0].name);
      if (ret != 0)
      {
        qiLogError()
            << "Cannot record snapshot: lttng_stop_tracing_no_wait: " << ret;
        goto free_sessions;
      }
    }

    ret = lttng_snapshot_list_output(sessions[0].name, &outputs);
    if (ret != 0)
    {
      qiLogError()
          << "Cannot record snapshot: lttng_snapshot_list_output: " << ret;
      goto start_tracing;
    }

    output = lttng_snapshot_output_list_get_next(outputs);
    if (output == 0)
    {
      qiLogError() << "Cannot record snapshot:"
                   << " lttng_snapshot_output_list_get_next empty";
      goto free_outputs;
    }

    // blocking call
    ret = lttng_snapshot_record(sessions[0].name, output, 0 );
    if (ret != 0)
    {
      qiLogError() << "Cannot record snapshot: lttng_snapshot_record: " << ret;
      //goto free_outputs;
    }
free_outputs:
  lttng_snapshot_output_list_destroy(outputs);
start_tracing:
    if (!traceWhileRecording)
    {
      // restart tracing if we stopped it
      ret = lttng_start_tracing(sessions[0].name);
      if (ret != 0)
      {
        qiLogError()
            << "Cannot restart tracing: lttng_start_tracing: " << ret;
      }
    }
free_sessions:
    free(sessions);
    qiLogVerbose() << "_recordSnapshot done";
#else
    qiLogError() << "Cannot record snapshot: LTTng disabled at compilation";
#endif
  }

  void _run()
  {
    qi::os::setCurrentThreadName("qiprobesCtrl");
    {
      boost::unique_lock<boost::mutex> lock(_mutex);
      while (_state != STOP)
      {
        _condition.wait(lock);
        switch (_state)
        {
        case IDLE:
          break;
        case RECORD:
          _recordSnapshot(traceWhileRecording);
          // sleep so that
          // * system has some time to recover from the dump overhead,
          // * and the ringbuffer fills up again (given that we stop recording
          //   while we dump, dumping several time in a row is pointless).
          //
          // this use of _grace (two subsequent reads) is a bit racy,
          // but is harmless
          if (gracePeriod > boost::chrono::milliseconds::zero())
            boost::this_thread::sleep_for(gracePeriod);
          _state = IDLE;
          break;
        case STOP:
          break;
        }
      }
    }
  }

private:
  State _state;
  boost::thread _thread;
  boost::mutex _mutex;
  boost::condition_variable _condition;
};

static boost::mutex _glProbesCtrlMutex;
static boost::scoped_ptr<ProbesCtrl> _glProbesCtrlInstance;
static ProbesCtrl *_getProbesCtrl()
{
  boost::unique_lock<boost::mutex> lock(_glProbesCtrlMutex);
  if (!_glProbesCtrlInstance)
    _glProbesCtrlInstance.reset(new ProbesCtrl());
  return _glProbesCtrlInstance.get();
}

namespace probes
{
  void init(bool traceWhileRecording,
            int gracePeriodInMilliSeconds)
  {
    ProbesCtrl *ctrl = _getProbesCtrl();
    ctrl->traceWhileRecording = traceWhileRecording;
    ctrl->gracePeriod = boost::chrono::milliseconds(gracePeriodInMilliSeconds);
  }

  bool recordSnapshot()
  {
    return _getProbesCtrl()->recordSnapshot();
  }

  void destroy()
  {
    boost::unique_lock<boost::mutex> lock(_glProbesCtrlMutex);
    _glProbesCtrlInstance.reset();
  }
}

// qi::Log limits the category to 64 chars, but does not expose this value.
// If it grows, we may falsely match categories which only differ in the
// >=64 chars of the category.
static const size_t CATEGORY_SIZE = 64;

// a match condition is a pair of plain category and message regex.
// we don't match the log level but use it to set the qi::Log filters,
// so we won't see (nor match) logs which are more verbose than "level".
class Cond
{
public:
  std::string cat;
  qi::LogLevel level;
  boost::regex msgRegex;
  Cond(const std::string &cat, const qi::LogLevel level, const::std::string &msgRegex)
    : cat(cat), level(level), msgRegex(msgRegex) {
  }

  bool operator==(const char *category) const
  {
    return cat == category;
  }
  bool operator==(const std::string &category) const
  {
    return cat == category;
  }
};

class ProbesLogTriggerPrivate
{
  boost::function0<void> _fct;
  // we want to avoid mutexes on the log() method, which will be called by
  // qi::log. So instead we ensure that the data can only be changed when we
  // are not registered has a log handler (hence the log() method won't be
  // called).
  mutable boost::mutex _isSubscribedMutex;
  bool _isSubscribed;
  qi::log::SubscriberId _id;
  static const std::string _name;
  // we'll have few conditions, better avoid fancy containers
  typedef std::vector<Cond> CondVect;
  CondVect _conds;
public:
  ProbesLogTriggerPrivate(boost::function0<void> onTrigFct) :
    _fct(onTrigFct),
    _isSubscribed(false),
    _id(0) {}

  void setTrigger(const std::string &category,
                  qi::LogLevel level,
                  const std::string &msgRegex);
  void removeTrigger(const std::string &category);

  void start();
  void stop();

  void log(const qi::LogLevel verb,
           const qi::os::timeval,
           const char* category,
           const char* msg,
           const char* file,
           const char* fct,
           int line);
};

const std::string ProbesLogTriggerPrivate::_name = "qiProbesCtrl";

void ProbesLogTriggerPrivate::setTrigger(const std::string &category,
                                         qi::LogLevel level,
                                         const std::string &msgRegex)
{
  // we do not support globbing, check category has no wild char
  // also refuse our own logs to avoid recursion
  if (category.find('*') != std::string::npos ||
      category.length() > CATEGORY_SIZE-1 ||
      category == myLogCategory)
  {
    throw std::runtime_error("invalid category");
  }
  boost::unique_lock<boost::mutex> lock(_isSubscribedMutex);
  if (_isSubscribed)
    throw std::runtime_error("cannot change triggers while running");
  qiLogVerbose() << "setTrigger("
               << category << ", "
               << log::logLevelToString(level, true)
               << ", " << msgRegex << ")";
  CondVect::iterator it = std::find(_conds.begin(), _conds.end(), category);
  if (it == _conds.end())
    _conds.push_back(Cond(category, level, msgRegex));
  else
    *it = Cond(category, level, msgRegex);
}

void ProbesLogTriggerPrivate::removeTrigger(const std::string &category)
{
  boost::unique_lock<boost::mutex> lock(_isSubscribedMutex);
  if (_isSubscribed)
    throw std::runtime_error("cannot change triggers while running");
  CondVect::iterator it = std::find(_conds.begin(), _conds.end(), category);
  if (it != _conds.end())
    _conds.erase(it);
}

void ProbesLogTriggerPrivate::start()
{
  boost::unique_lock<boost::mutex> lock(_isSubscribedMutex);
  if (_conds.empty())
    return;
  _isSubscribed = true;
  // by default, subscribe to no log (thanks to LogLevel_Silent)
  _id = qi::log::addLogHandler(_name,
      boost::bind(&ProbesLogTriggerPrivate::log, this,
                  _1, _2, _3, _4, _5, _6, _7),
      qi::LogLevel_Silent);
  // then subscribe to specific categories
  for (CondVect::const_iterator it=_conds.begin(); it!=_conds.end();++it)
  {
    qi::log::addFilter(std::string(it->cat), it->level, _id);
  }
}

void ProbesLogTriggerPrivate::log(const qi::LogLevel /*verb*/,
                                  const qi::os::timeval /*time*/,
                                  const char *category,
                                  const char *msg,
                                  const char */*file*/,
                                  const char */*fct*/,
                                  int /*line*/)
{
  // for each log we see, we compare the char[64] category name. We could
  // improve performance a bit if we
  // 1. keep the vector sorted
  // 2. compare cateogory name pointers instead of the string (be cause we know
  //    that qi::log will always use the same address for a given name
  CondVect::const_iterator it = std::find(_conds.begin(), _conds.end(), category);
  if (it != _conds.end() && boost::regex_match(msg, it->msgRegex))
  {
    _fct();
    return;
  }
}

void ProbesLogTriggerPrivate::stop()
{
  boost::unique_lock<boost::mutex> lock(_isSubscribedMutex);
  qi::log::removeLogHandler(_name);
  _id = 0;
  _isSubscribed = false;
}

ProbesLogTrigger::ProbesLogTrigger(boost::function0<void> onTrigFct)
  : _p(new ProbesLogTriggerPrivate(onTrigFct)) {}
ProbesLogTrigger::~ProbesLogTrigger() {delete _p;}

void ProbesLogTrigger::setTrigger(const std::string &category,
                                  qi::LogLevel level,
                                  const std::string &msgRegex)
{
  _p->setTrigger(category, level, msgRegex);
}

void ProbesLogTrigger::setTrigger(const std::string &category,
                                  const std::string &level,
                                  const std::string &msgRegex)
{
  _p->setTrigger(category, log::stringToLogLevel(level.c_str()), msgRegex);
}

void ProbesLogTrigger::removeTrigger(const std::string &category)
{
  _p->removeTrigger(category);
}

void ProbesLogTrigger::start()
{
  _p->start();
}

void ProbesLogTrigger::stop()
{
  _p->stop();
}
}
