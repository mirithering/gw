#ifndef WEAPON_SWORD_H
#define WEAPON_SWORD_H

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "weapon.h"

namespace {
const int kSwordAttackSpeed = 1330;  // 1 attack per 1330ms
const int kSwordMinDamage = 15;
const int kSwordMaxDamage = 22;
}  // namespace

class Sword : public Weapon {
 public:
  Sword() : Weapon({kSwordMinDamage, kSwordMaxDamage}){};
  Sword(int min_damage, int max_damage) : Weapon({min_damage, max_damage}){};

  int AttackSpeed() const override { return kSwordAttackSpeed; }
  DamageType GetDamageType() const override { return DamageType::Slashing; }
  Attribute GetAttribute() const override { return Attribute::Swordsmanship; }
  Type GetType() const override { return Type::Sword; }
};

#endif  // WEAPON_SWORD_H