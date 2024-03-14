#ifndef CONDITIONS_DEEP_WOUND_H
#define CONDITIONS_DEEP_WOUND_H

#include <bits/stdc++.h>

#include "base/function_list.h"
#include "character/condition.h"
#include "character/character.h"

// TODO deep wound also reduces healing effect, but I dongf
class DeepWound : public Condition {
 public:
  void AddModifiers(Character& character) override {
    reference_ = character.callbacks_max_health_.AddFunction(
        [](const Character&) { return Percent(-20); });
  }

  Type GetType() const override { return Type::DeepWound; }

 private:
  FunctionList<Percent(const Character&)>::UniqueReference reference_;
};

#endif  // CONDITIONS_DEEP_WOUND_H