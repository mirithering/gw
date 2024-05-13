#ifndef SKILLS_HEX_SOOTHING_IMAGES_H
#define SKILLS_HEX_SOOTHING_IMAGES_H

#include <bits/stdc++.h>

#include "base/units.h"
#include "entities/creature.h"
#include "entities/skill.h"
#include "skills/hex/hex_skill.h"

class SoothingImages : public HexSkill {
 public:
  std::string Name() const override { return "Soothing Images"; }

  Creature* GetTarget(Creature& creature, World& world) const override;
  Inches GetRange(const Creature&) const override { return CastingRange; }

 protected:
  void ActivationEnd(Creature& creature, World& world) override;

  Adrenaline AdrenalineCost() const override { return 0 * Strike; }
  int EnergyCost() const override { return 15; }
  Time RechargeTime() const override { return 8 * Second; }
  Time ActivationTime(Creature& creature) const override { return 2 * Second; }
};

#endif  // SKILLS_HEX_SOOTHING_IMAGES_H