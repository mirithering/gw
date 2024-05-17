#include "barbarous_slice.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/random.h"
#include "entities/build.h"
#include "entities/creature.h"
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
    creature = AddWarriorTo(team());
    creature->GetBuild().SetAttribute(Attribute::Swordsmanship, 12);
    barbarous_slice =
        creature->GetBuild().AddSkill(std::make_unique<BarbarousSlice>());

    dummy = AddWarriorTo(enemies());

    creature->target_ = dummy;
  }

 protected:
  Creature* creature;
  Creature* dummy;
  BarbarousSlice* barbarous_slice;
};

TEST_F(BarbarousSliceTest, NoActivationWithoutAdrenaline) {
  ASSERT_FALSE(barbarous_slice->CanActivate(*creature, world()));
}

TEST_F(BarbarousSliceTest, NoActivationWithTooLittleAdrenaline) {
  barbarous_slice->AddAdrenaline(5 * Strike);
  ASSERT_FALSE(barbarous_slice->CanActivate(*creature, world()));
}

TEST_F(BarbarousSliceTest, ActivationWithEnoughAdrenaline) {
  barbarous_slice->AddAdrenaline(6 * Strike);
  ASSERT_TRUE(barbarous_slice->CanActivate(*creature, world()));
}

TEST_F(BarbarousSliceTest, BarbarousSliceInflictsDamage) {
  constexpr int kExpectedSkillDamage = 25;

  barbarous_slice->AddAdrenaline(6 * Strike);
  creature->UseSkill(barbarous_slice, world());

  // Override random base attack damage to 0. Then, only skill damage is
  // inflicted.
  OverrideRandomValueForTesting(0);
  // Put myself in a stance to avoid inflicting bleeding.
  creature->SetStance(
      EffectDeprecated<Stance>(10 * Second, std::make_unique<NoOpStance>()));

  AwaitIdle(creature);

  ASSERT_EQ(dummy->GetLostHealth(), kExpectedSkillDamage);
}

TEST_F(BarbarousSliceTest, BarbarousSliceInflictsBleedingIfNoStance) {
  Time expected_duration = 13 * Second;
  barbarous_slice->AddAdrenaline(6 * Strike);
  creature->UseSkill(barbarous_slice, world());

  while (creature->GetActionType() != Action::Type::Idle) {
    Tick();
    if (dummy->HasEffect(Effect::Type::Bleeding)) {
      expected_duration--;
    }
  }
  while (expected_duration-- >= Time(0)) {
    ASSERT_TRUE(dummy->HasEffect(Effect::Type::Bleeding)) << expected_duration;
    Tick();
  }
  ASSERT_FALSE(dummy->HasEffect(Effect::Type::Bleeding));
}

TEST_F(BarbarousSliceTest, BarbarousSliceDoesNotInflictBleedingIfStance) {
  barbarous_slice->AddAdrenaline(6 * Strike);
  creature->UseSkill(barbarous_slice, world());

  creature->SetStance(
      EffectDeprecated<Stance>(10 * Second, std::make_unique<NoOpStance>()));

  AwaitIdle(creature);

  ASSERT_FALSE(dummy->HasEffect(Effect::Type::Bleeding));
}