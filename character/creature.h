#ifndef CREATURE_H
#define CREATURE_H

#include <bits/stdc++.h>

#include "action.h"
#include "base/clock.h"
#include "base/effect.h"
#include "base/event.h"
#include "base/function_list.h"
#include "build.h"
#include "character/world.h"
#include "condition.h"
#include "stance.h"

class Creature : public TimedObject {
 public:
  Creature(std::unique_ptr<Build> build,
           Position initial_position = {Inches(0), Inches(0)});
  // TODO I needed to remove the move constructor because a lot of code depends
  // on references and ptrs to creatures to stay valid. This is a sign of bad
  // design. Maybe I can fix my dependencies at some point and make creatures
  // movable?
  Creature(const Creature&) = delete;
  Creature& operator=(const Creature&) = delete;

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

  Action::Type GetActionType() const { return action_.GetType(); }

  void UseSkill(Skill* skill, World& world);

  void StartWeaponAttack();

  void AddProjectile(Event<>&& projectile) {
    incoming_projectiles_.push_back(std::move(projectile));
  }

  int GetMaxHealth() const;
  int GetLostHealth() const { return health_lost_; }
  int energy() const { return energy_; }

  std::string name_ = "Lovely Princess";  // For debugging.

  // TODO this violates Tell don't ask principle, I have to redirect a lot.. Not
  // sure how to get rid of it.
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

  Position position_;

  Action action_ = kActionIdle;

  std::vector<Event<>> incoming_projectiles_;

  Effect<Stance> stance_ = Effect<Stance>::None();
  std::map<Condition::Type, Effect<Condition>> conditions_;

  int health_lost_;
  int energy_;
};

std::ostream& operator<<(std::ostream& out, const Creature& character);

template <class W, class... S>
std::unique_ptr<Creature> ConstructCreature(
    Profession first_profession, std::unique_ptr<W> weapon,
    std::map<Attribute, int> attributes = {}, std::unique_ptr<S>... skills) {
  return std::make_unique<Creature>(ConstructBuild(
      first_profession, std::move(weapon), attributes, std::move(skills)...));
}

#endif  // CREATURE_H