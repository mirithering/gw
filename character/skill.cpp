#include "skill.h"

#include <bits/stdc++.h>

#include "character/action.h"
#include "character/creature.h"
#include "character/damage.h"
#include "character/world.h"

bool Skill::CanActivate(Creature& creature, World& world) const {
  return recharge_ == Time(0) && adrenaline_ >= AdrenalineCost() &&
         creature.energy() >= EnergyCost();
}

Action Skill::Activate(Creature& creature, World& world) {
  assert(CanActivate(creature, world));
  ActivationStart(creature, world);

  Time activation_time = ActivationTime(creature);

  std::function<Action::Result(Time)> tick = [&,
                                              activation_time](Time duration) {
    if (duration == activation_time / 2) {
      ActivationMiddle(creature, world);
    }
    // TODO cancel skills with dead targets.
    return Action::Result::Continue;
  };

  if (activation_time == Time(0)) {
    ActivationEnd(creature, world);
    return kActionIdle;
  }

  std::function<void()> end = [&]() { ActivationEnd(creature, world); };

  return Action(Action::Type::Busy, activation_time, tick, end);
}

// TODO Check if skills can have a limited amount of adrenaline, e.g. only their
// own costs.
void Skill::AddAdrenaline(Adrenaline adrenaline) { adrenaline_ += adrenaline; }
void Skill::LoseAdrenaline(Adrenaline adrenaline) {
  adrenaline_ = std::max(adrenaline_ - adrenaline, Adrenaline(0));
}
void Skill::LoseAllAdrenaline() { adrenaline_ = Adrenaline(0); }
Adrenaline Skill::GetAdrenaline() const { return adrenaline_; }

void Skill::ActivationStart(Creature& creature, World& world) {
  adrenaline_ = Adrenaline(0);
  if (AdrenalineCost() > Adrenaline(0)) {
    creature.RemoveOneAdrenalineStrike();
  }
  target_ = GetTarget(creature, world);
  creature.UseEnergy(EnergyCost());
}

void Skill::ActivationEnd(Creature& creature,

                          World& world) {
  recharge_ = RechargeTime();
  target_ = nullptr;
}
