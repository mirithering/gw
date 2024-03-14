#include "skill.h"

#include <bits/stdc++.h>

#include "character/action.h"
#include "character/character.h"
#include "character/damage.h"
#include "character/world.h"

bool Skill::CanActivate(Character& character, World& world) const {
  return recharge_ == Time(0) && adrenaline_ >= AdrenalineCost() &&
         character.energy() >= EnergyCost();
}

Action Skill::Activate(Character& character, World& world) {
  assert(CanActivate(character, world));
  ActivationStart(character, world);

  Time activation_time = ActivationTime(character);

  std::function<Action::Result(Time)> tick = [&,
                                              activation_time](Time duration) {
    if (duration == activation_time / 2) {
      ActivationMiddle(character, world);
    }
    // TODO cancel skills with dead targets.
    return Action::Result::Continue;
  };

  if (activation_time == Time(0)) {
    ActivationEnd(character, world);
    return kActionIdle;
  }

  std::function<void()> end = [&]() { ActivationEnd(character, world); };

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

void Skill::ActivationStart(Character& character, World& world) {
  adrenaline_ = Adrenaline(0);
  if (AdrenalineCost() > Adrenaline(0)) {
    character.RemoveOneAdrenalineStrike();
  }
  target_ = GetTarget(character, world);
  character.UseEnergy(EnergyCost());
}

void Skill::ActivationEnd(Character& character,

                          World& world) {
  recharge_ = RechargeTime();
  target_ = nullptr;
}
