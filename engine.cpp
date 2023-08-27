#include <bits/stdc++.h>

#include "base/clock.h"
#include "character/action.h"
#include "character/creature.h"
#include "character/skill.h"

namespace {
Action NextAction(Creature& creature, Creature& enemy) {
  if (enemy.GetAction().GetType() != Action::Type::Dead) {
    for (int i = 0; i < creature.GetBuild().GetSkills().size(); ++i) {
      Skill* skill = creature.GetBuild().GetSkill<Skill>(i);
      if (!skill) continue;
      if (skill->CanActivate(creature)) {
        std::cout << creature.name_ << ": " << skill->Name()
                  << " energy: " << creature.energy() << std::endl;
        return skill->Activate(creature, enemy);
      }
    }
    std::cout << creature.name_ << ": "
              << "weapon attack" << std::endl;
    return Action::WeaponAttack(creature, enemy);
  }
  return kActionIdle;
}
}  // namespace

void Tick(Creature& creature, Creature& enemy, int time) {
  if (creature.GetAction().GetType() == Action::Type::Idle) {
    // New action.
    creature.GetAction() = NextAction(creature, enemy);
  }
}