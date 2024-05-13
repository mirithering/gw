#ifndef SKILLS_HEX_HEX_SKILL_H
#define SKILLS_HEX_HEX_SKILL_H

#include <bits/stdc++.h>

#include "character/character.h"
#include "character/skill.h"
#include "character/world.h"
#include "weapon/weapon.h"

class HexSkill : public Skill {
 public:
  HexSkill() = default;
  virtual ~HexSkill() = default;
  Skill::Type GetType() const override { return Skill::Type::HexSpell; }
};

#endif  // SKILLS_HEX_HEX_SKILL_H