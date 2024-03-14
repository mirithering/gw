#include <bits/stdc++.h>

#include "base/clock.h"
#include "base/logging.h"
#include "character/action.h"
#include "character/character.h"
#include "character/skill.h"

namespace {
Character* NextTarget(Character& character,
                      std::vector<std::unique_ptr<Character>>& enemies) {
  for (auto& enemy : enemies) {
    if (enemy->GetActionType() != Action::Type::Dead) {
      return enemy.get();
    }
  }
  return nullptr;
}

const Character* EnemyVeryClose(const Character& character, World& world) {
  for (const auto& other : world.EnemiesOf(character)) {
    if (InRange(character.GetPosition(), other->GetPosition(), AdjacentRange)) {
      return other.get();
    }
  }
  return nullptr;
}

void NextAction(Character& character, World& world) {
  for (unsigned int i = 0; i < character.GetBuild().GetSkills().size(); ++i) {
    Skill* skill = character.GetBuild().GetSkill<Skill>(i);
    assert(skill);
    if (skill->CanActivate(character, world)) {
      LOG << character.name_ << " activating " << skill->Name();
      character.UseSkill(skill, world);
      return;
    }
  }

  if (!IsMeele(character.GetBuild().GetWeapon().GetType()) &&
      character.kiting_) {
    const Character* enemy_very_close = EnemyVeryClose(character, world);
    if (enemy_very_close) {
      LOG << character.name_ << "fleeing";
      character.FleeFrom(*enemy_very_close);
      return;
    }
  }

  if (character.target_ != nullptr) {
    LOG << character.name_ << " starting weapon attack.";
    character.StartWeaponAttack();
  }
}
}  // namespace

void NextActions(World& world) {
  for (auto& character : world.team) {
    if (character->GetActionType() == Action::Type::Idle) {
      if (character->target_ == nullptr) {
        character->target_ = NextTarget(*character, world.enemies);
      }

      NextAction(*character, world);
    }
  }

  for (auto& character : world.enemies) {
    if (character->GetActionType() == Action::Type::Idle) {
      if (character->target_ == nullptr) {
        character->target_ = NextTarget(*character, world.team);
      }

      NextAction(*character, world);
    }
  }
}