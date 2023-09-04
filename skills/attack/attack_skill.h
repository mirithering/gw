#ifndef SKILLS_ATTACK_ATTACK_SKILL_H
#define SKILLS_ATTACK_ATTACK_SKILL_H

#include <bits/stdc++.h>

#include "character/creature.h"
#include "character/skill.h"
#include "character/world.h"
#include "weapon/weapon.h"

class AttackSkill : public Skill {
 public:
  AttackSkill() = default;
  virtual ~AttackSkill() = default;
  bool CanActivate(const Creature& creature, const World& world) const override;

 protected:
  virtual Weapon::Type WeaponType() const = 0;
};

#endif