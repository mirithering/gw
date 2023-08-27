#include "action.h"

#include <gtest/gtest.h>

#include "base/profession.h"
#include "creature.h"
#include "weapon/dagger.h"
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
      Action::Type::Busy, 10, [](int duration) { return Action::Result::End; },
      []() {});
  ASSERT_EQ(action.Tick(), Action::Result::End);
}

TEST(ActionTest, TickFunctionIsCalledTickTimes) {
  int tick_called = 0;
  Action action(
      Action::Type::Busy, 3,
      [&](int duration) {
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
      Action::Type::Busy, 2,
      [](int duration) { return Action::Result::Continue; },
      [&]() { ++end_called; });
  ASSERT_EQ(action.Tick(), Action::Result::Continue);
  ASSERT_EQ(action.Tick(), Action::Result::End);
  ASSERT_EQ(action.Tick(), Action::Result::End);
  ASSERT_EQ(end_called, 1);
}

TEST(ActionTest, EndFunctionIsCalledWithNoDuration) {
  int end_called = 0;
  Action action(
      Action::Type::Busy, 0,
      [](int duration) { return Action::Result::Continue; },
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
  Creature character = ConstructCreature(Profession::Warrior, Sword());
  Action action = Action::WeaponAttack(character, character);
  ASSERT_EQ(action.GetType(), Action::Type::Busy);
}

TEST(ActionTest, WeaponAttackAttacksAfterHalfOfWeaponSpeed) {
  Creature character = ConstructCreature(Profession::Warrior, Sword());

  Action action = Action::WeaponAttack(character, character);

  int expect_attack_at = character.GetBuild().GetWeapon().AttackSpeed() / 2;
  for (int i = 0; i < expect_attack_at; ++i) {
    ASSERT_EQ(action.Tick(), Action::Result::Continue);
    ASSERT_EQ(character.GetLostHealth(), 0);
  }

  ASSERT_EQ(action.Tick(), Action::Result::Continue);
  ASSERT_NE(character.GetLostHealth(), 0);
}

TEST(ActionTest, WeaponAttackLastsForWeaponSpeedTime) {
  Creature character = ConstructCreature(Profession::Warrior, Sword());

  Action action = Action::WeaponAttack(character, character);

  int expect_end_at = character.GetBuild().GetWeapon().AttackSpeed();
  for (int i = 0; i < expect_end_at - 1; ++i) {
    ASSERT_EQ(action.Tick(), Action::Result::Continue);
  }
  ASSERT_EQ(action.Tick(), Action::Result::End);
}

// TODO double strike chance for daggers.