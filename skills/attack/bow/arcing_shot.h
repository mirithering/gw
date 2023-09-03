#ifndef SKILLS_ATTACK_BOW_ARCING_SHOT_H
#define SKILLS_ATTACK_BOW_ARCING_SHOT_H

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "base/units.h"
#include "character/creature.h"

class ArcingShot : public AttackSkill {
 public:
  std::string Name() const override { return "Arcing Shot"; }

 protected:
  void ActivationEnd(
      Creature& creature, std::vector<std::unique_ptr<Creature>>& my_team,
      std::vector<std::unique_ptr<Creature>>& enemy_team) override {
    int additional_damage =
        creature.GetBuild().GetAttribute(Attribute::Marksmanship) + 10;
    // TODO flight time actually also depends on distance to target, and
    // character's modifiers.
    Time flight_time =
        of(creature.GetBuild().GetWeapon().FlightTime(), Percent(150));
    bool blockable = false;

    assert(target_ != nullptr);

    // TODO All this binding to references etc. is problematic... I really seem
    // to trust that neither characters nor skills are moved while my program is
    // running.
    target_->AddProjectile(
        Event<>(flight_time, [&, additional_damage, blockable]() {
          creature.WeaponAttack(*target_, additional_damage, blockable);
        }));
  }

  Adrenaline AdrenalineCost() const override { return 0 * Strike; }
  int EnergyCost() const override { return 5; }
  Time RechargeTime() const override { return 6 * Second; }
  Time ActivationTime(Creature& character) const override {
    return character.GetBuild().GetWeapon().AttackDuration();
  }
  Weapon::Type WeaponType() const override { return Weapon::Type::Bow; };
};

#endif  // SKILLS_ATTACK_BOW_ARCING_SHOT_H