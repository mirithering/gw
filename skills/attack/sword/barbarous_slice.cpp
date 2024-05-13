#include "barbarous_slice.h"

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "conditions/bleeding.h"
#include "entities/creature.h"

void BarbarousSlice::ActivationMiddle(Creature& creature, World& world) {
  assert(target_ != nullptr);
  int skill_damage = kDamage[creature.GetBuild().GetAttribute(attribute)];
  Time bleeding_duration =
      kBleedingDurationSeconds[creature.GetBuild().GetAttribute(attribute)] *
      Second;

  bool success = creature.WeaponAttack(*target_, skill_damage);
  bool inflict_bleeding = (creature.GetStance() == nullptr);
  if (success && inflict_bleeding) {
    target_->AddCondition(EffectDeprecated<Condition>(
        bleeding_duration, std::make_unique<Bleeding>()));
  }
}