#ifndef SKILLS_RITUAL_NATURE_RITUAL_EDGE_OF_EXTINCTION_H
#define SKILLS_RITUAL_NATURE_RITUAL_EDGE_OF_EXTINCTION_H

#include <bits/stdc++.h>

#include "base/units.h"
#include "entities/creature.h"
#include "entities/skill.h"

class EdgeOfExtinction : public Skill {
 public:
  std::string Name() const override { return "Edge of Extinction"; }

  Creature* GetTarget(Creature& creature, World& world) const override {
    return &creature;
  }
  Inches GetRange(const Creature&) const override { return CastingRange; }

 protected:
  void ActivationEnd(Creature& creature, World& world) override;

  Adrenaline AdrenalineCost() const override { return 0 * Strike; }
  int EnergyCost() const override { return 5; }
  Time RechargeTime() const override { return 60 * Second; }
  Time ActivationTime(Creature& creature) const override { return 5 * Second; }

 private:
  std::unique_ptr<TimedObject> spirit_actor_;
};

#endif  // SKILLS_RITUAL_NATURE_RITUAL_EDGE_OF_EXTINCTION_H