#ifndef CHARACTER_STATE_H
#define CHARACTER_STATE_H

#include <bits/stdc++.h>

#include "base/units.h"
#include "weapon/weapon.h"

class Character;

class Action {
 public:
  enum class Type { Idle, Dead, Busy };
  enum class Result { Continue, End };
  Action(Type type, Time duration, std::function<Result(Time duration)> tick,
         std::function<void()> end)
      : type_(type), duration_(duration), tick_(tick), end_(end) {}

  Result Tick() {
    Result result = Result::Continue;

    --duration_;

    if (duration_ >= Time(0)) {
      result = tick_(duration_);
    }

    if (duration_ <= Time(0) && end_.has_value()) {
      end_.value()();
      end_ = std::nullopt;
    }

    if (duration_ <= Time(0)) {
      return Result::End;
    }

    return result;
  }

  static Action WeaponAttack(Character& source, Character& target);

  // Walks towards another character until in range.
  static Action WalkTowardsUntilInRange(Character& source,
                                        const Character& target, Inches range);

  static Action WalkAwayFromUntilOutOfRange(Character& character,
                                            const Character& away_from,
                                            Inches range);

  Type GetType() const { return type_; }

 private:
  Type type_;
  Time duration_;
  std::function<Result(Time duration)> tick_;
  std::optional<std::function<void()>> end_;
};

Action::Result DoNothingTick(Time);
void DoNothingEnd();

const Action kActionDead =
    Action(Action::Type::Dead, Time(INT_MAX), &DoNothingTick, &DoNothingEnd);

const Action kActionIdle =
    Action(Action::Type::Idle, Millisecond, &DoNothingTick, &DoNothingEnd);

#endif  // CHARACTER_STATE_H