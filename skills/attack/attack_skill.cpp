#include "attack_skill.h"

#include <bits/stdc++.h>

#include "character/creature.h"
#include "character/skill.h"
#include "weapon/weapon.h"

bool AttackSkill::CanActivate(const Creature& creature,
                              const std::vector<Creature>& my_team,
                              const std::vector<Creature>& enemy_team) const {
  return creature.GetBuild().GetWeapon().GetType() == WeaponType() &&
         Skill::CanActivate(creature, my_team, enemy_team);
}
