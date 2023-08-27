#ifndef BASE_CLOCK_H
#define BASE_CLOCK_H

#include <bits/stdc++.h>

#include "units.h"

class TimedObject {
 public:
  TimedObject();
  TimedObject(const TimedObject& obj) = delete;
  TimedObject& operator=(const TimedObject& obj) = delete;
  TimedObject(TimedObject&& other);
  TimedObject& operator=(TimedObject&& other);
  virtual ~TimedObject();

  // Called every 1ms of Game time.
  virtual void Tick(Time time_passed) = 0;

  Time creation_time_;
  Time last_tick_at_ = Time(-1);
};

void Tick();

#endif  // BASE_CLOCK_H