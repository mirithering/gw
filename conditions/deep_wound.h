#ifndef CONDITIONS_DEEP_WOUND_H
#define CONDITIONS_DEEP_WOUND_H

#include <bits/stdc++.h>

#include "character/condition.h"
#include "character/creature.h"

// TODO deep wound also reduces healing effect, but I dongf
class DeepWound : public Condition {
 public:
  DeepWound(Creature& character) : character_(character) {
    reference_ = character.GetModifiersMaxHealthPercentage().AddFunction(
        [](const Creature& character) { return -20; });
  }
  ~DeepWound() override {
    std::cout << "destroying deep wound" << std::endl;
    character_.GetModifiersMaxHealthPercentage().RemoveFunction(reference_);
  }

  Type GetType() const override { return Type::DeepWound; }

 private:
  Creature& character_;
  FunctionList<int(const Creature& character)>::ref reference_;
};

#endif  // CONDITIONS_DEEP_WOUND_H