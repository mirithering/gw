#include "action.h"

#include <gtest/gtest.h>

#include "base/logging.h"
#include "base/profession.h"
#include "character.h"
#include "test/test.h"
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
  std::unique_ptr<Character> character =
      ConstructCharacter(Profession::Warrior, std::make_unique<Sword>());
  Action action = Action::WeaponAttack(*character, *character);
  ASSERT_EQ(action.GetType(), Action::Type::Busy);
}

TEST(ActionTest, MeeleWeaponAttackAttacksAfterHalfOfWeaponSpeed) {
  std::unique_ptr<Character> character =
      ConstructCharacter(Profession::Warrior, std::make_unique<Sword>());

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
  std::unique_ptr<Character> character =
      ConstructCharacter(Profession::Warrior, std::make_unique<Sword>());

  Action action = Action::WeaponAttack(*character, *character);

  Time expect_end_at = character->GetBuild().GetWeapon().AttackDuration();
  for (int i = 0; i < expect_end_at.value() - 1; ++i) {
    ASSERT_EQ(action.Tick(), Action::Result::Continue);
  }
  ASSERT_EQ(action.Tick(), Action::Result::End);
}

TEST(ActionTest, RangedWeaponAttackAttacksAfterAttackDurationPlusFlightTime) {
  std::unique_ptr<Character> character =
      ConstructCharacter(Profession::Ranger, std::make_unique<Flatbow>(),
                         Position({Inches(0), Inches(0)}));
  std::unique_ptr<Character> target =
      ConstructCharacter(Profession::Assassin, std::make_unique<Dagger>(),
                         Position({Inches(1000), Inches(0)}));
  const Weapon& bow = character->GetBuild().GetWeapon();

  character->target_ = target.get();
  character->StartWeaponAttack();

  Time expected_flight_time = Inches(1000) / bow.GetFlightSpeed();

  Time expect_attack_at = bow.AttackDuration() + expected_flight_time;
  LOG << expect_attack_at;
  for (int i = 0; i < expect_attack_at.value(); ++i) {
    ASSERT_EQ(target->GetLostHealth(), 0) << i;
    Tick();
  }
  ASSERT_NE(target->GetLostHealth(), 0);
}

TEST(ActionTest, WalkTowardsEndsAfterOneTickIfAlreadyInRange) {
  std::unique_ptr<Character> character =
      ConstructCharacter(Profession::Ranger, std::make_unique<Flatbow>(),
                         Position({Inches(0), Inches(0)}));
  std::unique_ptr<Character> target =
      ConstructCharacter(Profession::Assassin, std::make_unique<Dagger>(),
                         Position({Inches(1000), Inches(0)}));

  character->WalkTowards(*target, Inches(1000));
  Tick();
  ASSERT_EQ(character->GetActionType(), Action::Type::Idle);
}

TEST(ActionTest, WalkTowardsWalksTowardsUntilTargetRangeReached) {
  std::unique_ptr<Character> character =
      ConstructCharacter(Profession::Ranger, std::make_unique<Flatbow>(),
                         Position({Inches(0), Inches(0)}));
  std::unique_ptr<Character> target =
      ConstructCharacter(Profession::Assassin, std::make_unique<Dagger>(),
                         Position({Inches(1000), Inches(0)}));

  character->WalkTowards(*target, Inches(50));
  AwaitIdle(character.get());
  ASSERT_TRUE(
      InRange(character->GetPosition(), target->GetPosition(), Inches(50)));
}

TEST(ActionTest, WalkTowardsWalksTowardsTargetEvenIfItMovesTowardsMe) {
  std::unique_ptr<Character> character =
      ConstructCharacter(Profession::Ranger, std::make_unique<Flatbow>(),
                         Position({Inches(0), Inches(0)}));
  std::unique_ptr<Character> target =
      ConstructCharacter(Profession::Assassin, std::make_unique<Dagger>(),
                         Position({Inches(1000), Inches(0)}));

  character->WalkTowards(*target, Inches(50));
  target->WalkTowards(*character, Inches(50));
  AwaitIdle(character.get());
  AwaitIdle(target.get());
  ASSERT_TRUE(
      InRange(character->GetPosition(), target->GetPosition(), Inches(50)));
}

TEST(ActionTest, WalkTowardsTargetWalksTowardsTargetEvenIfItMovesAwayFromMe) {
  std::unique_ptr<Character> character =
      ConstructCharacter(Profession::Ranger, std::make_unique<Flatbow>(),
                         Position({Inches(0), Inches(0)}));
  std::unique_ptr<Character> target =
      ConstructCharacter(Profession::Assassin, std::make_unique<Dagger>(),
                         Position({Inches(1000), Inches(0)}));
  std::unique_ptr<Character> target_target =
      ConstructCharacter(Profession::Assassin, std::make_unique<Dagger>(),
                         Position({Inches(2000), Inches(0)}));

  character->WalkTowards(*target, Inches(50));
  target->WalkTowards(*target_target, Inches(50));

  AwaitIdle(character.get());
  AwaitIdle(target.get());

  ASSERT_TRUE(Distance(character->GetPosition(),
                       Position({Inches(1900), Inches(0)})) < Inches(1))
      << Distance(character->GetPosition(),
                  Position({Inches(1900), Inches(0)}));
  ASSERT_TRUE(Distance(target->GetPosition(),
                       Position({Inches(1950), Inches(0)})) < Inches(1))
      << Distance(target->GetPosition(), Position({Inches(1950), Inches(0)}));
}

// TODO double strike chance for daggers.