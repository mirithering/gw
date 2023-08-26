
#ifndef DESPERATE_STRIKE_H
#define DESPERATE_STRIKE_H

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "character/action.h"
#include "character/character.h"
#include "character/damage.h"
#include "character/skill.h"

// TODO this is a first hand attack, need to set the state to allow for chains.
class DesperateStrike : public AttackSkill {
 public:
  std::string Name() const override { return "Desperate Strike"; }

 protected:
  void ActivationMiddle(Character& source, Character& target) override {
    AttackSkill::ActivationMiddle(source, target);

    int health_below = healthAndDamage.at(source.GetAttribute(attribute)).first;
    int added_damage =
        healthAndDamage.at(source.GetAttribute(attribute)).second;

    int skill_damage = 0;
    if ((source.health() * 100) / source.GetMaxHealth() < health_below) {
      skill_damage = added_damage;
    }

    source.WeaponAttack(target, skill_damage);
  }

  int AdrenalineCost() const override { return 0; }
  int EnergyCost() const override { return 5; }
  int RechargeTime() const override { return 6000; }
  int ActivationTime(Character& character) const override {
    return character.weapon().AttackSpeed();
  }
  Weapon::Type WeaponType() const override { return Weapon::Type::Dagger; };

 private:
  inline static const Attribute attribute = Attribute::DaggerMastery;
  // This skill does second (additional) damage if health % is below first.
  inline static const std::map<int, std::pair<int, int>> healthAndDamage{
      {0, {50, 15}},  {1, {52, 18}},  {2, {54, 21}},  {3, {56, 24}},
      {4, {58, 27}},  {5, {60, 30}},  {6, {62, 33}},  {7, {64, 36}},
      {8, {66, 39}},  {9, {68, 42}},  {10, {70, 45}}, {11, {72, 48}},
      {12, {74, 51}}, {13, {76, 54}}, {14, {78, 57}}, {15, {80, 60}},
      {16, {82, 63}}, {17, {84, 66}}, {18, {86, 69}}, {19, {88, 72}},
      {20, {90, 75}}, {21, {92, 78}}};
};

#endif  // DESPERATE_STRIKE_H