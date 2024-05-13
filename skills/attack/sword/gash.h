#ifndef SKILLS_GASH_H
#define SKILLS_GASH_H

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "conditions/deep_wound.h"
#include "entities/creature.h"

class Gash : public AttackSkill {
 public:
  std::string Name() const override { return "Gash"; }

 protected:
  void ActivationMiddle(Creature& creature, World& world) override {
    assert(target_);
    bool is_bleeding = target_->HasCondition(Condition::Type::Bleeding);
    int skill_damage = 0;
    if (is_bleeding) {
      skill_damage = Damage(creature.GetBuild().GetAttribute(attribute));
    }
    Time deep_wound_duration =
        DeepWoundDurationSeconds(creature.GetBuild().GetAttribute(attribute)) *
        Second;

    bool success = creature.WeaponAttack(*target_, skill_damage);

    if (success && is_bleeding) {
      target_->AddCondition(EffectDeprecated<Condition>(
          deep_wound_duration, std::make_unique<DeepWound>()));
    }
  }

  Adrenaline AdrenalineCost() const override { return 6 * Strike; }
  int EnergyCost() const override { return 0; }
  Time RechargeTime() const override { return Time(0); }
  Time ActivationTime(Creature& creature) const override {
    return creature.GetBuild().GetWeapon().AttackDuration();
  }
  Weapon::Type WeaponType() const override { return Weapon::Type::Sword; };

 private:
  inline static const Attribute attribute = Attribute::Swordsmanship;
  int Damage(int swordsmanship) { return swordsmanship + 5; }
  int DeepWoundDurationSeconds(int swordsmanship) { return swordsmanship + 5; }
};

#endif  // SKILLS_BARBAROUS_SLICE_H