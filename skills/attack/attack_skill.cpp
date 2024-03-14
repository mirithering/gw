#include "attack_skill.h"

#include <bits/stdc++.h>

#include "character/character.h"
#include "character/skill.h"
#include "character/world.h"
#include "weapon/weapon.h"

bool AttackSkill::CanActivate(Character& character, World& world) const {
  return character.GetBuild().GetWeapon().GetType() == WeaponType() &&
         Skill::CanActivate(character, world);
}
