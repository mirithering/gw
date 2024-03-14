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
#include "hex.h"
#include "stance.h"

class Character : public TimedObject {
 public:
  Character(std::unique_ptr<Build> build,
            Position initial_position = {Inches(0), Inches(0)});
  // TODO I needed to remove the move constructor because a lot of code depends
  // on references and ptrs to characters to stay valid. This is a sign of bad
  // design. Maybe I can fix my dependencies at some point and make characters
  // movable?
  Character(const Character&) = delete;
  Character& operator=(const Character&) = delete;

  ~Character() override { conditions_.clear(); };

  void Tick(Time time_passed) override;

  bool ReceiveWeaponDamage(int damage, Weapon::Type type,
                           bool blockable = true);

  // TODO I think I can make this one private if I move the weapon attack action
  // over here.
  bool WeaponAttack(Character& target, int skill_damage = 0,
                    bool blockable = true);

  // TODO I think I can make this private if I move walking action over here.
  void OneStepTowards(Position target);
  void OneStepAwayFrom(Position away_from);

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

  Effect<Hex>* AddHex(Effect<Hex> hex);
  bool IsHexed();

  bool HasCondition(Condition::Type type) { return !conditions_[type].Ended(); }

  Stance* GetStance() { return stance_.get(); }

  Action::Type GetActionType() const { return action_.GetType(); }

  void UseSkill(Skill* skill, World& world);

  void StartWeaponAttack();

  void WalkTowards(const Character& target, Inches target_range);
  void FleeFrom(const Character& target);

  void AddProjectile(Event<>&& projectile) {
    incoming_projectiles_.push_back(std::move(projectile));
  }

  int GetMaxHealth() const;
  int GetLostHealth() const { return health_lost_; }
  int energy() const { return energy_; }

  Speed GetWalkingSpeed();

  std::string name_ = "Lovely Princess";  // For debugging.

  // TODO this violates Tell don't ask principle, I have to redirect a lot.. Not
  // sure how to get rid of it.
  Build& GetBuild() const { return *build_.get(); }

  // Characters lock on one target, but will attack other targets with skills if
  // the locked target would be pointless.
  Character* target_ = nullptr;

  FunctionList<Percent(const Character& character)> callbacks_max_health_;
  FunctionList<int()> callbacks_health_generation_;
  FunctionList<Percent(const Character& character, Weapon::Type type)>
      callbacks_block_chance_;
  FunctionList<Percent()> callbacks_walking_speed_;
  FunctionList<void(Character& character, Weapon::Type type)>
      callbacks_attack_blocked_;
  FunctionList<bool()> callbacks_can_gain_adrenaline_;

  Position GetPosition() const { return position_; }

  // TODO should that be used?
  void SetPosition(Position position) { position_ = position; }

  // Should go into engine/AI. Whether this character will run away if enemies
  // are too close.
  bool kiting_ = false;

 private:
  void HealthGeneration();
  void EnergyGeneration();
  bool CanGainAdrenaline();
  void AddAdrenaline(Adrenaline adrenaline);
  void Die();
  bool WillBlockAttack(Weapon::Type type) const;

  std::unique_ptr<Build> build_;

  Position position_;

  Action action_ = kActionIdle;
  Action next_action_ = kActionIdle;

  std::vector<Event<>> incoming_projectiles_;

  Effect<Stance> stance_ = Effect<Stance>::None();
  std::map<Condition::Type, Effect<Condition>> conditions_;
  std::map<Hex::Type, Effect<Hex>> hexes_;

  int health_lost_;
  int energy_;
};

std::ostream& operator<<(std::ostream& out, const Character& character);

template <class W, class... S>
std::unique_ptr<Character> ConstructCharacter(
    Profession first_profession, std::unique_ptr<W> weapon,
    std::map<Attribute, int> attributes = {}, std::unique_ptr<S>... skills) {
  return std::make_unique<Character>(ConstructBuild(
      first_profession, std::move(weapon), attributes, std::move(skills)...));
}

template <class W, class... S>
std::unique_ptr<Character> ConstructCharacter(
    Profession first_profession, std::unique_ptr<W> weapon, Position position,
    std::map<Attribute, int> attributes = {}, std::unique_ptr<S>... skills) {
  return std::make_unique<Character>(
      ConstructBuild(first_profession, std::move(weapon), attributes,
                     std::move(skills)...),
      position);
}

#endif  // CREATURE_H