#include "clock.h"

#include <bits/stdc++.h>

namespace {
std::list<TimedObject*> g_ticks;
Time g_time(0);

void TickAllTicks(Time current_time) {
  for (auto it = std::begin(g_ticks); it != std::end(g_ticks); ++it) {
    auto size = g_ticks.size();
    if ((*it)->last_tick_at_ == current_time) continue;
    (*it)->last_tick_at_ = current_time;

    (*it)->Tick(g_time - (*it)->creation_time_);
    if (g_ticks.size() != size) {
      // Something was deleted, let's start over.
      TickAllTicks(current_time);
      return;
    }
  }
}

void Remove(TimedObject* obj) {
  for (auto it = std::begin(g_ticks); it != std::end(g_ticks);) {
    auto tmp = it;
    ++it;
    if (*tmp == obj) {
      g_ticks.erase(tmp);
    }
  }
}

}  // namespace

TimedObject::TimedObject() : creation_time_(g_time), last_tick_at_(g_time) {
  g_ticks.push_back(this);
}

TimedObject::TimedObject(TimedObject&& other)
    : creation_time_(other.creation_time_) {
  Remove(&other);
  g_ticks.push_back(this);
  last_tick_at_ = other.last_tick_at_;
}

TimedObject& TimedObject::operator=(TimedObject&& other) {
  Remove(&other);
  Remove(this);
  g_ticks.push_back(this);
  creation_time_ = other.creation_time_;
  last_tick_at_ = other.last_tick_at_;
  return *this;
}

TimedObject::~TimedObject() { Remove(this); }

void Tick() {
  ++g_time;
  TickAllTicks(g_time);
}

Time Now() {
  return g_time;
}
