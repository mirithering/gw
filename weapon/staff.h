#ifndef WEAPON_STAFF_H
#define WEAPON_STAFF_H

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "base/units.h"
#include "weapon.h"

namespace {
const Time kStaffAttackDuration = 1750 * Millisecond;
const Time kStaffFlightTimeAtShortbowRange = 560 * Millisecond;
const Inches kStaffRange = CastingRange;
const int kStaffMinDamage = 11;
const int kStaffMaxDamage = 22;
}  // namespace

class Staff : public Weapon {
 public:
  Staff(DamageType damage_type, Attribute attribute)
      : Staff(damage_type, attribute, kStaffMinDamage, kStaffMaxDamage){};
  Staff(DamageType damage_type, Attribute attribute, int min_damage,
        int max_damage)
      : damage_type_(damage_type),
        attribute_(attribute),
        Weapon({min_damage, max_damage}){};

  Time AttackDuration() const override { return kStaffAttackDuration; }
  DamageType GetDamageType() const override { return damage_type_; }
  Attribute GetAttribute() const override { return attribute_; }
  Type GetType() const override { return Type::Staff; }

  Inches GetRange() const override { return kStaffRange; }
  Speed GetFlightSpeed() const override {
    return Speed(Inches(ShortbowRange.value() /
                        kStaffFlightTimeAtShortbowRange.value()));
  }

 private:
  DamageType damage_type_;
  Attribute attribute_;
};

#endif  // WEAPON_STAFF_H