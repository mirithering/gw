#include <bits/stdc++.h>

#include "armor/armor.h"
#include "base/random.h"
#include "character.h"
#include "weapon/weapon.h"

double BaseDamage(const Weapon& weapon, bool requirement_met) {
  double modifier = 1;
  if (!requirement_met) modifier = 1.0 / 3.0;
  return RandomValue(weapon.MinDamage(), weapon.MaxDamage()) * modifier;
}

int WeaponStrikeDamage(const Character& attacker, const Character& defender) {
  bool weapon_requirement_met = true;
  const Weapon& weapon = attacker.GetBuild().GetWeapon();
  if (attacker.GetBuild().GetAttribute(weapon.GetAttribute()) <
      weapon.Requirement()) {
    weapon_requirement_met = false;
  }

  double base_damage = BaseDamage(weapon, weapon_requirement_met);

  // TODO Get the damage type and calculate armor against that type.
  int armor_rating = defender.GetBuild().GetArmor()->Rating();
  int attribute_level = attacker.GetBuild().GetAttribute(weapon.GetAttribute());
  int strike_level = 5 * attribute_level;

  double actual_damage =
      base_damage * std::pow(2.0, ((strike_level - armor_rating) / 40.0));
  return actual_damage;
}