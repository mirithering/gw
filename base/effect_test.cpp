#include "effect.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "clock.h"

namespace {
const Time kTime = 10 * Second;
constexpr int kNumber = 5;
}  // namespace

TEST(EffectTest, NoEffectIsAlwaysOver) {
  Effect<int> effect = Effect<int>::None();
  ASSERT_TRUE(effect.Ended());
  ASSERT_EQ(effect.get(), nullptr);
  Tick();
}

TEST(EffectTest, EffectLastsForCorrectTime) {
  Effect<int> effect(kTime, std::make_unique<int>(kNumber));

  for (int i = 0; i < kTime.value(); ++i) {
    ASSERT_FALSE(effect.Ended());
    ASSERT_EQ(*effect.get(), kNumber);
    Tick();
  }
  ASSERT_TRUE(effect.Ended());
  ASSERT_EQ(effect.get(), nullptr);
}

TEST(EffectTest, RemovedEffectIsGone) {
  Effect<int> effect(kTime, std::make_unique<int>(kNumber));

  for (int i = 0; i < kTime.value() / 2; ++i) {
    ASSERT_FALSE(effect.Ended());
    ASSERT_EQ(*effect.get(), kNumber);
    Tick();
  }

  effect = Effect<int>::None();

  Tick();
  Tick();
  ASSERT_TRUE(effect.Ended());
  ASSERT_EQ(effect.get(), nullptr);
}

TEST(EffectTest, EffectWithTimedObject) {
  int ticks = 0;
  class Timer : public TimedObject {
   public:
    explicit Timer(int& ticks) : ticks_(ticks) {}
    void Tick(Time) override { ++ticks_; }
    int& ticks_;
  };

  Effect<Timer> effect(kTime, std::make_unique<Timer>(ticks));
  for (int i = 1; i <= kTime.value(); ++i) {
    Tick();
    ASSERT_EQ(ticks, i);
  }
  effect = Effect<Timer>::None();
  Tick();
  ASSERT_EQ(ticks, kTime.value());
}