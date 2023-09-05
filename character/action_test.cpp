#include "action.h"

#include <gtest/gtest.h>

#include "base/profession.h"
#include "creature.h"
#include "weapon/dagger.h"
#include "weapon/flatbow.h"
#include "weapon/sword.h"

TEST(ActionTest, IdleEndsAfterOneTick) {
  Action action = kActionIdle;
  ASSERT_EQ(action.Tick(), Action::Result::End);
}

TEST(ActionTest, DeathDoesntEndSoon) {
  Action action = kActionDead;
  ASSERT_EQ(action.Tick(), Action::Result::Continue);
  ASSERT_EQ(action.Tick(), Action::Result::Continue);
  ASSERT_EQ(action.Tick(), Action::Result::Continue);
  ASSERT_EQ(action.Tick(), Action::Result::Continue);
}

TEST(ActionTest, TickFunctionCanEndAction) {
  Action action(
      Action::Type::Busy, Second,
      [](Time duration) { return Action::Result::End; }, []() {});
  ASSERT_EQ(action.Tick(), Action::Result::End);
}

TEST(ActionTest, TickFunctionIsCalledTickTimes) {
  int tick_called = 0;
  Action action(
      Action::Type::Busy, 3 * Millisecond,
      [&](Time duration) {
        ++tick_called;
        return Action::Result::Continue;
      },
      []() {});
  ASSERT_EQ(action.Tick(), Action::Result::Continue);
  ASSERT_EQ(tick_called, 1);
  ASSERT_EQ(action.Tick(), Action::Result::Continue);
  ASSERT_EQ(tick_called, 2);
  ASSERT_EQ(action.Tick(), Action::Result::End);
  ASSERT_EQ(tick_called, 3);
  ASSERT_EQ(action.Tick(), Action::Result::End);  // We're done, no more tick.
  ASSERT_EQ(tick_called, 3);
}

TEST(ActionTest, EndFunctionIsCalledOnce) {
  int end_called = 0;
  Action action(
      Action::Type::Busy, 2 * Millisecond,
      [](Time duration) { return Action::Result::Continue; },
      [&]() { ++end_called; });
  ASSERT_EQ(action.Tick(), Action::Result::Continue);
  ASSERT_EQ(action.Tick(), Action::Result::End);
  ASSERT_EQ(action.Tick(), Action::Result::End);
  ASSERT_EQ(end_called, 1);
}

TEST(ActionTest, EndFunctionIsCalledWithNoDuration) {
  int end_called = 0;
  Action action(
      Action::Type::Busy, Time(0),
      [](Time duration) { return Action::Result::Continue; },
      [&]() { ++end_called; });
  ASSERT_EQ(action.Tick(), Action::Result::End);
  ASSERT_EQ(end_called, 1);
}

TEST(ActionTest, IdleTypeIsIdle) {
  ASSERT_EQ(kActionIdle.GetType(), Action::Type::Idle);
}

TEST(ActionTest, DeadTypeIsDead) {
  ASSERT_EQ(kActionDead.GetType(), Action::Type::Dead);
}

TEST(ActionTest, WeaponAttackTypeIsBusy) {
  std::unique_ptr<Creature> character =
      ConstructCreature(Profession::Warrior, std::make_unique<Sword>());
  Action action = Action::WeaponAttack(*character, *character);
  ASSERT_EQ(action.GetType(), Action::Type::Busy);
}

TEST(ActionTest, MeeleWeaponAttackAttacksAfterHalfOfWeaponSpeed) {
  std::unique_ptr<Creature> character =
      ConstructCreature(Profession::Warrior, std::make_unique<Sword>());

  Action action = Action::WeaponAttack(*character, *character);

  Time expect_attack_at =
      character->GetBuild().GetWeapon().AttackDuration() / 2;
  for (int i = 0; i < expect_attack_at.value(); ++i) {
    ASSERT_EQ(character->GetLostHealth(), 0);
    ASSERT_EQ(action.Tick(), Action::Result::Continue);
  }
  ASSERT_NE(character->GetLostHealth(), 0);
}

TEST(ActionTest, WeaponAttackLastsForWeaponSpeedTime) {
  std::unique_ptr<Creature> character =
      ConstructCreature(Profession::Warrior, std::make_unique<Sword>());

  Action action = Action::WeaponAttack(*character, *character);

  Time expect_end_at = character->GetBuild().GetWeapon().AttackDuration();
  for (int i = 0; i < expect_end_at.value() - 1; ++i) {
    ASSERT_EQ(action.Tick(), Action::Result::Continue);
  }
  ASSERT_EQ(action.Tick(), Action::Result::End);
}

TEST(ActionTest, MeeleWeaponAttackAttacksAfterAttackDurationPlusFlightTime) {
  std::unique_ptr<Creature> character =
      ConstructCreature(Profession::Ranger, std::make_unique<Flatbow>());

  character->target_ = character.get();
  character->StartWeaponAttack();

  Time expect_attack_at = character->GetBuild().GetWeapon().AttackDuration() +
                          character->GetBuild().GetWeapon().FlightTime();
  for (int i = 0; i < expect_attack_at.value(); ++i) {
    ASSERT_EQ(character->GetLostHealth(), 0);
    Tick();
  }
  ASSERT_NE(character->GetLostHealth(), 0);
}

// TODO double strike chance for daggers.