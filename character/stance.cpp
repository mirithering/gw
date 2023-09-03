
#include "stance.h"

#include <bits/stdc++.h>

#include "base/function_list.h"
#include "base/units.h"
#include "character/creature.h"
#include "character/skill.h"
#include "creature.h"

void BlockEverythingStance::AddModifiers(Creature& creature) {
  block_modifier = creature.callbacks_block_chance_.AddFunction(
      [](const Creature& character, Weapon::Type type) {
        return Percent(100);
      });
  block_callback = creature.callbacks_attack_blocked_.AddFunction(
      [&](Creature& character, Weapon::Type type) {
        ++attack_blocked_called_;
      });
}
