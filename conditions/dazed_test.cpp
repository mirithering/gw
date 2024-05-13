#include "dazed.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/logging.h"
#include "skills/hex/imagined_burden.h"
#include "test/test.h"

class DazedTest : public GwTest {};

TEST_F(DazedTest, DazedDoublesSpellCastTime) {
  auto mesmer = AddMesmerTo(team());
  auto imagined_burden_ =
      mesmer->GetBuild().AddSkill(std::make_unique<ImaginedBurden>());

  auto enemy = AddWarriorTo(enemies());
  mesmer->target_ = enemy;

  int time_without_dazed = 0;
  mesmer->UseSkill(imagined_burden_, world());
  while (mesmer->GetActionType() != Action::Type::Idle) {
    Tick();
    ++time_without_dazed;
  }

  LOG << "time without dazed: " << time_without_dazed;

  while (!imagined_burden_->CanActivate(*mesmer, world())) {
    Tick();
  }

  mesmer->AddCondition(
      EffectDeprecated<Condition>(10 * Second, std::make_unique<Dazed>()));
  ASSERT_TRUE(mesmer->HasCondition(Condition::Type::Dazed));

  int time_with_dazed = 0;
  mesmer->UseSkill(imagined_burden_, world());
  while (mesmer->GetActionType() != Action::Type::Idle) {
    Tick();
    ++time_with_dazed;
  }

  ASSERT_EQ(time_with_dazed, 2 * time_without_dazed);
}

// TODO check that dazed makes easily interruptable.
