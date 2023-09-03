#include <bits/stdc++.h>

#include "base/clock.h"
#include "base/logging.h"
#include "character/action.h"
#include "character/creature.h"
#include "character/skill.h"

namespace {
Creature* NextTarget(Creature& creature, std::vector<Creature>& enemies) {
  for (auto& enemy : enemies) {
    if (enemy.GetAction().GetType() != Action::Type::Dead) {
      return &enemy;
    }
  }
  return nullptr;
}

Action NextAction(Creature& creature, std::vector<Creature>& my_team,
                  std::vector<Creature>& enemy_team) {
  if (creature.target_ == nullptr) {
    creature.target_ = NextTarget(creature, enemy_team);
  }

  for (unsigned int i = 0; i < creature.GetBuild().GetSkills().size(); ++i) {
    Skill* skill = creature.GetBuild().GetSkill<Skill>(i);
    assert(skill);
    if (skill->CanActivate(creature, my_team, enemy_team)) {
      LOG << creature.name_ << " activating " << skill->Name();
      return skill->Activate(creature, my_team, enemy_team);
    }
  }

  if (creature.target_ != nullptr) {
    LOG << creature.name_ << " starting weapon attack.";
    return Action::WeaponAttack(creature, *creature.target_);
  }

  return kActionIdle;
}
}  // namespace

void NextActions(std::vector<Creature>& team, std::vector<Creature>& enemies) {
  for (auto& creature : team) {
    if (creature.GetAction().GetType() == Action::Type::Idle) {
      creature.GetAction() = NextAction(creature, team, enemies);
    }
  }

  for (auto& creature : enemies) {
    if (creature.GetAction().GetType() == Action::Type::Idle) {
      creature.GetAction() = NextAction(creature, enemies, team);
    }
  }
}