
#ifndef DESPERATE_STRIKE_H
#define DESPERATE_STRIKE_H

#include <bits/stdc++.h>

#include "character/action.h"
#include "character/character.h"
#include "character/damage.h"
#include "character/skill.h"

// TODO this is a first hand attack, need to set the state to allow for chains.
class DesperateStrike : public Skill {
 public:
  bool CanUse(const Character& character) const override;

  Action Use(Character& source, Character& target) override;

  std::string Name() override { return "Desperate Strike"; }
  int RechargeTime() const override { return 6000; }
  int EnergyCost() const override { return 5; }

 private:
  inline static const Attribute attribute = Attribute::DaggerMastery;
  // This skill does second (additional) damage if health % is below first.
  inline static const std::map<int, std::pair<int, int>> healthAndDamage{
      {0, {50, 15}},  {1, {52, 18}},  {2, {54, 21}},  {3, {56, 24}},
      {4, {58, 27}},  {5, {60, 30}},  {6, {62, 33}},  {7, {64, 36}},
      {8, {66, 39}},  {9, {68, 42}},  {10, {70, 45}}, {11, {72, 48}},
      {12, {74, 51}}, {13, {76, 54}}, {14, {78, 57}}, {15, {80, 60}},
      {16, {82, 63}}, {17, {84, 66}}, {18, {86, 69}}, {19, {88, 72}},
      {20, {90, 75}}, {21, {92, 78}}};
};

#endif  // DESPERATE_STRIKE_H