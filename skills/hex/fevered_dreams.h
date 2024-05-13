#ifndef SKILLS_HEX_FEVERED_DREAMS_H
#define SKILLS_HEX_FEVERED_DREAMS_H

#include <bits/stdc++.h>

#include "character/skill.h"
#include "skills/hex/hex_skill.h"

class FeveredDreams : public HexSkill {
 public:
  std::string Name() const override { return "Fevered Dreams"; }

  Character* GetTarget(Character& character, World& world) const override {
    return character.target_;
  }
  Inches GetRange(const Character&) const override { return CastingRange; }

 protected:
  void ActivationEnd(Character& character, World& world) override;

  Adrenaline AdrenalineCost() const override { return 0 * Strike; }
  int EnergyCost() const override { return 10; }
  Time RechargeTime() const override { return 10 * Second; }
  Time ActivationTime(Character& character) const override {
    return 2 * Second;
  }
};

#endif  // SKILLS_HEX_FEVERED_DREAMS_H
