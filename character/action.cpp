#include "action.h"

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "base/logging.h"
#include "base/random.h"
#include "creature.h"
#include "damage.h"
#include "weapon/weapon.h"

Action Action::WeaponAttack(Creature& source, Creature& target) {
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

Action Action::WalkTowardsUntilInRange(Creature& source, const Creature& target,
                                       Inches range) {
  std::function<Action::Result(Time duration)> tick = [&,
                                                       range](Time duration) {
    if (InRange(source.GetPosition(), target.GetPosition(), range)) {
      LOG << source.name_ << " arrived";
      return Action::Result::End;
    }
    source.OneStepTowards(target.GetPosition());
    return Action::Result::Continue;
  };
  // TODO I'm guessing that we would give up eventually, need to figure out what
  // NPCs do if they can never reach their target.
  return Action(Action::Type::Busy, Time(INT_MAX), tick, &DoNothingEnd);
}

Action::Result DoNothingTick(Time) { return Action::Result::Continue; }
void DoNothingEnd() {}