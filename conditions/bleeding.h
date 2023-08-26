#ifndef CONDITIONS_BLEEDING_H
#define CONDITIONS_BLEEDING_H

#include <bits/stdc++.h>

#include "character/condition.h"

class Bleeding : public Condition {
 public:
  int HealthGeneration() const override { return -3; }
  Type GetType() const override { return Type::Bleeding; }
};

#endif  // CONDITIONS_BLEEDING_H