#include <bits/stdc++.h>

#include "base/clock.h"
#include "character/action.h"
#include "character/character.h"
#include "character/skill.h"

namespace {
Action NextAction(Character& character, Character& enemy) {
  if (enemy.health() > 0) {
    for (int i = 0; i < 8; ++i) {
      Skill* skill = character.GetSkill(i);
      if (!skill) continue;
      if (skill->CanUse(character)) {
        std::cout << character.name_ << ": " << skill->Name()
                  << " energy: " << character.energy() << std::endl;
        return skill->Use(character, enemy);
      }
    }
    std::cout << character.name_ << ": "
              << "weapon attack" << std::endl;
    return Action::WeaponAttack(character, enemy);
  }
  return kActionIdle;
}
}  // namespace

void Tick(Character& character, Character& enemy, int time) {
  // End stances.
  if (character.GetStance() &&
      character.GetStance()->Tick() == Stance::State::End) {
    character.SetStance(nullptr);
  }

  // Continue current action.
  if (character.GetAction().Tick() == Action::Result::End) {
    // New action.
    character.GetAction() = NextAction(character, enemy);
  }
}