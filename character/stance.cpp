
#include "stance.h"

#include <bits/stdc++.h>

#include "base/function_list.h"
#include "base/units.h"
#include "character/character.h"
#include "character/skill.h"
#include "character.h"

void BlockEverythingStance::AddModifiers(Character& character) {
  block_modifier = character.callbacks_block_chance_.AddFunction(
      [](const Character& character, Weapon::Type type) {
        return Percent(100);
      });
  block_callback = character.callbacks_attack_blocked_.AddFunction(
      [&](Character& character, Weapon::Type type) {
        ++attack_blocked_called_;
      });
}
