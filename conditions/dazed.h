#ifndef CONDITIONS_DAZED_H
#define CONDITIONS_DAZED_H

#include <bits/stdc++.h>

#include "character/character.h"
#include "character/condition.h"

class Dazed : public Condition {
 public:
  void AddModifiers(Character& character) override {
    // 100 percent more means double as much time...
    // TODO figure out how these calculations work.
    reference_ = character.callbacks_spell_casting_speed_.AddFunction(
        []() { return Percent(100); });
  }

  ~Dazed() override {}
  Type GetType() const override { return Type::Dazed; }

 private:
  FunctionList<Percent()>::UniqueReference reference_;
};

#endif  // CONDITIONS_DAZED_H