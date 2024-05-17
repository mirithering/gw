#ifndef CONDITIONS_BLEEDING_H
#define CONDITIONS_BLEEDING_H

#include <bits/stdc++.h>

#include "entities/condition.h"
#include "entities/creature.h"
#include "entities/effect.h"

class Bleeding : public Effect {
 public:
  Bleeding(Creature& creature, Time duration)
      : Effect(duration), creature_(creature){};

  Effect::Type GetType() const override { return Effect::Type::Bleeding; }

  int OverrideValue() const override {
    Time remaining_duration = duration_ - time_passed_;
    return remaining_duration.value();
  }

 private:
  void Start() override {
    reference_ =
        creature_.callbacks_health_generation_.AddFunction([]() { return -3; });
  }

  void Tick() override {}
  void End() override { reference_.reset(); }

  Creature& creature_;
  FunctionList<int()>::UniqueReference reference_;
};

#endif  // CONDITIONS_BLEEDING_H