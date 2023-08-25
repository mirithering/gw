#ifndef PURE_STRIKE_H
#define PURE_STRIKE_H

#include <bits/stdc++.h>

#include "character/character.h"
#include "character/damage.h"
#include "character/skill.h"

class PureStrike : public Skill {
 public:
  bool CanUse(const Character& character) const override {
    return Skill::CanUse(character) &&
           character.weapon().GetType() == Weapon::Type::Sword;
  }

  Action Use(Character& source, Character& target) override {
    // TODO can not be blocked unless you are in a stance.
    Skill::ActivationStart(source);

    int activation_time = source.weapon().AttackSpeed();
    int skill_damage = damage.at(source.GetAttribute(attribute));

    std::function<Action::Result(int)> tick = [&, activation_time,
                                               skill_damage](int duration) {
      if (duration == activation_time / 2 &&
          target.GetAction().GetType() != Action::Type::Dead) {
        source.WeaponAttack(target, skill_damage);
      }
      return Action::Result::Continue;
    };

    std::function<void()> end = [&]() { Skill::ActivationEnd(source); };

    return Action(Action::Type::Busy, activation_time, tick, end);
  }

  std::string Name() override { return "Pure Strike"; }
  int RechargeTime() const override { return 8000; }
  int EnergyCost() const override { return 5; }

 private:
  inline static const Attribute attribute = Attribute::Swordsmanship;
  inline static const std::map<int, int> damage{
      {0, 1},   {1, 3},   {2, 5},   {3, 7},   {4, 9},   {5, 11},
      {6, 13},  {7, 15},  {8, 16},  {9, 18},  {10, 20}, {11, 22},
      {12, 24}, {13, 26}, {14, 28}, {15, 30}, {16, 32}, {17, 34},
      {18, 36}, {19, 38}, {20, 40}, {21, 42}};
};

#endif  // PURE_STRIKE_H