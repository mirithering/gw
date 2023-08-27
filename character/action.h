#ifndef CHARACTER_STATE_H
#define CHARACTER_STATE_H

#include <bits/stdc++.h>

#include "weapon/weapon.h"

class Creature;

class Action {
 public:
  enum class Type { Idle, Dead, Busy };
  enum class Result { Continue, End };
  Action(Type type, int duration, std::function<Result(int duration)> tick,
         std::function<void()> end)
      : type_(type), duration_(duration), tick_(tick), end_(end) {}

  Result Tick() {
    Result result = Result::Continue;
    if (duration_ > 0) {
      result = tick_(duration_);
    }

    --duration_;

    if (duration_ <= 0 && end_.has_value()) {
      end_.value()();
      end_ = std::nullopt;
    }

    if (duration_ <= 0) {
      return Result::End;
    }

    return result;
  }

  static Action WeaponAttack(Creature& source, Creature& target);

  Type GetType() const { return type_; }

 private:
  Type type_;
  int duration_;
  std::function<Result(int duration)> tick_;
  std::optional<std::function<void()>> end_;
};

Action::Result DoNothingTick(int);
void DoNothingEnd();

const Action kActionDead =
    Action(Action::Type::Dead, INT32_MAX, &DoNothingTick, &DoNothingEnd);

const Action kActionIdle =
    Action(Action::Type::Idle, 1, &DoNothingTick, &DoNothingEnd);

#endif  // CHARACTER_STATE_H