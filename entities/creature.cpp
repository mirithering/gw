#include "creature.h"

#include <bits/stdc++.h>

#include "action.h"
#include "base/attribute.h"
#include "base/logging.h"
#include "base/profession.h"
#include "base/random.h"
#include "damage.h"
#include "hex.h"
#include "skill.h"

namespace {

// 1 means 1 energy every 3 seconds. TODO this is wrong, the additional energy
// and energy regen is actually provided by armor.
std::map<Profession, int> kEnergyRegeneration = {
    {Profession::Warrior, 2},     {Profession::Ranger, 3},
    {Profession::Dervish, 4},     {Profession::Assassin, 4},
    {Profession::Paragon, 2},     {Profession::Elementalist, 4},
    {Profession::Monk, 4},        {Profession::Mesmer, 4},
    {Profession::Necromancer, 4}, {Profession::Ritualist, 4}};
}  // namespace

Creature::Creature(std::unique_ptr<Build> build, Position initial_position,
                   Type type)
    : build_(std::move(build)), position_(initial_position), type_(type) {
  health_lost_ = 0;
  energy_ = build_->GetMaxEnergy();
}

void Creature::Tick(Time time_passed) {
  if (time_passed % (3 * Second) == Time(0)) {
    EnergyGeneration();
  }
  if (time_passed % Second == Time(0)) {
    HealthGeneration();
  }

  if (action_.Tick() == Action::Result::End) {
    action_ = next_action_;
    next_action_ = kActionIdle;
  }
}

void Creature::HealthGeneration() {
  int health_generation = 0;  // Base is 0.

  for (auto& modifier_health_generation :
       callbacks_health_generation_.GetList()) {
    health_generation += modifier_health_generation();
  }

  health_generation =
      std::max(-10, health_generation);  // Negative generation is capped at 10.
  LoseHealth(-(2 * health_generation));
}

void Creature::EnergyGeneration() {
  int energy_generation = kEnergyRegeneration.at(build_->GetFirstProfession());
  AddEnergy(energy_generation);
}

void Creature::AddEnergy(int amount) {
  energy_ = std::min(build_->GetMaxEnergy(), energy_ + amount);
}

void Creature::LoseHealth(int amount) {
  if (amount > 0) LOG << name_ << " losing " << amount << " health.";
  health_lost_ += amount;
  if (amount > 0 && health_lost_ >= GetMaxHealth()) {
    Die();
  }
}

int Creature::GetMaxHealth() const {
  int max_health = build_->GetMaxHealth();
  Percent percent(100);
  for (const auto& modifier : callbacks_max_health_.GetList()) {
    percent += modifier(*this);
  }
  // TODO I think there is a cap to this.. E.g. cannot lose more than 100 max
  // health via deep wound.
  return of(max_health, percent);
}

// TODO If I could make this a function that would just receive a duration and a
// type, that might make more sense.
// Then the Condition constructor itself could be Condition(time, type) that
// creates the correct thing? What seems conceptually off is that a condition is
// the same object, no matter if applied or not...
EffectDeprecated<Condition>* Creature::AddCondition(
    EffectDeprecated<Condition> condition) {
  // TODO not all creatures can have all conditions, e.g. bleeding, deep wound.
  assert(condition.get());
  Condition::Type type = condition.get()->GetType();
  if (conditions_[type].RemainingDuration() >= condition.RemainingDuration()) {
    // Existing condition lasts longer, not adding anything.
    return nullptr;
  }
  conditions_[type] = std::move(condition);
  conditions_[type].get()->AddModifiers(*this);

  // TODO run add condition callbacks.

  return &conditions_[type];
}

bool Creature::ReceiveWeaponDamage(int damage, Weapon::Type type,
                                   bool blockable) {
  // TODO interrupt if easily interruptable or dazed. Not sure where to best put
  // this... because any kind of attack seems to interrupt.
  if (blockable && WillBlockAttack(type)) {
    for (const auto& callback : callbacks_attack_blocked_.GetList()) {
      callback(*this, type);
    }

    return false;
  }

  int percentage = damage * 100 / GetMaxHealth();
  AddAdrenaline(Adrenaline(percentage));

  LoseHealth(damage);
  return true;
}

EffectDeprecated<Hex>* Creature::AddHex(EffectDeprecated<Hex> hex) {
  assert(hex.get());
  Hex::Type type = hex.get()->GetType();
  if (hexes_[type].RemainingDuration() >= hex.RemainingDuration()) {
    // Existing condition last longer, not adding anything.
    return nullptr;
  }
  hexes_[type] = std::move(hex);
  hexes_[type].get()->AddModifiers(*this);
  return &hexes_[type];
}

Effect* Creature::AddEffect(std::unique_ptr<Effect> effect) {
  assert(effect);

  if (!effects_.contains(effect->GetType())) {
    Effect* effect_ptr = effect.get();
    effects_[effect->GetType()] = std::move(effect);
    return effect_ptr;
  }

  auto& old_effect = effects_.at(effect->GetType());
  if (old_effect->OverrideValue() <= effect->OverrideValue()) {
    Effect* effect_ptr = effect.get();
    effects_[effect->GetType()] = std::move(effect);
    return effect_ptr;
  }

  return nullptr;
}

bool Creature::IsHexed() {
  for (const auto& hex : hexes_) {
    if (!hex.second.Ended()) {
      return true;
    }
  }
  return false;
}

bool Creature::WillBlockAttack(Weapon::Type type) const {
  Percent block_chance(0);
  for (auto& modifier : callbacks_block_chance_.GetList()) {
    block_chance += modifier(*this, type);
  }
  if (RandomDecision(block_chance)) {
    return true;
  }
  return false;
}

bool Creature::WeaponAttack(Creature& target, int skill_damage,
                            bool blockable) {
  int damage = WeaponStrikeDamage(*this, target);
  bool success = target.ReceiveWeaponDamage(
      damage + skill_damage, build_->GetWeapon().GetType(), blockable);
  if (success) {
    AddAdrenaline(Strike);
  }
  return success;
}

void Creature::AddAdrenaline(Adrenaline adrenaline) {
  if (!CanGainAdrenaline()) {
    return;
  }
  const auto& skills = build_->GetSkills();
  std::for_each(std::begin(skills), std::end(skills),
                [adrenaline](const std::unique_ptr<Skill>& skill) {
                  if (skill) skill->AddAdrenaline(adrenaline);
                });
}

bool Creature::CanGainAdrenaline() {
  for (auto& can_gain : callbacks_can_gain_adrenaline_.GetList()) {
    if (!can_gain()) {
      return false;
    }
  }
  return true;
}

void Creature::RemoveOneAdrenalineStrike() {
  const auto& skills = build_->GetSkills();
  std::for_each(std::begin(skills), std::end(skills),
                [](const std::unique_ptr<Skill>& skill) {
                  if (skill) skill->LoseAdrenaline(Strike);
                });
}

void Creature::Die() {
  action_ = kActionDead;
  // TODO what else happens when you die? Probably lose all timed effects.
  const auto& skills = build_->GetSkills();
  std::for_each(std::begin(skills), std::end(skills),
                [](const std::unique_ptr<Skill>& skill) {
                  if (skill) skill->LoseAllAdrenaline();
                });
  for (auto& on_death : callbacks_death_.GetList()) {
    on_death(*this);
  }
  health_lost_ = GetMaxHealth();
  energy_ = 0;
}

void Creature::UseSkill(Skill* skill, World& world) {
  bool in_range =
      InRange(GetPosition(), skill->GetTarget(*this, world)->GetPosition(),
              skill->GetRange(*this));
  if (in_range) {
    action_ = skill->Activate(*this, world);
    return;
  }
  LOG << name_ << " walking towards target and then " << skill->Name();
  WalkTowards(*skill->GetTarget(*this, world), skill->GetRange(*this));
  next_action_ = skill->Activate(*this, world);
}

void Creature::StartWeaponAttack() {
  assert(target_ != nullptr);

  bool in_range = InRange(GetPosition(), target_->GetPosition(),
                          build_->GetWeapon().GetRange());
  if (in_range) {
    action_ = Action::WeaponAttack(*this, *target_);
    return;
  }

  LOG << name_ << " walking towards target and then weapon attack";
  WalkTowards(*target_, build_->GetWeapon().GetRange());
  next_action_ = Action::WeaponAttack(*this, *target_);
}

void Creature::WalkTowards(const Creature& target, Inches target_range) {
  action_ = Action::WalkTowardsUntilInRange(*this, target, target_range);
}

void Creature::FleeFrom(const Creature& target) {
  action_ =
      Action::WalkAwayFromUntilOutOfRange(*this, target, EarshotRange / 2);
}

void Creature::OneStepTowards(Position target) {
  Direction direction = Towards(position_, target);
  position_ = NextPosition(position_, direction, GetWalkingSpeed());
}

void Creature::OneStepAwayFrom(Position away_from) {
  Direction direction = AwayFrom(position_, away_from);
  // TODO implement speed modifiers.
  position_ = NextPosition(position_, direction, GetWalkingSpeed());
}

Speed Creature::GetWalkingSpeed() {
  Percent modifier = Percent(100);
  // TODO check wether those add up, and not apply one after the other or
  // something.
  for (const auto& modifier_callback : callbacks_walking_speed_.GetList()) {
    modifier += modifier_callback();
  }
  modifier = std::max(Percent(0), modifier);
  // TODO fix this, I should be able to take a percentage of speed.
  return Speed(of(WalkingSpeed.value(), modifier));
}
