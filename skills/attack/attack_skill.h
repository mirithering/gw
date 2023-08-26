#ifndef SKILLS_ATTACK_ATTACK_SKILL_H
#define SKILLS_ATTACK_ATTACK_SKILL_H

#include <bits/stdc++.h>

#include "character/character.h"
#include "character/skill.h"
#include "weapon/weapon.h"

class AttackSkill : public Skill {
 public:
  virtual ~AttackSkill() = default;
  bool CanActivate(const Character& character) const override {
    return character.weapon().GetType() == WeaponType() &&
           Skill::CanActivate(character);
  }

 protected:
  virtual Weapon::Type WeaponType() const = 0;
};

#endif