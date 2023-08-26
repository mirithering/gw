#include <bits/stdc++.h>

#include "armor/armor.h"
#include "base/random.h"
#include "character.h"
#include "weapon/weapon.h"

double BaseDamage(const Weapon& weapon, bool requirement_met) {
  double modifier = 1;
  if (!requirement_met) modifier = 0.33;
  return RandomValue(weapon.MinDamage(), weapon.MaxDamage()) * modifier;
}

int WeaponStrikeDamage(const Character& attacker, const Character& defender) {
  bool weapon_requirement_met = true;
  if (attacker.GetAttribute(attacker.weapon().GetAttribute()) <
      attacker.weapon().Requirement()) {
    weapon_requirement_met = false;
  }

  double base_damage = BaseDamage(attacker.weapon(), weapon_requirement_met);

  int armor_rating = defender.armor().Rating();
  int attribute_level = attacker.GetAttribute(attacker.weapon().GetAttribute());
  int strike_level = 5 * attribute_level;

  double actual_damage =
      base_damage * std::pow(2.0, ((strike_level - armor_rating) / 40.0));
  return actual_damage;
}