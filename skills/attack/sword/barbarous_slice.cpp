#include "barbarous_slice.h"

#include <bits/stdc++.h>

#include "../attack_skill.h"
#include "character/creature.h"
#include "conditions/bleeding.h"

void BarbarousSlice::ActivationMiddle(Creature& creature,
                                      std::vector<Creature>& my_team,
                                      std::vector<Creature>& enemy_team) {
  assert(target_ != nullptr);
  int skill_damage = kDamage[creature.GetBuild().GetAttribute(attribute)];
  int bleeding_duration =
      kBleedingDurationSeconds[creature.GetBuild().GetAttribute(attribute)] *
      1000;

  bool success = creature.WeaponAttack(*target_, skill_damage);
  bool inflict_bleeding = (creature.GetStance() == nullptr);
  if (success && inflict_bleeding) {
    target_->AddCondition(
        Effect<Condition>(bleeding_duration, std::make_unique<Bleeding>()));
  }
}