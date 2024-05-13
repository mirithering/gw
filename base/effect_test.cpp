#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "clock.h"
#include "effect_deprecated.h"

namespace {
const Time kTime = 10 * Second;
constexpr int kNumber = 5;
}  // namespace

TEST(EffectDeprecatedTest, NoEffectDeprecatedIsAlwaysOver) {
  EffectDeprecated<int> effect = EffectDeprecated<int>::None();
  ASSERT_TRUE(effect.Ended());
  ASSERT_EQ(effect.get(), nullptr);
  Tick();
}

TEST(EffectDeprecatedTest, EffectDeprecatedLastsForCorrectTime) {
  EffectDeprecated<int> effect(kTime, std::make_unique<int>(kNumber));

  for (int i = 0; i < kTime.value(); ++i) {
    ASSERT_EQ(effect.RemainingDuration(), kTime - Time(i));
    ASSERT_FALSE(effect.Ended());
    ASSERT_EQ(*effect.get(), kNumber);
    Tick();
  }
  ASSERT_TRUE(effect.Ended());
  ASSERT_EQ(effect.get(), nullptr);
}

TEST(EffectDeprecatedTest, RemovedEffectDeprecatedIsGone) {
  EffectDeprecated<int> effect(kTime, std::make_unique<int>(kNumber));

  for (int i = 0; i < kTime.value() / 2; ++i) {
    ASSERT_FALSE(effect.Ended());
    ASSERT_EQ(*effect.get(), kNumber);
    Tick();
  }

  effect = EffectDeprecated<int>::None();

  Tick();
  Tick();
  ASSERT_TRUE(effect.Ended());
  ASSERT_EQ(effect.get(), nullptr);
}

TEST(EffectDeprecatedTest, EffectDeprecatedWithTimedObject) {
  int ticks = 0;
  class Timer : public TimedObject {
   public:
    explicit Timer(int& ticks) : ticks_(ticks) {}
    void Tick(Time) override { ++ticks_; }
    int& ticks_;
  };

  EffectDeprecated<Timer> effect(kTime, std::make_unique<Timer>(ticks));
  for (int i = 1; i <= kTime.value(); ++i) {
    Tick();
    ASSERT_EQ(ticks, i);
  }
  effect = EffectDeprecated<Timer>::None();
  Tick();
  ASSERT_EQ(ticks, kTime.value());
}