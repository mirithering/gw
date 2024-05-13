#include "event.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "clock.h"
#include "effect_deprecated.h"
#include "units.h"

TEST(EventTest, EventIsResolvedAfterTime) {
  bool callback_called = false;
  Event<> event(2 * Millisecond, [&]() { callback_called = true; });
  Tick();
  ASSERT_FALSE(callback_called);
  Tick();
  ASSERT_TRUE(callback_called);
}

TEST(EventTest, EventIsNotCalledIfDestroyedEarly) {
  bool callback_called = false;
  std::optional<Event<>> event;
  event.emplace(2 * Millisecond, [&]() { callback_called = true; });
  Tick();
  event = std::nullopt;
  Tick();
  ASSERT_FALSE(callback_called);
}

TEST(EventTest, EventsCanBeMoved) {
  int callback_called = 0;
  std::optional<Event<>> event;
  event.emplace(Millisecond, [&]() { callback_called++; });
  Event<> moved_event = std::move(event.value());
  event = std::nullopt;
  Tick();
  ASSERT_EQ(callback_called, 1);
}
