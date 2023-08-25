#ifndef WEAPON_DAGGER_H
#define WEAPON_DAGGER_H

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "weapon.h"

namespace {
const int kDaggerAttackSpeed = 1330;
const int kDaggerMinDamage = 7;
const int kDaggerMaxDamage = 17;
}  // namespace

class Dagger : public Weapon {
 public:
  Dagger() : Weapon({kDaggerMinDamage, kDaggerMaxDamage}){};
  Dagger(int min_damage, int max_damage) : Weapon({min_damage, max_damage}){};

  int AttackSpeed() const override { return kDaggerAttackSpeed; }
  DamageType GetDamageType() const override {
    return DamageType::Piercing;
  }  // TODO some daggers to slashing damage.
  Attribute GetAttribute() const override { return Attribute::DaggerMastery; }
  Type GetType() const override { return Type::Dagger; }
};

#endif  // WEAPON_DAGGER_H