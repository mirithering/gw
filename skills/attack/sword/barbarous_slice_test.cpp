#include "barbarous_slice.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/random.h"
#include "character/build.h"
#include "character/creature.h"
#include "test/test.h"
#include "weapon/sword.h"

namespace {
// TODO create a few files with helper classes and factory functions.
class NoOpStance : public Stance {
 public:
  NoOpStance() {}
  void AddModifiers(Creature&) override {}
};
}  // namespace

class BarbarousSliceTest : public GwTest {
 public:
  void SetUp() override {
    character = AddWarriorTo(team());
    character->GetBuild().SetAttribute(Attribute::Swordsmanship, 12);
    barbarous_slice =
        character->GetBuild().AddSkill(std::make_unique<BarbarousSlice>());

    dummy = AddWarriorTo(enemies());

    character->target_ = dummy;
  }

 protected:
  Creature* character;
  Creature* dummy;
  BarbarousSlice* barbarous_slice;
};

TEST_F(BarbarousSliceTest, NoActivationWithoutAdrenaline) {
  ASSERT_FALSE(barbarous_slice->CanActivate(*character, world()));
}

TEST_F(BarbarousSliceTest, NoActivationWithTooLittleAdrenaline) {
  barbarous_slice->AddAdrenaline(5 * Strike);
  ASSERT_FALSE(barbarous_slice->CanActivate(*character, world()));
}

TEST_F(BarbarousSliceTest, ActivationWithEnoughAdrenaline) {
  barbarous_slice->AddAdrenaline(6 * Strike);
  ASSERT_TRUE(barbarous_slice->CanActivate(*character, world()));
}

TEST_F(BarbarousSliceTest, BarbarousSliceInflictsDamage) {
  constexpr int kExpectedSkillDamage = 25;

  barbarous_slice->AddAdrenaline(6 * Strike);
  character->UseSkill(barbarous_slice, world());

  // Override random base attack damage to 0. Then, only skill damage is
  // inflicted.
  OverrideRandomValueForTesting(0);
  // Put myself in a stance to avoid inflicting bleeding.
  character->SetStance(
      Effect<Stance>(10 * Second, std::make_unique<NoOpStance>()));

  AwaitIdle(character);

  ASSERT_EQ(dummy->GetLostHealth(), kExpectedSkillDamage);
}

TEST_F(BarbarousSliceTest, BarbarousSliceInflictsBleedingIfNoStance) {
  Time expected_duration = 13 * Second;
  barbarous_slice->AddAdrenaline(6 * Strike);
  character->UseSkill(barbarous_slice, world());

  while (character->GetActionType() != Action::Type::Idle) {
    Tick();
    if (dummy->HasCondition(Condition::Type::Bleeding)) {
      expected_duration--;
    }
  }
  while (--expected_duration >= Time(0)) {
    ASSERT_TRUE(dummy->HasCondition(Condition::Type::Bleeding));
    Tick();
  }
  ASSERT_FALSE(dummy->HasCondition(Condition::Type::Bleeding));
}

TEST_F(BarbarousSliceTest, BarbarousSliceDoesNotInflictBleedingIfStance) {
  barbarous_slice->AddAdrenaline(6 * Strike);
  character->UseSkill(barbarous_slice, world());

  character->SetStance(
      Effect<Stance>(10 * Second, std::make_unique<NoOpStance>()));

  AwaitIdle(character);

  ASSERT_FALSE(dummy->HasCondition(Condition::Type::Bleeding));
}