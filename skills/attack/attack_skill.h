#ifndef SKILLS_ATTACK_ATTACK_SKILL_H
#define SKILLS_ATTACK_ATTACK_SKILL_H

#include <bits/stdc++.h>

#include "character/character.h"
#include "character/skill.h"
#include "character/world.h"
#include "weapon/weapon.h"

class AttackSkill : public Skill {
 public:
  AttackSkill() = default;
  virtual ~AttackSkill() = default;
  bool CanActivate(Character& character, World& world) const override;
  Character* GetTarget(Character& character, World& world) const override {
    return character.target_;
  }
  Inches GetRange(const Character& character) const override {
    return character.GetBuild().GetWeapon().GetRange();
  }
  Skill::Type GetType() const override { return Skill::Type::Attack; }

 protected:
  virtual Weapon::Type WeaponType() const = 0;
};

#endif