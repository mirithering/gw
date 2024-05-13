#include "effect.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

class FakeEffect : public Effect {
 public:
  FakeEffect(Time duration) : Effect(duration) {}

  void Start() override { ++start_; }
  void Tick() override { ++tick_; }
  void End() override { ++end_; }

  int start_calls() { return start_; }
  int tick_calls() { return tick_; }
  int end_calls() { return end_; }

 private:
  int start_ = 0;
  int tick_ = 0;
  int end_ = 0;
};

TEST(EffectTest, StartIsCalled) {
  auto effect = FakeEffect(Time(1));
  Tick();
  EXPECT_EQ(effect.start_calls(), 1);
}

TEST(EffectTest, EndIsCalledAtEnd) {
  auto effect = FakeEffect(Time(1));
  Tick();
  EXPECT_EQ(effect.end_calls(), 1);
}

TEST(EffectTest, TickIsCalledDurationTimes) {
  auto effect = FakeEffect(Time(3));
  Tick();
  EXPECT_EQ(effect.tick_calls(), 1);
  Tick();
  EXPECT_EQ(effect.tick_calls(), 2);
  Tick();
  EXPECT_EQ(effect.tick_calls(), 3);
  Tick();
  EXPECT_EQ(effect.tick_calls(), 3);
}
