#include "action.h"

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "base/random.h"
#include "creature.h"
#include "damage.h"
#include "weapon/weapon.h"

Action Action::WeaponAttack(Creature& source, Creature& target) {
  const Weapon& weapon = source.GetBuild().GetWeapon();
  Time attack_duration = weapon.AttackDuration();

  std::function<Action::Result(Time duration)> tick = [&, attack_duration](
                                                          Time duration) {
    if (duration == attack_duration / 2 &&
        target.GetAction().type_ != Action::Type::Dead) {
      source.WeaponAttack(target);

      // Daggers may attack twice.
      if (weapon.GetType() == Weapon::Type::Dagger) {
        int double_strike_chance =
            2 + kDoubleStrikeChance.at(
                    source.GetBuild().GetAttribute(Attribute::DaggerMastery));
        if (RandomDecision(Chance(double_strike_chance))) {
          source.WeaponAttack(target);
        }
      }
    }
    return Action::Result::Continue;
  };

  return Action(Action::Type::Busy, attack_duration, tick, &DoNothingEnd);
}

Action::Result DoNothingTick(Time) { return Action::Result::Continue; }
void DoNothingEnd() {}