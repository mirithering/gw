#ifndef WEAPON_FLATBOW_H
#define WEAPON_FLATBOW_H

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "base/units.h"
#include "weapon.h"

namespace {
const Time kFlatBowAttackDuration = 2025 * Millisecond;
const Time kFlatBowFlightTime = 880 * Millisecond;
const int kFlatBowMinDamage = 15;
const int kFlatBowMaxDamage = 28;
}  // namespace

class Flatbow : public Weapon {
 public:
  Flatbow() : Weapon({kFlatBowMinDamage, kFlatBowMaxDamage}){};
  Flatbow(int min_damage, int max_damage) : Weapon({min_damage, max_damage}){};

  Time FlightTimeDeprecated() const override { return kFlatBowFlightTime; }
  Time AttackDuration() const override { return kFlatBowAttackDuration; }
  DamageType GetDamageType() const override { return DamageType::Piercing; }
  Attribute GetAttribute() const override { return Attribute::Marksmanship; }
  Type GetType() const override { return Type::Bow; }

  Inches GetRange() const override { return FlatbowRange; }
  Speed GetFlightSpeed() const override {
    return Speed(Inches(1.1409));
  }  // 880ms for 1004inches.
};

#endif  // WEAPON_FLATBOW_H