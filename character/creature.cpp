#include "creature.h"

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "base/logging.h"
#include "base/profession.h"
#include "base/random.h"
#include "damage.h"
#include "skill.h"

namespace {

std::map<Profession, int> kMaxEnergy = {
    {Profession::Warrior, 20},     {Profession::Ranger, 25},
    {Profession::Dervish, 25},     {Profession::Assassin, 25},
    {Profession::Paragon, 30},     {Profession::Elementalist, 30},
    {Profession::Monk, 30},        {Profession::Mesmer, 30},
    {Profession::Necromancer, 30}, {Profession::Ritualist, 30}};

// 1 means 1 energy every 3 seconds. TODO this is wrong, the additional energy
// and energy regen is actually provided by armor.
std::map<Profession, int> kEnergyRegeneration = {
    {Profession::Warrior, 2},     {Profession::Ranger, 3},
    {Profession::Dervish, 4},     {Profession::Assassin, 4},
    {Profession::Paragon, 2},     {Profession::Elementalist, 4},
    {Profession::Monk, 4},        {Profession::Mesmer, 4},
    {Profession::Necromancer, 4}, {Profession::Ritualist, 4}};

constexpr int kMaxHealth = 480;  // At level 20
}  // namespace

Creature::Creature(std::unique_ptr<Build> build) : build_(std::move(build)) {
  health_lost_ = 0;
  energy_ = kMaxEnergy[build_->GetFirstProfession()];
}

void Creature::Tick(Time time_passed) {
  if (time_passed % (3 * Second) == Time(0)) {
    EnergyGeneration();
  }
  if (time_passed % Second == Time(0)) {
    HealthGeneration();
  }

  if (GetAction().Tick() == Action::Result::End) {
    GetAction() = kActionIdle;
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
  energy_ =
      std::min(kMaxEnergy.at(build_->GetFirstProfession()), energy_ + amount);
}

void Creature::LoseHealth(int amount) {
  if (amount > 0) LOG << name_ << " losing " << amount << " health.";
  health_lost_ += amount;
  if (amount > 0 && health_lost_ >= GetMaxHealth()) {
    Die();
  }
}

int Creature::GetMaxHealth() const {
  int max_health = kMaxHealth;
  Percent percent(100);
  for (const auto& modifier : callbacks_max_health_.GetList()) {
    percent += modifier(*this);
  }
  // TODO I think there is a cap to this.. E.g. cannot lose more than 100 max
  // health via deep wound.
  return of(max_health, percent);
}

Effect<Condition>* Creature::AddCondition(Effect<Condition> condition) {
  // TODO not all creatures can have all conditions, e.g. bleeding, deep wound.
  assert(condition.get());
  Condition::Type type = condition.get()->GetType();
  if (conditions_[type].RemainingDuration() >= condition.RemainingDuration()) {
    // Existing condition last longer, not adding anything.
    return nullptr;
  }
  conditions_[type] = std::move(condition);
  conditions_[type].get()->AddModifiers(*this);
  return &conditions_[type];
}

bool Creature::ReceiveWeaponDamage(int damage, Weapon::Type type,
                                   bool blockable) {
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
  const auto& skills = build_->GetSkills();
  std::for_each(std::begin(skills), std::end(skills),
                [adrenaline](const std::unique_ptr<Skill>& skill) {
                  if (skill) skill->AddAdrenaline(adrenaline);
                });
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
  health_lost_ = GetMaxHealth();
  energy_ = 0;
}
