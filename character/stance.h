#ifndef CHARACTER_STANCE_H
#define CHARACTER_STANCE_H

#include <bits/stdc++.h>

#include "base/function_list.h"
#include "base/units.h"
#include "weapon/weapon.h"

class Character;

class Stance {
 public:
  virtual ~Stance() = default;
  virtual void AddModifiers(Character& character) = 0;
};

class BlockEverythingStance : public Stance {
 public:
  BlockEverythingStance() = default;
  void AddModifiers(Character& character) override;

  ~BlockEverythingStance() override = default;

  int attack_blocked_called_ = 0;

 private:
  FunctionList<Percent(const Character& character,
                       Weapon::Type)>::UniqueReference block_modifier;
  FunctionList<void(Character& character, Weapon::Type)>::UniqueReference
      block_callback;
};

#endif  // CHARACTER_STANCE_H