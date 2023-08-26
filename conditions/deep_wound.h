#ifndef CONDITIONS_DEEP_WOUND_H
#define CONDITIONS_DEEP_WOUND_H

#include <bits/stdc++.h>

#include "character/condition.h"

class DeepWound : public Condition {
 public:
  Type GetType() const override { return Type::DeepWound; }
  // TODO implement deep wound effect.
};

#endif  // CONDITIONS_DEEP_WOUND_H