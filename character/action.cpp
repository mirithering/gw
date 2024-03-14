#include "action.h"

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "base/logging.h"
#include "base/random.h"
#include "character.h"
#include "damage.h"
#include "weapon/weapon.h"

Action Action::WeaponAttack(Character& source, Character& target) {
  const Weapon& weapon = source.GetBuild().GetWeapon();
  // TODO walk towards target if not in range.
  Time attack_duration = weapon.AttackDuration();

  std::function<Action::Result(Time duration)> tick = [&, attack_duration](
                                                          Time duration) {
    if (target.GetActionType() == Action::Type::Dead) {
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
      Inches distance_to_target =
          Distance(source.GetPosition(), target.GetPosition());
      assert(distance_to_target <= weapon.GetRange());

      Time flight_time = distance_to_target / weapon.GetFlightSpeed();
      target.AddProjectile(
          Event<>(flight_time, [&]() { source.WeaponAttack(target); }));
    }
  };

  return Action(Action::Type::Busy, attack_duration, tick, end);
}

Action Action::WalkTowardsUntilInRange(Character& source,
                                       const Character& target, Inches range) {
  std::function<Action::Result(Time duration)> tick = [&,
                                                       range](Time duration) {
    if (InRange(source.GetPosition(), target.GetPosition(), range)) {
      LOG << source.name_ << " arrived";
      return Action::Result::End;
    }
    source.OneStepTowards(target.GetPosition());
    return Action::Result::Continue;
  };
  // TODO Give up if your target is moving faster than you
  return Action(Action::Type::Busy, Time(INT_MAX), tick, &DoNothingEnd);
}

Action Action::WalkAwayFromUntilOutOfRange(Character& character,
                                           const Character& away_from,
                                           Inches range) {
  Time expected_flee_time = range / character.GetWalkingSpeed();
  std::function<Action::Result(Time duration)> tick = [&,
                                                       range](Time duration) {
    if (!InRange(character.GetPosition(), away_from.GetPosition(), range)) {
      return Action::Result::End;
    }
    character.OneStepAwayFrom(away_from.GetPosition());
    return Action::Result::Continue;
  };
  // TODO Give up if your target is moving faster than you
  return Action(Action::Type::Busy, Time(expected_flee_time), tick,
                &DoNothingEnd);
}

Action::Result DoNothingTick(Time) { return Action::Result::Continue; }
void DoNothingEnd() {}