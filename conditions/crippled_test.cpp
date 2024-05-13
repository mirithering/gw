#include "crippled.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/logging.h"
#include "test/test.h"

class CrippledTest : public GwTest {};

TEST_F(CrippledTest, Test) {
  auto creature = AddWarriorTo(team());
  auto target = AddWarriorTo(team());
  target->SetPosition({Inches(1000), Inches(0)});
  creature->WalkTowards(*target, Inches(1));

  Speed speed_non_crippled = creature->GetWalkingSpeed();

  int required_ticks_not_crippled = 0;
  while (creature->GetActionType() != Action::Type::Idle) {
    Tick();
    required_ticks_not_crippled++;
  }

  target->SetPosition({Inches(2000), Inches(0)});
  creature->WalkTowards(*target, Inches(1));
  creature->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<Crippled>()));

  Speed speed_crippled = creature->GetWalkingSpeed();

  int required_ticks_crippled = 0;
  while (creature->GetActionType() != Action::Type::Idle) {
    Tick();
    required_ticks_crippled++;
  }

  ASSERT_DOUBLE_EQ(speed_crippled.value().value(),
                   speed_non_crippled.value().value() / 2);
  ASSERT_NEAR(required_ticks_crippled, required_ticks_not_crippled * 2, 10);
}