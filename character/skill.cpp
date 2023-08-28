#include "skill.h"

#include <bits/stdc++.h>

#include "character/action.h"
#include "character/creature.h"
#include "character/damage.h"

bool Skill::CanActivate(const Creature& creature,
                        const std::vector<Creature>& my_team,
                        const std::vector<Creature>& enemy_team) const {
  return recharge_ == Time(0) && adrenaline_ >= AdrenalineCost() &&
         creature.energy() >= EnergyCost();
}

Action Skill::Activate(Creature& creature, std::vector<Creature>& my_team,
                       std::vector<Creature>& enemy_team) {
  ActivationStart(creature, my_team, enemy_team);

  Time activation_time = ActivationTime(creature);

  std::function<Action::Result(Time)> tick = [&,
                                              activation_time](Time duration) {
    if (duration == activation_time / 2) {
      ActivationMiddle(creature, my_team, enemy_team);
    }
    // TODO cancel skills with dead targets.
    return Action::Result::Continue;
  };

  if (activation_time == Time(0)) {
    ActivationEnd(creature, my_team, enemy_team);
    return kActionIdle;
  }

  std::function<void()> end = [&]() {
    ActivationEnd(creature, my_team, enemy_team);
  };

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

Creature* Skill::GetTarget(Creature& creature, std::vector<Creature>& my_team,
                           std::vector<Creature>& enemy_team) {
  return creature.target_;
}
void Skill::ActivationStart(Creature& creature, std::vector<Creature>& my_team,
                            std::vector<Creature>& enemy_team) {
  adrenaline_ = Adrenaline(0);
  if (AdrenalineCost() > Adrenaline(0)) {
    creature.RemoveOneAdrenalineStrike();
  }
  target_ = GetTarget(creature, my_team, enemy_team);
  creature.UseEnergy(EnergyCost());
}

void Skill::ActivationEnd(Creature& creature, std::vector<Creature>& my_team,
                          std::vector<Creature>& enemy_team) {
  recharge_ = RechargeTime();
  target_ = nullptr;
}
