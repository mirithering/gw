#include "clock.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

namespace {
class Counter : public TimedObject {
 public:
  Counter(int& counter) : counter_(counter) {}

  void Tick(Time) override { ++counter_; }

 private:
  int& counter_;
};

class Timer : public TimedObject {
 public:
  Timer(Time& time_passed) : time_passed_(time_passed) {}

  void Tick(Time time_passed) override { time_passed_ = time_passed; }

 private:
  Time& time_passed_;
};
}  // namespace

TEST(ClockTest, EmptyClockCanTick) { Tick(); }

TEST(ClockTest, TickRegisterAndUnregister) {
  int ticks = 0;
  std::unique_ptr<Counter> counter = std::make_unique<Counter>(ticks);
  ASSERT_EQ(ticks, 0);
  Tick();
  ASSERT_EQ(ticks, 1);
  Tick();
  ASSERT_EQ(ticks, 2);
  counter.reset();
  Tick();
  ASSERT_EQ(ticks, 2);
}

TEST(ClockTest, AllTicksAreCalled) {
  int ticks = 0;
  std::unique_ptr<Counter> counter = std::make_unique<Counter>(ticks);
  std::unique_ptr<Counter> second_counter = std::make_unique<Counter>(ticks);
  ASSERT_EQ(ticks, 0);
  Tick();
  ASSERT_EQ(ticks, 2);
  Tick();
  ASSERT_EQ(ticks, 4);
  counter.reset();
  Tick();
  ASSERT_EQ(ticks, 5);
}

TEST(ClockTest, TimePassedIsHowOftenTicksWasCalled) {
  Time time_passed;
  std::unique_ptr<Timer> counter = std::make_unique<Timer>(time_passed);
  Tick();
  Tick();
  ASSERT_EQ(time_passed.milliseconds(), 2);
  counter = std::make_unique<Timer>(time_passed);
  Tick();
  ASSERT_EQ(time_passed.milliseconds(), 1);
}

TEST(ClockTest, MovedObjectKeepsTicking) {
  Time time;
  Timer* counter = new Timer(time);
  std::vector<Timer> counters;
  counters.push_back(std::move(*counter));
  delete counter;
  Tick();
  ASSERT_EQ(time.milliseconds(), 1);
}

namespace {
class Remover : public TimedObject {
 public:
  Remover(bool& deleted) : deleted_(deleted) {}
  ~Remover() override { deleted_ = true; }
  void Tick(Time) override { delete delete_on_tick_; }
  void SetRemove(TimedObject* delete_on_tick) {
    delete_on_tick_ = delete_on_tick;
  }

 private:
  TimedObject* delete_on_tick_ = nullptr;
  bool& deleted_;
};
}  // namespace

TEST(ClockTest, TickRemovesItself) {
  bool remover_deleted;
  Remover* remover = new Remover(remover_deleted);
  remover->SetRemove(remover);
  Tick();
  ASSERT_TRUE(remover_deleted);
  Tick();
}

// Ticks are called in construction order, so the one who is first constructed
// will delete the other one.
TEST(ClockTest, TickRemovesOtherTick) {
  bool remover1_deleted, remover2_deleted;
  Remover *remover1, *remover2;
  remover1 = new Remover(remover1_deleted);
  remover2 = new Remover(remover2_deleted);
  remover1->SetRemove(remover2);
  remover2->SetRemove(remover1);
  Tick();
  ASSERT_TRUE(remover2_deleted);
  ASSERT_FALSE(remover1_deleted);
  delete remover1;
}
