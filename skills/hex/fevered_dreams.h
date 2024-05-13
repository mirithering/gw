#ifndef SKILLS_HEX_FEVERED_DREAMS_H
#define SKILLS_HEX_FEVERED_DREAMS_H

#include <bits/stdc++.h>

#include "entities/skill.h"
#include "skills/hex/hex_skill.h"

class FeveredDreams : public HexSkill {
 public:
  std::string Name() const override { return "Fevered Dreams"; }

  Creature* GetTarget(Creature& creature, World& world) const override {
    return creature.target_;
  }
  Inches GetRange(const Creature&) const override { return CastingRange; }

 protected:
  void ActivationEnd(Creature& creature, World& world) override;

  Adrenaline AdrenalineCost() const override { return 0 * Strike; }
  int EnergyCost() const override { return 10; }
  Time RechargeTime() const override { return 10 * Second; }
  Time ActivationTime(Creature& creature) const override { return 2 * Second; }
};

#endif  // SKILLS_HEX_FEVERED_DREAMS_H
