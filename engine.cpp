#include <bits/stdc++.h>

#include "base/clock.h"
#include "base/logging.h"
#include "character/action.h"
#include "character/creature.h"
#include "character/skill.h"

namespace {
Creature* NextTarget(Creature& creature,
                     std::vector<std::unique_ptr<Creature>>& enemies) {
  for (auto& enemy : enemies) {
    if (enemy->GetAction().GetType() != Action::Type::Dead) {
      return enemy.get();
    }
  }
  return nullptr;
}

Action NextAction(Creature& creature, World& world) {
  for (unsigned int i = 0; i < creature.GetBuild().GetSkills().size(); ++i) {
    Skill* skill = creature.GetBuild().GetSkill<Skill>(i);
    assert(skill);
    if (skill->CanActivate(creature, world)) {
      LOG << creature.name_ << " activating " << skill->Name();
      return skill->Activate(creature, world);
    }
  }

  if (creature.target_ != nullptr) {
    LOG << creature.name_ << " starting weapon attack.";
    return Action::WeaponAttack(creature, *creature.target_);
  }

  return kActionIdle;
}
}  // namespace

void NextActions(World& world) {
  for (auto& creature : world.team) {
    if (creature->GetAction().GetType() == Action::Type::Idle) {
      if (creature->target_ == nullptr) {
        creature->target_ = NextTarget(*creature, world.enemies);
      }

      creature->GetAction() = NextAction(*creature, world);
    }
  }

  for (auto& creature : world.enemies) {
    if (creature->GetAction().GetType() == Action::Type::Idle) {
      if (creature->target_ == nullptr) {
        creature->target_ = NextTarget(*creature, world.team);
      }

      creature->GetAction() = NextAction(*creature, world);
    }
  }
}