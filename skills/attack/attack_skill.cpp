#include "attack_skill.h"

#include <bits/stdc++.h>

#include "character/creature.h"
#include "character/skill.h"
#include "character/world.h"
#include "weapon/weapon.h"

bool AttackSkill::CanActivate(const Creature& creature,
                              const World& world) const {
  return creature.GetBuild().GetWeapon().GetType() == WeaponType() &&
         Skill::CanActivate(creature, world);
}
