#include "imagined_burden.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/clock.h"
#include "base/random.h"
#include "entities/creature.h"
#include "test/test.h"
#include "weapon/sword.h"

class ImaginedBurdenTest : public GwTest {
 public:
  void SetUp() override {
    mesmer_ = AddMesmerTo(team());
    imagined_burden_ =
        mesmer_->GetBuild().AddSkill(std::make_unique<ImaginedBurden>());

    enemy_ = AddWarriorTo(enemies());
    mesmer_->target_ = enemy_;
  }

 protected:
  Creature* mesmer_;
  ImaginedBurden* imagined_burden_;

  Creature* enemy_;
};

TEST_F(ImaginedBurdenTest, TargetMoves50PercentSlower) {
  mesmer_->UseSkill(imagined_burden_, world());
  auto normal_walking_speed = enemy_->GetWalkingSpeed();
  AwaitIdle(mesmer_);
  ASSERT_DOUBLE_EQ(enemy_->GetWalkingSpeed().value().value(),
                   normal_walking_speed.value().value() / 2);
}

TEST_F(ImaginedBurdenTest, UnskilledDurationIs8Seconds) {
  mesmer_->UseSkill(imagined_burden_, world());
  AwaitIdle(mesmer_);

  for (int ticks = 0; Time(ticks) < 8 * Second; ++ticks) {
    ASSERT_TRUE(enemy_->IsHexed()) << ticks;
    Tick();
  }
  ASSERT_FALSE(enemy_->IsHexed());
}

TEST_F(ImaginedBurdenTest, EnergyCostIs15) {
  int energy = mesmer_->energy();
  mesmer_->UseSkill(imagined_burden_, world());
  AwaitIdle(mesmer_);
  ASSERT_EQ(mesmer_->energy(), energy - 15);
}
