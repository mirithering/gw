#ifndef WEAPON_WEAPON_H
#define WEAPON_WEAPON_H

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "base/units.h"

// TODO critical hits. crit base rate is roughly weapon mastery / 100.

enum class DamageType { Slashing, Piercing };

class Weapon {
 public:
  enum class Type {
    Axe,
    Sword,
    Dagger,
    Hammer,
    Scythe,
    Spear,
    Bow,
    Wand,
    Staff,
    Focus,
    Shield
  };

  Weapon() = default;
  Weapon(std::pair<int, int> damage) : damage_(damage) {}

  Weapon(const Weapon&) = delete;
  Weapon& operator=(const Weapon&) = delete;

  virtual Time AttackDuration() const = 0;
  virtual DamageType GetDamageType() const = 0;
  virtual Attribute GetAttribute() const = 0;
  virtual Type GetType() const = 0;

  virtual Inches GetRange() const = 0;
  virtual Speed GetFlightSpeed() const { return Speed(Inches(0)); }
  // TODO I think flight time depends on distance. Need to use speed and
  // distance later.
  virtual Time FlightTimeDeprecated() const { return Time(0); }

  int MinDamage() const { return damage_.first; };
  int MaxDamage() const { return damage_.second; }

  int Requirement() const { return requirement_; }

 private:
  std::pair<int, int> damage_;
  int requirement_ = 9;
};

bool IsMeele(Weapon::Type type);
bool IsRanged(Weapon::Type type);
bool IsProjectile(Weapon::Type type);

Inches RangeFor(Weapon::Type type);

#endif  // WEAPON_WEAPON_H