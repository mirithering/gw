#ifndef SKILLS_HEX_SOOTHING_IMAGES_H
#define SKILLS_HEX_SOOTHING_IMAGES_H

#include <bits/stdc++.h>

#include "base/units.h"
#include "character/creature.h"
#include "character/skill.h"

class SoothingImages : public Skill {
 public:
  std::string Name() const override { return "Soothing Images"; }

  Creature* GetTarget(Creature& creature, World& world) const override;
  Inches GetRange(const Creature&) const override { return CastingRange; }

 protected:
  void ActivationEnd(Creature& creature, World& world) override;

  Adrenaline AdrenalineCost() const override { return 0 * Strike; }
  int EnergyCost() const override { return 15; }
  Time RechargeTime() const override { return 8 * Second; }
  Time ActivationTime(Creature& character) const override { return 2 * Second; }
};

#endif  // SKILLS_HEX_SOOTHING_IMAGES_H