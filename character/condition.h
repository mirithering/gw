#ifndef CHARACTER_CONDITION_H
#define CHARACTER_CONDITION_H

#include <bits/stdc++.h>

#include "base/clock.h"

class Condition {
 public:
  virtual ~Condition() = default;
  enum class Type { Bleeding, DeepWound };
  virtual Type GetType() const = 0;
};

#endif  // CHARACTER_CONDITION_H