#ifndef CONDITIONS_BLEEDING_H
#define CONDITIONS_BLEEDING_H

#include <bits/stdc++.h>

#include "character/condition.h"

class Bleeding : public Condition {
 public:
  int HealthGeneration() { return -3; }
};

#endif  // CONDITIONS_BLEEDING_H