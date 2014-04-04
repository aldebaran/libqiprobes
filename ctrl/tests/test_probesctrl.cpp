#include <gtest/gtest.h>
#include <qiprobes/ctrl.hpp>
#include <qi/log.hpp>
#include <qi/application.hpp>

class Increment
{
public:
  int &n;
  Increment(int &n) : n(n) {};
  void operator()() {++n;}
};

TEST(ProbesCtrl, invalid_trigger_catgory)
{
  int n = 0;
  qi::ProbesLogTrigger trigger = qi::ProbesLogTrigger(Increment(n));
  // wildchar
  ASSERT_THROW(trigger.setTrigger("d*d", qi::LogLevel_Error, ".* a0"),
               std::runtime_error);
  // infinite recursion safeguard
  ASSERT_THROW(trigger.setTrigger("qiprobes.ctrl", qi::LogLevel_Error, ".* a0"),
               std::runtime_error);
}

TEST(ProbesCtrl, trigger)
{
  int n = 0;
  qi::ProbesLogTrigger trigger = qi::ProbesLogTrigger(Increment(n));
  trigger.setTrigger("aaa", qi::LogLevel_Error, ".* a0");
  trigger.setTrigger("bbb", qi::LogLevel_Error, ".* a0");
  trigger.setTrigger("ccc", qi::LogLevel_Error, ".* c0");

  trigger.start();
  ASSERT_EQ(0, n);

  qiLogError("aaa") << "error a0"; // match!
  ASSERT_EQ(1, n);
  qiLogFatal("aaa") << "error a0"; // match!
  ASSERT_EQ(2, n);

  qiLogError("aaa") << "error a1"; // wrong msg
  qiLogWarning("aaa") << "warning a0"; // too verbose
  ASSERT_EQ(2, n);

  qiLogError("bbb") << "error a0"; // match!
  ASSERT_EQ(3, n);

  qiLogError("ccc") << "error a0"; // wrong (cat, msg) pair
  ASSERT_EQ(3, n);

  // ignored, running
  ASSERT_THROW(trigger.setTrigger("aaa", qi::LogLevel_Error, ".* a0"), std::runtime_error);
  ASSERT_THROW(trigger.removeTrigger("aaa"), std::runtime_error);

  trigger.stop();

  qiLogError("aaa") << "error a0"; // no match, stopped
  qiLogError("bbb") << "error a0"; // no match, stopped
  ASSERT_EQ(3, n);

  trigger.removeTrigger("aaa");
  trigger.setTrigger("ccc", qi::LogLevel_Error, ".* a0");

  trigger.start();

  qiLogError("aaa") << "error a0"; // no match, match removed
  ASSERT_EQ(3, n);
  qiLogError("bbb") << "error a0"; // match!
  ASSERT_EQ(4, n);
  qiLogError("ccc") << "error a0"; // match!
  ASSERT_EQ(5, n);

  trigger.stop();
}

int main(int argc, char* argv[])
{
  qi::Application app(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
