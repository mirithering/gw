#ifndef CONDITIONS_CRIPPLED_H
#define CONDITIONS_CRIPPLED_H

#include <bits/stdc++.h>

#include "character/condition.h"
#include "character/creature.h"

// TODO add effect once characters have a position and move.
class Crippled : public Condition {
 public:
  void AddModifiers(Creature& creature) override {}

  ~Crippled() override {}
  Type GetType() const override { return Type::Crippled; }

 private:
  struct Modifier {
    Creature* creature;
    FunctionList<int()>::ref reference;
  };

  std::optional<Modifier> modifier_;
};

#endif  // CONDITIONS_BLEEDING_H