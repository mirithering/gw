#include "barbarous_slice.h"

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "character/character.h"
#include "conditions/bleeding.h"

void BarbarousSlice::ActivationMiddle(Character& character, World& world) {
  assert(target_ != nullptr);
  int skill_damage = kDamage[character.GetBuild().GetAttribute(attribute)];
  Time bleeding_duration =
      kBleedingDurationSeconds[character.GetBuild().GetAttribute(attribute)] *
      Second;

  bool success = character.WeaponAttack(*target_, skill_damage);
  bool inflict_bleeding = (character.GetStance() == nullptr);
  if (success && inflict_bleeding) {
    target_->AddCondition(
        Effect<Condition>(bleeding_duration, std::make_unique<Bleeding>()));
  }
}