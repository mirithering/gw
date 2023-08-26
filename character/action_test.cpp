#include "action.h"

#include <gtest/gtest.h>

#include "base/profession.h"
#include "character.h"
#include "weapon/sword.h"

TEST(ActionTest, IdleEndsAfterOneTick) {
  Action action = kActionIdle;
  EXPECT_EQ(action.Tick(), Action::Result::End);
}

TEST(ActionTest, DeathDoesntEndSoon) {
  Action action = kActionDead;
  EXPECT_EQ(action.Tick(), Action::Result::Continue);
  EXPECT_EQ(action.Tick(), Action::Result::Continue);
  EXPECT_EQ(action.Tick(), Action::Result::Continue);
  EXPECT_EQ(action.Tick(), Action::Result::Continue);
}

TEST(ActionTest, TickFunctionCanEndAction) {
  Action action(
      Action::Type::Busy, 10, [](int duration) { return Action::Result::End; },
      []() {});
  EXPECT_EQ(action.Tick(), Action::Result::End);
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
  EXPECT_EQ(action.Tick(), Action::Result::Continue);
  EXPECT_EQ(tick_called, 1);
  EXPECT_EQ(action.Tick(), Action::Result::Continue);
  EXPECT_EQ(tick_called, 2);
  EXPECT_EQ(action.Tick(), Action::Result::End);
  EXPECT_EQ(tick_called, 3);
  EXPECT_EQ(action.Tick(), Action::Result::End);  // We're done, no more tick.
  EXPECT_EQ(tick_called, 3);
}

TEST(ActionTest, EndFunctionIsCalledOnce) {
  int end_called = 0;
  Action action(
      Action::Type::Busy, 2,
      [](int duration) { return Action::Result::Continue; },
      [&]() { ++end_called; });
  EXPECT_EQ(action.Tick(), Action::Result::Continue);
  EXPECT_EQ(action.Tick(), Action::Result::End);
  EXPECT_EQ(action.Tick(), Action::Result::End);
  EXPECT_EQ(end_called, 1);
}

TEST(ActionTest, EndFunctionIsCalledWithNoDuration) {
  int end_called = 0;
  Action action(
      Action::Type::Busy, 0,
      [](int duration) { return Action::Result::Continue; },
      [&]() { ++end_called; });
  EXPECT_EQ(action.Tick(), Action::Result::End);
  EXPECT_EQ(end_called, 1);
}

TEST(ActionTest, IdleTypeIsIdle) {
  EXPECT_EQ(kActionIdle.GetType(), Action::Type::Idle);
}

TEST(ActionTest, DeadTypeIsDead) {
  EXPECT_EQ(kActionDead.GetType(), Action::Type::Dead);
}

TEST(ActionTest, WeaponAttackTypeIsBusy) {
  Character character(Profession::Warrior);
  character.GiveWeapon(std::make_unique<Sword>());
  Action action = Action::WeaponAttack(character, character);
  EXPECT_EQ(action.GetType(), Action::Type::Busy);
}

TEST(ActionTest, WeaponAttackAttacksAfterHalfOfWeaponSpeed) {
  Character character(Profession::Warrior);
  character.GiveWeapon(std::make_unique<Sword>());
  character.SetAttribute(Attribute::Swordsmanship, 12);

  Action action = Action::WeaponAttack(character, character);

  int expect_attack_at = character.weapon().AttackSpeed() / 2;
  for (int i = 0; i < expect_attack_at; ++i) {
    EXPECT_EQ(action.Tick(), Action::Result::Continue);
    EXPECT_EQ(character.GetLostHealth(), 0);
  }

  EXPECT_EQ(action.Tick(), Action::Result::Continue);
  EXPECT_NE(character.GetLostHealth(), 0);
}

TEST(ActionTest, WeaponAttackLastsForWeaponSpeedTime) {
  Character character(Profession::Warrior);
  character.GiveWeapon(std::make_unique<Sword>());
  character.SetAttribute(Attribute::Swordsmanship, 12);

  Action action = Action::WeaponAttack(character, character);

  int expect_end_at = character.weapon().AttackSpeed();
  for (int i = 0; i < expect_end_at - 1; ++i) {
    EXPECT_EQ(action.Tick(), Action::Result::Continue);
  }
  EXPECT_EQ(action.Tick(), Action::Result::End);
}

// TODO write a random class that can be faked for tests and check the double
// strike chance for assassins.