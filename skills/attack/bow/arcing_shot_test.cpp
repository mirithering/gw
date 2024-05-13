#include "arcing_shot.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/clock.h"
#include "base/random.h"
#include "entities/stance.h"
#include "test/test.h"
#include "weapon/flatbow.h"
#include "weapon/sword.h"

class ArcingShotTest : public GwTest {
 public:
  void SetUp() override {
    ranger = AddRangerTo(team());
    // 16 marksmanship means damage goes through exactly unaffected by 80 armor.
    // 16 * 5 = 80.
    ranger->GetBuild().SetAttribute(Attribute::Marksmanship, 16);
    arcing_shot = ranger->GetBuild().AddSkill(std::make_unique<ArcingShot>());

    enemy = AddWarriorTo(enemies());
  }

 protected:
  Creature* ranger;
  ArcingShot* arcing_shot;
  Creature* enemy;
};

TEST_F(ArcingShotTest, Test) {
  //   Arcing shot is unblockable, prove that by giving the defender a stance
  //   that blocks all attacks.
  enemy->SetStance(EffectDeprecated<Stance>(
      100 * Second, std::make_unique<BlockEverythingStance>()));

  OverrideRandomValueForTesting((22));
  ranger->target_ = enemy;

  ranger->UseSkill(arcing_shot, world());

  int until_shot_fired =
      (2025 * Millisecond).value();  // flatbow attack duration
  for (int i = 0; i < until_shot_fired; ++i) {
    EXPECT_EQ(ranger[0].GetActionType(), Action::Type::Busy);
    Tick();
  }
  EXPECT_EQ(ranger[0].GetActionType(), Action::Type::Idle);

  // No distance between the two
  int flight_time = 1;
  for (int i = 0; i < flight_time; ++i) {
    EXPECT_EQ(enemy->GetLostHealth(), 0);
    Tick();
  }

  // 22 from shot, 16 Marksmanship + 10 additional skill damage
  int health_lost = 22 + 16 + 10;
  EXPECT_EQ(enemy->GetLostHealth(), health_lost);
}