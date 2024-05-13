#ifndef SKILLS_ATTACK_ATTACK_SKILL_H
#define SKILLS_ATTACK_ATTACK_SKILL_H

#include <bits/stdc++.h>

#include "entities/creature.h"
#include "entities/skill.h"
#include "entities/world.h"
#include "weapon/weapon.h"

class AttackSkill : public Skill {
 public:
  AttackSkill() = default;
  virtual ~AttackSkill() = default;
  bool CanActivate(Creature& creature, World& world) const override;
  Creature* GetTarget(Creature& creature, World& world) const override {
    return creature.target_;
  }
  Inches GetRange(const Creature& creature) const override {
    return creature.GetBuild().GetWeapon().GetRange();
  }
  Skill::Type GetType() const override { return Skill::Type::Attack; }

 protected:
  virtual Weapon::Type WeaponType() const = 0;
};

#endif