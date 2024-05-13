#ifndef CONDITIONS_DEEP_WOUND_H
#define CONDITIONS_DEEP_WOUND_H

#include <bits/stdc++.h>

#include "base/function_list.h"
#include "entities/condition.h"
#include "entities/creature.h"

// TODO deep wound also reduces healing effect, but I dongf
class DeepWound : public Condition {
 public:
  void AddModifiers(Creature& creature) override {
    reference_ = creature.callbacks_max_health_.AddFunction(
        [](const Creature&) { return Percent(-20); });
  }

  Type GetType() const override { return Type::DeepWound; }

 private:
  FunctionList<Percent(const Creature&)>::UniqueReference reference_;
};

#endif  // CONDITIONS_DEEP_WOUND_H