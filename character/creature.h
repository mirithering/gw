#ifndef CREATURE_H
#define CREATURE_H

#include <bits/stdc++.h>

#include "action.h"
#include "base/clock.h"
#include "base/effect.h"
#include "base/function_list.h"
#include "build.h"
#include "condition.h"
#include "stance.h"

class Creature : public TimedObject {
 public:
  Creature(std::unique_ptr<Build> build);
  Creature(Creature&&) = default;
  Creature& operator=(Creature&&) = default;
  ~Creature() override { conditions_.clear(); };

  void Tick(Time time_passed) override;

  bool ReceiveWeaponDamage(int damage, Weapon::Type type,
                           bool blockable = true);
  bool WeaponAttack(Creature& target, int skill_damage = 0,
                    bool blockable = true);

  void UseEnergy(int use_energy) {
    assert(energy_ >= use_energy);
    energy_ -= use_energy;
  }

  void LoseHealth(int amount);
  void AddEnergy(int amount);

  void RemoveOneAdrenalineStrike();

  Stance* SetStance(Effect<Stance> stance) {
    stance_ = std::move(stance);
    GetStance()->AddModifiers(*this);
    return GetStance();
  }

  Effect<Condition>* AddCondition(Effect<Condition> condition);
  const Effect<Condition>& GetCondition(Condition::Type type) {
    return conditions_[type];
  }

  bool HasCondition(Condition::Type type) { return !conditions_[type].Ended(); }

  Stance* GetStance() { return stance_.get(); }

  Action& GetAction() { return action_; }

  int GetMaxHealth() const;
  int GetLostHealth() const { return health_lost_; }
  int energy() const { return energy_; }

  std::string name_ = "Lovely Princess";  // For debugging.

  Build& GetBuild() const { return *build_.get(); }

  // Creatures lock on one target, but will attack other targets with skills if
  // the locked target would be pointless.
  Creature* target_ = nullptr;

  FunctionList<Percent(const Creature& creature)> callbacks_max_health_;
  FunctionList<int()> callbacks_health_generation_;
  FunctionList<Percent(const Creature& creature, Weapon::Type type)>
      callbacks_block_chance_;
  FunctionList<void(Creature& creature, Weapon::Type type)>
      callbacks_attack_blocked_;

 private:
  void HealthGeneration();
  void EnergyGeneration();
  void AddAdrenaline(Adrenaline adrenaline);
  void Die();
  bool WillBlockAttack(Weapon::Type type) const;

  std::unique_ptr<Build> build_;

  Action action_ = kActionIdle;

  Effect<Stance> stance_ = Effect<Stance>::None();
  std::map<Condition::Type, Effect<Condition>> conditions_;

  int health_lost_;
  int energy_;
};

std::ostream& operator<<(std::ostream& out, const Creature& character);

template <class W, class... S>
Creature ConstructCreature(Profession first_profession, W&& weapon,
                           std::map<Attribute, int> attributes = {},
                           S&&... skills) {
  return Creature(std::make_unique<Build>(
      ConstructBuild(first_profession, std::move(weapon), attributes,
                     std::forward<S>(skills)...)));
}

#endif  // CREATURE_H