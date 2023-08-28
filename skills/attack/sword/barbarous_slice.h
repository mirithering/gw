#ifndef SKILLS_BARBAROUS_SLICE_H
#define SKILLS_BARBAROUS_SLICE_H

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "character/creature.h"
#include "conditions/bleeding.h"

class BarbarousSlice : public AttackSkill {
 public:
  std::string Name() const override { return "Barbarous Slice"; }

 protected:
  void ActivationMiddle(Creature& creature, std::vector<Creature>& my_team,
                        std::vector<Creature>& enemy_team) override;

  Adrenaline AdrenalineCost() const override { return 6 * Strike; }
  int EnergyCost() const override { return 0; }
  Time RechargeTime() const override { return Time(0); }
  Time ActivationTime(Creature& character) const override {
    return character.GetBuild().GetWeapon().AttackDuration();
  }
  Weapon::Type WeaponType() const override { return Weapon::Type::Sword; };

 private:
  inline static const Attribute attribute = Attribute::Swordsmanship;
  inline static int kDamage[]{5,  7,  8,  10, 12, 13, 15, 17, 18, 20, 22,
                              23, 25, 27, 28, 30, 32, 33, 35, 37, 38, 40};
  inline static int kBleedingDurationSeconds[]{5,  6,  6,  7,  8,  8,  9,  10,
                                               10, 11, 12, 12, 13, 14, 14, 15,
                                               16, 16, 17, 18, 18, 19};
};

#endif  // SKILLS_BARBAROUS_SLICE_H