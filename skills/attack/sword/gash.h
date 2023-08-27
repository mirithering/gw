#ifndef SKILLS_GASH_H
#define SKILLS_GASH_H

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "character/creature.h"
#include "character/skill.h"
#include "conditions/deep_wound.h"

class Gash : public AttackSkill {
 public:
  std::string Name() const override { return "Gash"; }

 protected:
  void ActivationMiddle(Creature& source, Creature& target) override {
    bool is_bleeding = target.HasCondition(Condition::Type::Bleeding);
    int skill_damage = 0;
    if (is_bleeding) {
      skill_damage = Damage(source.GetBuild().GetAttribute(attribute));
    }
    int deep_wound_duration =
        DeepWoundDurationSeconds(source.GetBuild().GetAttribute(attribute)) *
        1000;

    bool success = source.WeaponAttack(target, skill_damage);

    if (success && is_bleeding) {
      std::cout << "Inflicting deep wound." << std::endl;
      target.AddCondition(Effect<Condition>(
          deep_wound_duration, std::make_unique<DeepWound>(target)));
    }
  }

  int AdrenalineCost() const override { return 6 * 25; }
  int EnergyCost() const override { return 0; }
  int RechargeTime() const override { return 0; }
  int ActivationTime(Creature& character) const override {
    return character.GetBuild().GetWeapon().AttackSpeed();
  }
  Weapon::Type WeaponType() const override { return Weapon::Type::Sword; };

 private:
  inline static const Attribute attribute = Attribute::Swordsmanship;
  int Damage(int swordsmanship) { return swordsmanship + 5; }
  int DeepWoundDurationSeconds(int swordsmanship) { return swordsmanship + 5; }
};

#endif  // SKILLS_BARBAROUS_SLICE_H