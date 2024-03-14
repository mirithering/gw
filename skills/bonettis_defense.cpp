#include "bonettis_defense.h"

#include <bits/stdc++.h>

#include "character/character.h"
#include "character/skill.h"
#include "character/stance.h"
#include "character/world.h"

void BonettisDefense::ActivationEnd(Character& character, World& world) {
  Time time = kDuration[character.GetBuild().GetAttribute(kAttribute)] * Second;
  character.SetStance(
      Effect<Stance>(time, std::make_unique<BonettisDefenseStance>()));

  Skill::ActivationEnd(character, world);
};
