#ifndef CONDITIONS_BLEEDING_H
#define CONDITIONS_BLEEDING_H

#include <bits/stdc++.h>

#include "character/condition.h"
#include "character/creature.h"

class Bleeding : public Condition {
 public:
  void AddModifiers(Creature& creature) override {
    modifier_ = {
        .creature = &creature,
        .reference = creature.GetModifiersHealthGeneration().AddFunction(
            []() { return -3; }),
    };
  }

  ~Bleeding() override {
    if (modifier_.has_value()) {
      modifier_->creature->GetModifiersHealthGeneration().RemoveFunction(
          modifier_->reference);
    }
  }
  Type GetType() const override { return Type::Bleeding; }

 private:
  struct Modifier {
    Creature* creature;
    FunctionList<int()>::ref reference;
  };

  std::optional<Modifier> modifier_;
};

#endif  // CONDITIONS_BLEEDING_H