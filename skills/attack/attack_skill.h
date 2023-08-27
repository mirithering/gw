#ifndef SKILLS_ATTACK_ATTACK_SKILL_H
#define SKILLS_ATTACK_ATTACK_SKILL_H

#include <bits/stdc++.h>

#include "character/creature.h"
#include "character/skill.h"
#include "weapon/weapon.h"

class AttackSkill : public Skill {
 public:
  virtual ~AttackSkill() = default;
  bool CanActivate(const Creature& character) const override {
    return character.GetBuild().GetWeapon().GetType() == WeaponType() &&
           Skill::CanActivate(character);
  }

 protected:
  virtual Weapon::Type WeaponType() const = 0;
};

#endif