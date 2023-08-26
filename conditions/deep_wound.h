#ifndef CONDITIONS_DEEP_WOUND_H
#define CONDITIONS_DEEP_WOUND_H

#include <bits/stdc++.h>

#include "character/condition.h"

// TODO deep wound also reduces healing effect.
class DeepWound : public Condition {
 public:
  DeepWound(Character& character) : character_(character) {
    reference_ = character.AddMaxHealthModifier(
        [](const Character& character) { return -20; });
  }
  ~DeepWound() override { character_.RemoveMaxHealthModifier(reference_); }

  Type GetType() const override { return Type::DeepWound; }
  // TODO implement deep wound effect.
 private:
  Character& character_;
  Character::MaxHealthModifierRef reference_;
};

#endif  // CONDITIONS_DEEP_WOUND_H