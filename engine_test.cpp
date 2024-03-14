#include "engine.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "armor/armor.h"
#include "base/attribute.h"
#include "base/logging.h"
#include "base/profession.h"
#include "character/creature.h"
#include "character/skill.h"
#include "skills/attack/bow/arcing_shot.h"
#include "skills/attack/sword/barbarous_slice.h"
#include "skills/attack/sword/crippling_slash.h"
#include "skills/attack/sword/gash.h"
#include "skills/attack/sword/jaizenju_strike.h"
#include "skills/attack/sword/pure_strike.h"
#include "skills/bonettis_defense.h"
#include "skills/hex/imagined_burden.h"
#include "skills/hex/soothing_images.h"
#include "test/test.h"
#include "weapon/dagger.h"
#include "weapon/flatbow.h"
#include "weapon/sword.h"

class EngineTest : public GwTest {};

// TODO this test needs to change when something about random changes, but it
// guarantees I am not changing anything else.
TEST_F(EngineTest, WarriorWithSkillsKillsWarriorInExactlyXTurns) {
  int kTime = 33916;

  auto attacker = AddWarriorTo(team());
  attacker->GetBuild().SetAttribute(Attribute::Swordsmanship, 12);
  attacker->GetBuild().SetAttribute(Attribute::Strength, 12);
  attacker->GetBuild().SetSkills(
      std::make_unique<PureStrike>(), std::make_unique<JaizenjuStrike>(),
      std::make_unique<BarbarousSlice>(), std::make_unique<Gash>());
  attacker->name_ = "Attacker";

  auto defender = AddWarriorTo(enemies());
  defender->GetBuild().SetAttribute(Attribute::Tactics, 12);
  defender->GetBuild().AddSkill(std::make_unique<BonettisDefense>());
  defender->name_ = "Defender";

  for (int ticks = 0; ticks < kTime; ++ticks) {
    ASSERT_NE(attacker->GetActionType(), Action::Type::Dead) << " " << ticks;
    ASSERT_NE(defender->GetActionType(), Action::Type::Dead) << " " << ticks;
    Tick();
    NextActions(world());
  }
  ASSERT_EQ(defender->GetActionType(), Action::Type::Dead);
}

TEST_F(EngineTest, RangerAndWarriorWalkTowardsAndFight) {
  int kTime = 25217;

  auto attacker = AddWarriorTo(team());
  attacker->GetBuild().SetAttribute(Attribute::Swordsmanship, 12);
  attacker->GetBuild().SetAttribute(Attribute::Strength, 12);
  attacker->GetBuild().SetSkills(
      std::make_unique<PureStrike>(), std::make_unique<JaizenjuStrike>(),
      std::make_unique<BarbarousSlice>(), std::make_unique<Gash>());
  attacker->SetPosition({Inches(2000), Inches(0)});

  auto defender = AddRangerTo(enemies());
  defender->kiting_ = true;
  defender->GetBuild().SetAttribute(Attribute::Marksmanship, 12);
  defender->GetBuild().AddSkill(std::make_unique<ArcingShot>());
  defender->SetPosition({Inches(0), Inches(2000)});

  for (int ticks = 0; ticks < kTime; ++ticks) {
    ASSERT_NE(attacker->GetActionType(), Action::Type::Dead) << " " << ticks;
    ASSERT_NE(defender->GetActionType(), Action::Type::Dead) << " " << ticks;
    Tick();
    NextActions(world());
  }
  ASSERT_EQ(defender->GetActionType(), Action::Type::Dead);
}

void ModifyLevel6Iboga(Creature* creature) {
  creature->kiting_ = false;
  creature->GetBuild().SetAttribute(Attribute::IllusionMagic, 7);
  creature->GetBuild().SetArmor(std::make_unique<Armor>(20));  // guessed
  creature->GetBuild().AddSkill(std::make_unique<ImaginedBurden>());
  creature->GetBuild().AddSkill(std::make_unique<SoothingImages>());
}

TEST_F(EngineTest, KossVsIboga) {
  int kTime = 25217;

  auto attacker = AddWarriorTo(team());
  attacker->GetBuild().SetAttribute(Attribute::Swordsmanship, 12);
  attacker->GetBuild().SetAttribute(Attribute::Strength, 12);
  attacker->GetBuild().SetSkills(
      std::make_unique<PureStrike>(), std::make_unique<JaizenjuStrike>(),
      std::make_unique<BarbarousSlice>(), std::make_unique<Gash>());
  attacker->SetPosition({Inches(2000), Inches(0)});

  // TODO Iboga at level 6... what else do I need to change? Maybe the staff?
  auto iboga = AddMesmerTo(enemies());
  auto iboga2 = AddMesmerTo(enemies());
  auto iboga3 = AddMesmerTo(enemies());
  ModifyLevel6Iboga(iboga);
  ModifyLevel6Iboga(iboga2);
  ModifyLevel6Iboga(iboga3);

  for (int ticks = 0; ticks < kTime; ++ticks) {
    ASSERT_NE(attacker->GetActionType(), Action::Type::Dead);
    ASSERT_NE(iboga->GetActionType(), Action::Type::Dead)
        << " " << attacker->GetLostHealth();
    Tick();
    NextActions(world());
  }
  ASSERT_EQ(iboga->GetActionType(), Action::Type::Dead);
}
