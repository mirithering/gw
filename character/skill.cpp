#include "skill.h"

#include <bits/stdc++.h>

#include "action.h"
#include "creature.h"
#include "damage.h"

bool Skill::CanActivate(const Creature& character) const {
  return recharge_ == 0 && adrenaline_ >= AdrenalineCost() &&
         character.energy() >= EnergyCost();
}

Action Skill::Activate(Creature& source, Creature& target) {
  std::cout << source.name_ << " activating skill " << Name() << std::endl;
  ActivationStart(source);

  int activation_time = ActivationTime(source);

  std::function<Action::Result(int)> tick = [&, activation_time](int duration) {
    if (duration == activation_time / 2) {
      ActivationMiddle(source, target);
    }
    // TODO cancel skills with dead targets.
    return Action::Result::Continue;
  };

  if (activation_time == 0) {
    ActivationEnd(source);
    return kActionIdle;
  }

  std::function<void()> end = [&]() { ActivationEnd(source); };

  return Action(Action::Type::Busy, activation_time, tick, end);
}

// TODO Check if skills can have a limited amount of adrenaline, e.g. only their
// own costs.
void Skill::AddAdrenaline(int units) { adrenaline_ += units; }
void Skill::LoseAdrenaline(int units) {
  adrenaline_ = std::max(adrenaline_ - units, 0);
}
void Skill::LoseAllAdrenaline() { adrenaline_ = 0; }
int Skill::GetAdrenaline() const { return adrenaline_; }

void Skill::ActivationStart(Creature& character) {
  adrenaline_ = 0;
  if (AdrenalineCost() > 0) {
    character.RemoveOneAdrenalineStrike();
  }
  character.UseEnergy(EnergyCost());
}

void Skill::ActivationEnd(Creature& character) {
  std::cout << "activation end" << std::endl;
  recharge_ = RechargeTime();
}
