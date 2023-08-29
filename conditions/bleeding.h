#ifndef CONDITIONS_BLEEDING_H
#define CONDITIONS_BLEEDING_H

#include <bits/stdc++.h>

#include "character/condition.h"
#include "character/creature.h"

class Bleeding : public Condition {
 public:
  Bleeding(Creature& creature) : creature_(creature) {
    reference_ = creature.GetModifiersHealthGeneration().AddFunction(
        []() { return -3; });
  }

  ~Bleeding() override {
    creature_.GetModifiersHealthGeneration().RemoveFunction(reference_);
  }
  Type GetType() const override { return Type::Bleeding; }

 private:
  Creature& creature_;
  FunctionList<int()>::ref reference_;
};

#endif  // CONDITIONS_BLEEDING_H