#ifndef CHARACTER_CONDITION_H
#define CHARACTER_CONDITION_H

#include <bits/stdc++.h>

#include "base/clock.h"

class Condition {
 public:
  enum class Type { Bleeding, DeepWound };
  virtual int HealthGeneration() const { return 0; }
  virtual Type GetType() const = 0;
};

#endif  // CHARACTER_CONDITION_H