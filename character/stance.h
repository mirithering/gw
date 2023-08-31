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

  ~BlockEverythingStance() override;

  int attack_blocked_called_ = 0;

 private:
  struct Modifier {
    Creature* creature;
    FunctionList<Percent(const Creature& creature, Weapon::Type)>::ref
        block_modifier;
    FunctionList<void(Creature& creature, Weapon::Type)>::ref block_callback;
  };

  std::optional<Modifier> modifier_;
};

#endif  // CHARACTER_STANCE_H