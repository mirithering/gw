#ifndef BASE_CLOCK_H
#define BASE_CLOCK_H

#include <bits/stdc++.h>

// There is no guarantee time will be 0 at any point, the only guarantee is that
// it is increased by one on every tick. Only use relatively.
class TimedObject {
 public:
  TimedObject();
  virtual ~TimedObject();

  virtual void Tick(int time_passed) = 0;

  int creation_time_ = 0;
  int last_tick_at_ = -1;
};

void Tick();

#endif  // BASE_CLOCK_H