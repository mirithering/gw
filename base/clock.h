#ifndef BASE_CLOCK_H
#define BASE_CLOCK_H

#include <bits/stdc++.h>

#include "units.h"

class TimedObject {
 public:
  TimedObject();
  TimedObject(const TimedObject& obj) = delete;
  TimedObject& operator=(const TimedObject& obj) = delete;
  virtual ~TimedObject();

  // Called every 1ms of Game time. `time_passed` is the time that passed since
  // creation of this object.
  virtual void Tick(Time time_passed) = 0;

  Time creation_time_;
  Time last_tick_at_;
};

void Tick();

// Don't use this except for testing and logging. Use TimedObjects.
Time Now();

#endif  // BASE_CLOCK_H