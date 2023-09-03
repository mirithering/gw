#ifndef SKILLS_ATTACK_ATTACK_SKILL_H
#define SKILLS_ATTACK_ATTACK_SKILL_H

#include <bits/stdc++.h>

#include "character/creature.h"
#include "character/skill.h"
#include "weapon/weapon.h"

class AttackSkill : public Skill {
 public:
  AttackSkill() = default;
  virtual ~AttackSkill() = default;
  bool CanActivate(
      const Creature& creature,
      const std::vector<std::unique_ptr<Creature>>& my_team,
      const std::vector<std::unique_ptr<Creature>>& enemy_team) const override;

 protected:
  virtual Weapon::Type WeaponType() const = 0;
};

#endif