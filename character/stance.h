#ifndef CHARACTER_STANCE_H
#define CHARACTER_STANCE_H

#include "base/clock.h"
#include "weapon/weapon.h"

class Stance {
 public:
  virtual int BlockChance(Weapon::Type type) { return 0; }
  virtual void AttackBlocked(Weapon::Type type) {}
};

#endif  // CHARACTER_STANCE_H