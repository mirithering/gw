
#include "desperate_strike.h"

#include <bits/stdc++.h>

#include "character/character.h"
#include "character/damage.h"
#include "character/skill.h"

bool DesperateStrike::CanUse(const Character& character) const {
  return Skill::CanUse(character) &&
         character.weapon().GetType() == Weapon::Type::Dagger;
}

Action DesperateStrike::Use(Character& source, Character& target) {
  Skill::ActivationStart(source);

  int activation_time = source.weapon().AttackSpeed();

  int health_below = healthAndDamage.at(source.GetAttribute(attribute)).first;
  int added_damage = healthAndDamage.at(source.GetAttribute(attribute)).second;

  int skill_damage = 0;
  if ((source.health() * 100) / source.GetMaxHealth() < health_below) {
    skill_damage = added_damage;
  }

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