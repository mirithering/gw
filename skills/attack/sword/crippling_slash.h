#ifndef CRIPPLING_SLASH_H
#define CRIPPLING_SLASH_H

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "character/creature.h"
#include "character/damage.h"
#include "conditions/bleeding.h"
#include "conditions/crippled.h"

class CripplingSlash : public AttackSkill {
 public:
  std::string Name() const override { return "Crippling Slash"; }

 protected:
  void ActivationMiddle(
      Creature& creature, std::vector<std::unique_ptr<Creature>>& my_team,
      std::vector<std::unique_ptr<Creature>>& enemy_team) override {
    assert(target_);
    bool success = creature.WeaponAttack(*target_);
    if (success) {
      Time cripple_duration =
          kCrippledDurationSeconds[creature.GetBuild().GetAttribute(
              kAttribute)] *
          Second;
      Time bleed_duration = BleedingDurationSeconds(
                                creature.GetBuild().GetAttribute(kAttribute)) *
                            Second;
      target_->AddCondition(
          Effect<Condition>(bleed_duration, std::make_unique<Bleeding>()));
      target_->AddCondition(
          Effect<Condition>(cripple_duration, std::make_unique<Crippled>()));
    }
  }

  Adrenaline AdrenalineCost() const override { return 6 * Strike; }
  int EnergyCost() const override { return 0; }
  Time RechargeTime() const override { return 0 * Second; }
  Time ActivationTime(Creature& character) const override {
    return character.GetBuild().GetWeapon().AttackDuration();
  }
  Weapon::Type WeaponType() const override { return Weapon::Type::Sword; };

 private:
  inline static const Attribute kAttribute = Attribute::Swordsmanship;
  inline static const int kCrippledDurationSeconds[] = {
      5,  6,  6,  7,  8,  8,  9,  10, 10, 11, 12,
      12, 13, 14, 14, 15, 16, 16, 17, 18, 18, 19};
  static int BleedingDurationSeconds(int attribute) { return attribute + 10; }
};

#endif  // CRIPPLING_SLASH_H