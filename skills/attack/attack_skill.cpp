#include "attack_skill.h"

#include <bits/stdc++.h>

#include "entities/creature.h"
#include "entities/skill.h"
#include "entities/world.h"
#include "weapon/weapon.h"

bool AttackSkill::CanActivate(Creature& creature, World& world) const {
  return creature.GetBuild().GetWeapon().GetType() == WeaponType() &&
         Skill::CanActivate(creature, world);
}
