#include "clock.h"

#include <bits/stdc++.h>

namespace {
std::set<std::function<void(int time_passed)>*> g_ticks;
unsigned int g_time = 0;

void RegisterTick(std::function<void(int time_passed)>* tick) {
  g_ticks.insert(tick);
}

void UnregisterTick(std::function<void(int time_passed)>* tick) {
  g_ticks.erase(tick);
}

}  // namespace

TimedObject::TimedObject() : creation_time_(g_time) {
  tick_ = [&](int time) { Tick(time - creation_time_); };
  RegisterTick(&tick_);
}

TimedObject::~TimedObject() { UnregisterTick(&tick_); }

void Tick() {
  ++g_time;
  for (auto tick : g_ticks) {
    (*tick)(g_time);
  }
}
