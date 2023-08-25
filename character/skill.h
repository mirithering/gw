#ifndef SKILL_H
#define SKILL_H

#include <bits/stdc++.h>

#include "action.h"
#include "base/clock.h"
#include "character.h"
#include "damage.h"

class Skill : public TimedObject {
 public:
  virtual bool CanUse(const Character& character) const;

  virtual Action Use(Character& source, Character& target) = 0;

  void Tick(int) override final { recharge_ = std::max(0, recharge_ - 1); }

  virtual std::string Name() = 0;

  void AddAdrenaline(int units);
  void LoseAdrenaline(int units);
  void LoseAllAdrenaline();
  int GetAdrenaline() const;

  virtual int AdrenalineCost() const { return 0; }
  virtual int EnergyCost() const { return 0; }
  virtual int RechargeTime() const { return 0; }

 protected:
  virtual void ActivationStart(Character& character);

  virtual void ActivationEnd(Character& character);

 private:
  int adrenaline_ = 0;  // In theory, adrenaline is also lost when being idle.
                        // But I don't think I care.
  int recharge_ = 0;
};

#endif