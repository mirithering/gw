#ifndef BASE_CLOCK_H
#define BASE_CLOCK_H

#include <bits/stdc++.h>

class TimedObject {
 public:
  TimedObject();
  TimedObject(const TimedObject& obj) = delete;
  TimedObject& operator=(const TimedObject& obj) = delete;
  TimedObject(TimedObject&& other);
  TimedObject& operator=(TimedObject&& other);
  virtual ~TimedObject();

  virtual void Tick(int time_passed) = 0;

  int creation_time_ = 0;
  int last_tick_at_ = -1;
};

void Tick();

#endif  // BASE_CLOCK_H