#ifndef SKILL_H
#define SKILL_H

#include <bits/stdc++.h>

#include "action.h"
#include "base/clock.h"
#include "damage.h"

class Character;

class Skill : public TimedObject {
 public:
  virtual bool CanActivate(const Character& character) const;
  virtual ~Skill() override = default;

  Action Activate(Character& source, Character& target);

  void Tick(int) override final { recharge_ = std::max(0, recharge_ - 1); }

  void AddAdrenaline(int units);
  void LoseAdrenaline(int units);
  void LoseAllAdrenaline();
  int GetAdrenaline() const;

  virtual std::string Name() const = 0;

 protected:
  virtual int AdrenalineCost() const = 0;
  virtual int EnergyCost() const = 0;
  virtual int RechargeTime() const = 0;
  virtual int ActivationTime(Character& character) const = 0;

  virtual void ActivationStart(Character& character);
  virtual void ActivationMiddle(Character& source, Character& target){};
  virtual void ActivationEnd(Character& character);

 private:
  int adrenaline_ = 0;  // In theory, adrenaline is also lost when being idle.
                        // But I don't think I care.
  int recharge_ = 0;
};

#endif