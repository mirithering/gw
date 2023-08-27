#ifndef SKILL_H
#define SKILL_H

#include <bits/stdc++.h>

#include "base/clock.h"

class Action;
class Creature;

class Skill : public TimedObject {
 public:
  Skill() = default;
  Skill(Skill&& other) = default;
  Skill& operator=(Skill&& other) = default;

  virtual bool CanActivate(const Creature& creature,
                           const std::vector<Creature>& my_team,
                           const std::vector<Creature>& enemy_team) const;
  virtual ~Skill() override = default;

  Action Activate(Creature& creature, std::vector<Creature>& my_team,
                  std::vector<Creature>& enemy_team);

  void Tick(int) override final { recharge_ = std::max(0, recharge_ - 1); }

  void AddAdrenaline(int units);
  void LoseAdrenaline(int units);
  void LoseAllAdrenaline();
  int GetAdrenaline() const;

  virtual std::string Name() const = 0;

 protected:
  Creature* target_ = nullptr;
  virtual int AdrenalineCost() const = 0;
  virtual int EnergyCost() const = 0;
  virtual int RechargeTime() const = 0;
  virtual int ActivationTime(Creature& character) const = 0;

  Creature* GetTarget(Creature& creature, std::vector<Creature>& my_team,
                      std::vector<Creature>& enemy_team);
  virtual void ActivationStart(Creature& creature,
                               std::vector<Creature>& my_team,
                               std::vector<Creature>& enemy_team);
  virtual void ActivationMiddle(Creature& creature,
                                std::vector<Creature>& my_team,
                                std::vector<Creature>& enemy_team){};
  virtual void ActivationEnd(Creature& creature, std::vector<Creature>& my_team,
                             std::vector<Creature>& enemy_team);

 private:
  int adrenaline_ = 0;  // In theory, adrenaline is also lost when being idle.
                        // But I don't think I care.
  int recharge_ = 0;
};

#endif