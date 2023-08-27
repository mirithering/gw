#ifndef WEAPON_SWORD_H
#define WEAPON_SWORD_H

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "base/units.h"
#include "weapon.h"

namespace {
const Time kSwordAttackDuration = 1330 * Millisecond;
const int kSwordMinDamage = 15;
const int kSwordMaxDamage = 22;
}  // namespace

class Sword : public Weapon {
 public:
  Sword() : Weapon({kSwordMinDamage, kSwordMaxDamage}){};
  Sword(int min_damage, int max_damage) : Weapon({min_damage, max_damage}){};

  Time AttackDuration() const override { return kSwordAttackDuration; }
  DamageType GetDamageType() const override { return DamageType::Slashing; }
  Attribute GetAttribute() const override { return Attribute::Swordsmanship; }
  Type GetType() const override { return Type::Sword; }
};

#endif  // WEAPON_SWORD_H