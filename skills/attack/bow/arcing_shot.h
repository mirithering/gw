#ifndef SKILLS_ATTACK_BOW_ARCING_SHOT_H
#define SKILLS_ATTACK_BOW_ARCING_SHOT_H

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "base/units.h"
#include "entities/creature.h"

class ArcingShot : public AttackSkill {
 public:
  std::string Name() const override { return "Arcing Shot"; }

 protected:
  void ActivationEnd(Creature& creature, World& world) override {
    assert(target_ != nullptr);

    int additional_damage =
        creature.GetBuild().GetAttribute(Attribute::Marksmanship) + 10;

    Inches distance_to_target =
        Distance(creature.GetPosition(), target_->GetPosition());
    assert(distance_to_target <= creature.GetBuild().GetWeapon().GetRange());

    Time flight_time =
        distance_to_target / creature.GetBuild().GetWeapon().GetFlightSpeed();

    flight_time = of(flight_time, Percent(150));

    bool blockable = false;

    target_->AddProjectile(
        Event<>(flight_time, [&, additional_damage, blockable]() {
          creature.WeaponAttack(*target_, additional_damage, blockable);
        }));
  }

  Adrenaline AdrenalineCost() const override { return 0 * Strike; }
  int EnergyCost() const override { return 5; }
  Time RechargeTime() const override { return 6 * Second; }
  Time ActivationTime(Creature& creature) const override {
    return creature.GetBuild().GetWeapon().AttackDuration();
  }
  Weapon::Type WeaponType() const override { return Weapon::Type::Bow; };
};

#endif  // SKILLS_ATTACK_BOW_ARCING_SHOT_H