/**
 * @author Sebastien Barthelemy
 * Aldebaran Robotics (c) 2014 All Rights Reserved.
 *
 */

#pragma once
#ifndef QIPROBES_CTRL
#define QIPROBES_CTRL
#include <qi/log.hpp>

namespace qi {
namespace probes {

  // init the control library. Can be called at anytime.
  void init(bool traceWhileRecording,
            int gracePeriodInMilliSeconds);
  // return true if it could *request* the snapshot recording (ie, if another
  // recording was not already in progress).
  bool recordSnapshot();
  // destroy the control library
  void destroy();
}

class ProbesLogTriggerPrivate;

// will trig (ie. call onTrigFct) if a log with
//
//   category == category and
//   verbosity <= level and
//   msg which matches msgRegex
//
// is seen. Logs are seen once start() as been called.
// Triggers can only be changed (add, remove) before calling start (or after
// calling stop)
class ProbesLogTrigger {
public:
  ProbesLogTrigger(boost::function0<void> onTrigFct);
  ~ProbesLogTrigger();
  // set a trigger condition.
  //
  // Note that:
  //  * category is a raw category, globs are not accepted
  //  * we handle only one trigger per category (One can always merge the
  //    regexps if needed).
  void setTrigger(const std::string &category,
                  qi::LogLevel level,
                  const std::string &msgRegex);
  void setTrigger(const std::string &category,
                  const std::string &level,
                  const std::string &msgRegex);
  void removeTrigger(const std::string &category);
  // start watching logs (register as a log handler)
  void start();
  // stop watching logs (unregister as a log handler)
  void stop();
private:
  ProbesLogTriggerPrivate *_p;
};
}
#endif
