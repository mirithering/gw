#ifndef CONDITIONS_CRIPPLED_H
#define CONDITIONS_CRIPPLED_H

#include <bits/stdc++.h>

#include "character/condition.h"
#include "character/character.h"

// TODO add effect once characters have a position and move.
class Crippled : public Condition {
 public:
  void AddModifiers(Character& character) override {
    reference_ = character.callbacks_walking_speed_.AddFunction(
        []() { return Percent(-50); });
  }

  ~Crippled() override {}
  Type GetType() const override { return Type::Crippled; }

 private:
  FunctionList<Percent()>::UniqueReference reference_;
};

#endif  // CONDITIONS_BLEEDING_H