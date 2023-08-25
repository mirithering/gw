#include "action.h"

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "base/random.h"
#include "character.h"
#include "damage.h"
#include "weapon/weapon.h"

Action Action::WeaponAttack(Character& source, Character& target) {
  int attack_duration = source.weapon().AttackSpeed();

  std::function<Action::Result(int duration)> tick =
      [&, attack_duration](int duration) {
        if (duration == attack_duration / 2 &&
            target.GetAction().type_ != Action::Type::Dead) {
          source.WeaponAttack(target);

          // Daggers may attack twice.
          if (source.weapon().GetType() == Weapon::Type::Dagger) {
            int double_strike_chance =
                2 + kDoubleStrikeChance.at(
                        source.GetAttribute(Attribute::DaggerMastery));
            if (RandomDecision(Chance(double_strike_chance))) {
              source.WeaponAttack(target);
            }
          }
        }
        return Action::Result::Continue;
      };

  return Action(Action::Type::Busy, attack_duration, tick, &DoNothingEnd);
}

Action::Result DoNothingTick(int) { return Action::Result::Continue; }
void DoNothingEnd() {}