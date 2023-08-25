#include "clock.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

namespace {
class Counter : public TimedObject {
 public:
  Counter(int& counter) : counter_(counter) {}

  void Tick(int) { ++counter_; }

 private:
  int& counter_;
};

class Timer : public TimedObject {
 public:
  Timer(int& time_passed) : time_passed_(time_passed) {}

  void Tick(int time_passed) { time_passed_ = time_passed; }

 private:
  int& time_passed_;
};
}  // namespace

TEST(ClockTest, EmptyClockCanTick) { Tick(); }

TEST(ClockTest, TickRegisterAndUnregister) {
  int ticks = 0;
  std::unique_ptr<Counter> counter = std::make_unique<Counter>(ticks);
  EXPECT_EQ(ticks, 0);
  Tick();
  EXPECT_EQ(ticks, 1);
  Tick();
  EXPECT_EQ(ticks, 2);
  counter.reset();
  Tick();
  EXPECT_EQ(ticks, 2);
}

TEST(ClockTest, AllTicksAreCalled) {
  int ticks = 0;
  std::unique_ptr<Counter> counter = std::make_unique<Counter>(ticks);
  std::unique_ptr<Counter> second_counter = std::make_unique<Counter>(ticks);
  EXPECT_EQ(ticks, 0);
  Tick();
  EXPECT_EQ(ticks, 2);
  Tick();
  EXPECT_EQ(ticks, 4);
  counter.reset();
  Tick();
  EXPECT_EQ(ticks, 5);
}

TEST(ClockTest, TimePassedIsHowOftenTicksWasCalled) {
  int time_passed;
  std::unique_ptr<Timer> counter = std::make_unique<Timer>(time_passed);
  Tick();
  Tick();
  EXPECT_EQ(time_passed, 2);
  counter = std::make_unique<Timer>(time_passed);
  Tick();
  EXPECT_EQ(time_passed, 1);
}
