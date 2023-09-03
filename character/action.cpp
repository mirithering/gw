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
    if (target.GetAction().type_ == Action::Type::Dead) {
      return Action::Result::End;
    }
    // There are two types of weapons: Meele weapons attack after half of the
    // attack duration. Ranged wepons emit a projectile at the end of the attack
    // duration.
    if (IsMeele(weapon.GetType()) && duration == attack_duration / 2) {
      source.WeaponAttack(target);

      // Daggers may attack twice.
      if (weapon.GetType() == Weapon::Type::Dagger) {
        int double_strike_chance =
            2 + kDoubleStrikeChance.at(
                    source.GetBuild().GetAttribute(Attribute::DaggerMastery));
        if (RandomDecision(Percent(double_strike_chance))) {
          source.WeaponAttack(target);
        }
      }
    }
    return Action::Result::Continue;
  };

  std::function<void()> end = [&]() {
    if (IsRanged(weapon.GetType())) {
      target.AddProjectile(
          Event<>(weapon.FlightTime(), [&]() { source.WeaponAttack(target); }));
    }
  };

  return Action(Action::Type::Busy, attack_duration, tick, end);
}

Action::Result DoNothingTick(Time) { return Action::Result::Continue; }
void DoNothingEnd() {}