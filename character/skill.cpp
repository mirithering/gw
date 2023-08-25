#include <bits/stdc++.h>

#include "action.h"
#include "character.h"
#include "damage.h"

bool Skill::CanUse(const Character& character) const {
  return recharge_ == 0 && adrenaline_ >= AdrenalineCost() &&
         character.energy() >= EnergyCost();
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

void Skill::ActivationEnd(Character& character) { recharge_ = RechargeTime(); }
