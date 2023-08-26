#ifndef PURE_STRIKE_H
#define PURE_STRIKE_H

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "character/character.h"
#include "character/damage.h"
#include "character/skill.h"

class PureStrike : public AttackSkill {
 public:
  std::string Name() const override { return "Pure Strike"; }

 protected:
  void ActivationMiddle(Character& source, Character& target) override {
    int skill_damage = kDamage.at(source.GetAttribute(kAttribute));
    source.WeaponAttack(target, skill_damage,
                        /*blockable=*/(source.GetStance() != nullptr));
  }

  int AdrenalineCost() const override { return 0; }
  int EnergyCost() const override { return 5; }
  int RechargeTime() const override { return 8000; }
  int ActivationTime(Character& character) const override {
    return character.weapon().AttackSpeed();
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