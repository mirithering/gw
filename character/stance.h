#ifndef CHARACTER_STANCE_H
#define CHARACTER_STANCE_H

#include <bits/stdc++.h>

#include "base/function_list.h"
#include "base/units.h"
#include "weapon/weapon.h"

class Creature;

class Stance {
 public:
  virtual ~Stance() = default;
  virtual void AddModifiers(Creature& creature) = 0;
};

class BlockEverythingStance : public Stance {
 public:
  BlockEverythingStance() = default;
  void AddModifiers(Creature& creature) override;

  ~BlockEverythingStance() override = default;

  int attack_blocked_called_ = 0;

 private:
  FunctionList<Percent(const Creature& creature, Weapon::Type)>::UniqueReference
      block_modifier;
  FunctionList<void(Creature& creature, Weapon::Type)>::UniqueReference
      block_callback;
};

#endif  // CHARACTER_STANCE_H