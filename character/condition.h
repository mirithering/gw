#ifndef CHARACTER_CONDITION_H
#define CHARACTER_CONDITION_H

#include <bits/stdc++.h>

#include "base/clock.h"

class Condition {
 public:
  virtual int HealthGeneration() { return 0; }
};

#endif  // CHARACTER_CONDITION_H