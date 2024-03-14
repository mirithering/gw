#ifndef CONDITIONS_BLEEDING_H
#define CONDITIONS_BLEEDING_H

#include <bits/stdc++.h>

#include "character/condition.h"
#include "character/character.h"

class Bleeding : public Condition {
 public:
  void AddModifiers(Character& character) override {
    modifier_ = {
        .character = &character,
        .reference =
            character.callbacks_health_generation_.AddFunctionDeprecated(
                []() { return -3; }),
    };
  }

  ~Bleeding() override {
    if (modifier_.has_value()) {
      modifier_->character->callbacks_health_generation_.RemoveFunction(
          modifier_->reference);
    }
  }
  Type GetType() const override { return Type::Bleeding; }

 private:
  struct Modifier {
    Character* character;
    FunctionList<int()>::ref reference;
  };

  std::optional<Modifier> modifier_;
};

#endif  // CONDITIONS_BLEEDING_H