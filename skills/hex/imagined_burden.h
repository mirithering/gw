#ifndef SKILLS_HEX_IMAGINED_BURDEN_H
#define SKILLS_HEX_IMAGINED_BURDEN_H

#include <bits/stdc++.h>

#include "base/units.h"
#include "character/creature.h"
#include "character/skill.h"

// TODO I can unify some of these into a generic "hex" skill base class
class ImaginedBurden : public Skill {
 public:
  std::string Name() const override { return "Imagined Burden"; }

  Creature* GetTarget(Creature& creature, World& world) const override {
    return creature.target_;
  }
  Inches GetRange(const Creature&) const override { return CastingRange; }

 protected:
  void ActivationEnd(Creature& creature, World& world) override;

  Adrenaline AdrenalineCost() const override { return 0 * Strike; }
  int EnergyCost() const override { return 15; }
  Time RechargeTime() const override { return 30 * Second; }
  Time ActivationTime(Creature& character) const override { return 1 * Second; }
};

#endif  // SKILLS_HEX_IMAGINED_BURDEN_H