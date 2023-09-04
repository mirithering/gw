#include "barbarous_slice.h"

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "character/creature.h"
#include "conditions/bleeding.h"

void BarbarousSlice::ActivationMiddle(Creature& creature, World& world) {
  assert(target_ != nullptr);
  int skill_damage = kDamage[creature.GetBuild().GetAttribute(attribute)];
  Time bleeding_duration =
      kBleedingDurationSeconds[creature.GetBuild().GetAttribute(attribute)] *
      Second;

  bool success = creature.WeaponAttack(*target_, skill_damage);
  bool inflict_bleeding = (creature.GetStance() == nullptr);
  if (success && inflict_bleeding) {
    // TODO it seems wrong that I have to pass target_ here twice. Maybe I can
    // do the modifier registration when adding the condition to a creature, and
    // not when creating it?
    target_->AddCondition(
        Effect<Condition>(bleeding_duration, std::make_unique<Bleeding>()));
  }
}