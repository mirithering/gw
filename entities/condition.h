#ifndef CHARACTER_CONDITION_H
#define CHARACTER_CONDITION_H

#include <bits/stdc++.h>

#include "base/clock.h"

class Creature;

class Condition {
 public:
  Condition() = default;
  Condition(const Condition&) = delete;
  Condition& operator=(const Condition&) = delete;
  virtual ~Condition() = default;

  virtual void AddModifiers(Creature& creature) = 0;
  enum class Type {
    Bleeding,
    Blind,
    Burning,
    CrackedArmor,
    Crippled,
    Dazed,
    DeepWound,
    Disease,
    Poison,
    Weakness
  };
  virtual Type GetType() const = 0;
};

#endif  // CHARACTER_CONDITION_H