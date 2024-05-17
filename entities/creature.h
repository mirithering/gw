#ifndef CREATURE_H
#define CREATURE_H

#include <bits/stdc++.h>

#include "action.h"
#include "base/clock.h"
#include "base/effect_deprecated.h"
#include "base/event.h"
#include "base/function_list.h"
#include "build.h"
#include "condition.h"
#include "effect.h"
#include "entities/world.h"
#include "hex.h"
#include "stance.h"

class Creature : public TimedObject {
 public:
  enum Type { Human, Spirit };

  Creature(std::unique_ptr<Build> build,
           Position initial_position = {Inches(0), Inches(0)},
           Type type = Human);
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

  // TODO I think I can make this one private if I move the weapon attack action
  // over here.
  bool WeaponAttack(Creature& target, int skill_damage = 0,
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

  Stance* SetStance(EffectDeprecated<Stance> stance) {
    stance_ = std::move(stance);
    GetStance()->AddModifiers(*this);
    return GetStance();
  }

  EffectDeprecated<Condition>* AddCondition(
      EffectDeprecated<Condition> condition);
  const EffectDeprecated<Condition>& GetCondition(Condition::Type type) {
    return conditions_[type];
  }

  EffectDeprecated<Hex>* AddHex(EffectDeprecated<Hex> hex);
  bool IsHexed();

  Effect* AddEffect(std::unique_ptr<Effect> effect);

  bool HasEffect(Effect::Type type) {
    if (!effects_.contains(type)) return false;
    return !effects_.at(type)->Ended();
  };

  bool HasCondition(Condition::Type type) { return !conditions_[type].Ended(); }

  int ConditionsCount() {
    int count = 0;
    for (const auto& effect : effects_) {
      if (!effect.second->Ended() && IsCondition(effect.second->GetType())) {
        ++count;
      }
    }
    return count;
  }

  Stance* GetStance() { return stance_.get(); }

  Action::Type GetActionType() const { return action_.GetType(); }

  void UseSkill(Skill* skill, World& world);

  void StartWeaponAttack();

  void WalkTowards(const Creature& target, Inches target_range);
  void FleeFrom(const Creature& target);

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

  // Creatures lock on one target, but will attack other targets with skills if
  // the locked target would be pointless.
  Creature* target_ = nullptr;

  FunctionList<Percent(const Creature& creature)> callbacks_max_health_;
  FunctionList<int()> callbacks_health_generation_;
  FunctionList<Percent(const Creature& creature, Weapon::Type type)>
      callbacks_block_chance_;
  FunctionList<Percent()> callbacks_walking_speed_;
  FunctionList<void(Creature& creature, Weapon::Type type)>
      callbacks_attack_blocked_;
  FunctionList<bool()> callbacks_can_gain_adrenaline_;
  FunctionList<Percent()> callbacks_spell_casting_speed_;
  FunctionList<void(const Creature& creature,
                    const EffectDeprecated<Condition>& condition, World& world)>
      callbacks_add_condition_;
  FunctionList<void(const Creature& creature)> callbacks_death_;

  Position GetPosition() const { return position_; }

  // TODO should that be used?
  void SetPosition(Position position) { position_ = position; }

  // Should go into engine/AI. Whether this creature will run away if enemies
  // are too close.
  bool kiting_ = false;

  const Type type_;
  void Die();

 private:
  void HealthGeneration();
  void EnergyGeneration();
  bool CanGainAdrenaline();
  void AddAdrenaline(Adrenaline adrenaline);
  bool WillBlockAttack(Weapon::Type type) const;

  std::unique_ptr<Build> build_;

  Position position_;

  Action action_ = kActionIdle;
  Action next_action_ = kActionIdle;

  std::vector<Event<>> incoming_projectiles_;

  EffectDeprecated<Stance> stance_ = EffectDeprecated<Stance>::None();
  std::map<Condition::Type, EffectDeprecated<Condition>> conditions_;
  std::map<Hex::Type, EffectDeprecated<Hex>> hexes_;

  std::unordered_map<Effect::Type, std::unique_ptr<Effect>> effects_;

  int health_lost_;
  int energy_;
};

std::ostream& operator<<(std::ostream& out, const Creature& creature);

template <class W, class... S>
std::unique_ptr<Creature> ConstructCreature(
    Profession first_profession, std::unique_ptr<W> weapon,
    std::map<Attribute, int> attributes = {}, std::unique_ptr<S>... skills) {
  return std::make_unique<Creature>(ConstructBuild(
      first_profession, std::move(weapon), attributes, std::move(skills)...));
}

template <class W, class... S>
std::unique_ptr<Creature> ConstructCreature(
    Profession first_profession, std::unique_ptr<W> weapon, Position position,
    std::map<Attribute, int> attributes = {}, std::unique_ptr<S>... skills) {
  return std::make_unique<Creature>(
      ConstructBuild(first_profession, std::move(weapon), attributes,
                     std::move(skills)...),
      position);
}

#endif  // CHARACTER_H_H