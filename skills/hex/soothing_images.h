#ifndef SKILLS_HEX_SOOTHING_IMAGES_H
#define SKILLS_HEX_SOOTHING_IMAGES_H

#include <bits/stdc++.h>

#include "base/units.h"
#include "character/character.h"
#include "character/skill.h"

class SoothingImages : public Skill {
 public:
  std::string Name() const override { return "Soothing Images"; }

  Character* GetTarget(Character& character, World& world) const override;
  Inches GetRange(const Character&) const override { return CastingRange; }

 protected:
  void ActivationEnd(Character& character, World& world) override;

  Adrenaline AdrenalineCost() const override { return 0 * Strike; }
  int EnergyCost() const override { return 15; }
  Time RechargeTime() const override { return 8 * Second; }
  Time ActivationTime(Character& character) const override {
    return 2 * Second;
  }
};

#endif  // SKILLS_HEX_SOOTHING_IMAGES_H