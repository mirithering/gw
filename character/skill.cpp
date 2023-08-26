#include "skill.h"

#include <bits/stdc++.h>

#include "action.h"
#include "character.h"
#include "damage.h"

bool Skill::CanActivate(const Character& character) const {
  return recharge_ == 0 && adrenaline_ >= AdrenalineCost() &&
         character.energy() >= EnergyCost();
}

Action Skill::Activate(Character& source, Character& target) {
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

void Skill::AddAdrenaline(int units) { adrenaline_ += units; }
void Skill::LoseAdrenaline(int units) { adrenaline_ -= units; }
void Skill::LoseAllAdrenaline() { adrenaline_ = 0; }
int Skill::GetAdrenaline() const { return adrenaline_; }

void Skill::ActivationStart(Character& character) {
  adrenaline_ -= AdrenalineCost();
  if (AdrenalineCost() > 0) {
    character.RemoveOneAdrenalineStrike();
  }
  character.UseEnergy(EnergyCost());
}

void Skill::ActivationEnd(Character& character) {
  std::cout << "activation end" << std::endl;
  recharge_ = RechargeTime();
}
