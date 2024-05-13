
#include "stance.h"

#include <bits/stdc++.h>

#include "base/function_list.h"
#include "base/units.h"
#include "creature.h"
#include "entities/creature.h"
#include "entities/skill.h"

void BlockEverythingStance::AddModifiers(Creature& creature) {
  block_modifier = creature.callbacks_block_chance_.AddFunction(
      [](const Creature& creature, Weapon::Type type) { return Percent(100); });
  block_callback = creature.callbacks_attack_blocked_.AddFunction(
      [&](Creature& creature, Weapon::Type type) { ++attack_blocked_called_; });
}
