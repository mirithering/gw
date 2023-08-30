#ifndef CONDITIONS_DEEP_WOUND_H
#define CONDITIONS_DEEP_WOUND_H

#include <bits/stdc++.h>

#include "character/condition.h"
#include "character/creature.h"

// TODO deep wound also reduces healing effect, but I dongf
class DeepWound : public Condition {
 public:
 public:
  void AddModifiers(Creature& creature) {
    modifier_ = {
        .creature = &creature,
        .reference = creature.GetModifiersMaxHealthPercentage().AddFunction(
            [](const Creature& character) { return -20; }),
    };
  }

  ~DeepWound() override {
    if (modifier_.has_value()) {
      modifier_->creature->GetModifiersMaxHealthPercentage().RemoveFunction(
          modifier_->reference);
    }
  }
  Type GetType() const override { return Type::DeepWound; }

 private:
  struct Modifier {
    Creature* creature;
    FunctionList<int(const Creature& character)>::ref reference;
  };

  std::optional<Modifier> modifier_;
};

#endif  // CONDITIONS_DEEP_WOUND_H