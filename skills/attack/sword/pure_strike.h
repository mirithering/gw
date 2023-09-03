#ifndef PURE_STRIKE_H
#define PURE_STRIKE_H

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "character/creature.h"
#include "character/damage.h"

class PureStrike : public AttackSkill {
 public:
  std::string Name() const override { return "Pure Strike"; }

 protected:
  void ActivationMiddle(
      Creature& creature, std::vector<std::unique_ptr<Creature>>& my_team,
      std::vector<std::unique_ptr<Creature>>& enemy_team) override {
    assert(target_);
    int skill_damage = kDamage.at(creature.GetBuild().GetAttribute(kAttribute));
    creature.WeaponAttack(*target_, skill_damage,
                          /*blockable=*/(creature.GetStance() != nullptr));
  }

  Adrenaline AdrenalineCost() const override { return Adrenaline(0); }
  int EnergyCost() const override { return 5; }
  Time RechargeTime() const override { return 8 * Second; }
  Time ActivationTime(Creature& character) const override {
    return character.GetBuild().GetWeapon().AttackDuration();
  }
  Weapon::Type WeaponType() const override { return Weapon::Type::Sword; };

 private:
  inline static const Attribute kAttribute = Attribute::Swordsmanship;
  inline static const std::map<int, int> kDamage{
      {0, 1},   {1, 3},   {2, 5},   {3, 7},   {4, 9},   {5, 11},
      {6, 13},  {7, 15},  {8, 16},  {9, 18},  {10, 20}, {11, 22},
      {12, 24}, {13, 26}, {14, 28}, {15, 30}, {16, 32}, {17, 34},
      {18, 36}, {19, 38}, {20, 40}, {21, 42}};
};

#endif  // PURE_STRIKE_H