#include "clock.h"

#include <bits/stdc++.h>

namespace {
std::list<TimedObject*> g_ticks;
unsigned int g_time = 0;

void TickAllTicks(int current_time) {
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

}  // namespace

TimedObject::TimedObject() : creation_time_(g_time) { g_ticks.push_back(this); }

TimedObject::~TimedObject() {
  for (auto it = std::begin(g_ticks); it != std::end(g_ticks); ++it) {
    if (*it == this) {
      g_ticks.erase(it);
      break;
    }
  }
}

void Tick() {
  ++g_time;
  TickAllTicks(g_time);
}
