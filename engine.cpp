#include <bits/stdc++.h>

#include "base/clock.h"
#include "character/action.h"
#include "character/character.h"
#include "character/skill.h"

namespace {
Action NextAction(Character& character, Character& enemy) {
  if (enemy.GetAction().GetType() != Action::Type::Dead) {
    for (int i = 0; i < 8; ++i) {
      Skill* skill = character.GetSkill(i);
      if (!skill) continue;
      if (skill->CanActivate(character)) {
        std::cout << character.name_ << ": " << skill->Name()
                  << " energy: " << character.energy() << std::endl;
        return skill->Activate(character, enemy);
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
  if (character.GetAction().GetType() == Action::Type::Idle) {
    // New action.
    character.GetAction() = NextAction(character, enemy);
  }
}