#ifndef SKILL_H
#define SKILL_H

#include <bits/stdc++.h>

#include "base/clock.h"
#include "character/world.h"

class Action;
class Creature;

// TODO implement aftercast delay
class Skill : public TimedObject {
 public:
  Skill() = default;

  virtual bool CanActivate(Creature& creature, World& world) const;
  virtual ~Skill() override = default;

  Action Activate(Creature& creature, World& world);

  void Tick(Time) override final {
    recharge_ = std::max(Time(0), recharge_ - Millisecond);
  }

  void AddAdrenaline(Adrenaline adrenaline);
  void LoseAdrenaline(Adrenaline adrenaline);
  void LoseAllAdrenaline();
  Adrenaline GetAdrenaline() const;

  virtual std::string Name() const = 0;

  virtual Creature* GetTarget(Creature& creature, World& world) const = 0;

  virtual Inches GetRange(const Creature& creature) const = 0;

 protected:
  Creature* target_ = nullptr;
  virtual Adrenaline AdrenalineCost() const = 0;
  virtual int EnergyCost() const = 0;
  virtual Time RechargeTime() const = 0;
  virtual Time ActivationTime(Creature& character) const = 0;

  virtual void ActivationStart(Creature& creature, World& world);
  virtual void ActivationMiddle(Creature& creature, World& world){};
  virtual void ActivationEnd(Creature& creature, World& world);

 private:
  Adrenaline adrenaline_ =
      Adrenaline(0);  // In theory, adrenaline is also lost when being idle.
                      // But I don't think I care.
  Time recharge_ = Time(0);
};

#endif