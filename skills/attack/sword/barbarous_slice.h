#ifndef SKILLS_BARBAROUS_SLICE_H
#define SKILLS_BARBAROUS_SLICE_H

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "character/character.h"
#include "character/skill.h"
#include "conditions/bleeding.h"

class BarbarousSlice : public AttackSkill {
 public:
  std::string Name() const override { return "Barbarous Slice"; }

 protected:
  void ActivationMiddle(Character& source, Character& target) override {
    int skill_damage = kDamage[source.GetAttribute(attribute)];
    int bleeding_duration =
        kBleedingDurationSeconds[source.GetAttribute(attribute)] * 1000;

    bool success = source.WeaponAttack(target, skill_damage);
    bool inflict_bleeding = source.GetStance() == nullptr;
    if (success && inflict_bleeding) {
      source.AddCondition(
          Effect<Condition>(bleeding_duration, std::make_unique<Bleeding>()));
    }
  }

  int AdrenalineCost() const override { return 6 * 25; }
  int EnergyCost() const override { return 0; }
  int RechargeTime() const override { return 0; }
  int ActivationTime(Character& character) const override {
    return character.weapon().AttackSpeed();
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