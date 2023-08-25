#include "bonettis_defense.h"

#include "character/character.h"
#include "character/skill.h"
#include "character/stance.h"

Action BonettisDefense::Use(Character& source, Character&) {
  Skill::ActivationStart(source);

  source.SetStance(std::make_unique<BonettisDefenseStance>(
      duration[source.GetAttribute(attribute)] * 1000, source));

  Skill::ActivationEnd(source);
  return kActionIdle;
};

int BonettisDefenseStance::BlockChance(Weapon::Type type) {
  if (IsMeele(type) || IsProjectile(type)) {
    return 75;
  }
  return 0;
}

void BonettisDefenseStance::AttackBlocked(Weapon::Type type) {
  if (IsMeele(type)) {
    character_.AddEnergy(5);
  }
}