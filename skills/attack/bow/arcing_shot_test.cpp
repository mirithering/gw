#include "arcing_shot.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/clock.h"
#include "base/random.h"
#include "character/stance.h"
#include "weapon/flatbow.h"
#include "weapon/sword.h"

TEST(ArcingShotTest, Test) {
  std::vector<Creature> ranger;
  // 16 marksmanship means damage goes through exactly unaffected by 80 armor.
  // 16 * 5 = 80.
  ranger.push_back(ConstructCreature(Profession::Ranger, Flatbow(),
                                     {{Attribute::Marksmanship, 16}},
                                     ArcingShot()));
  ArcingShot* arcing_shot = ranger[0].GetBuild().GetSkill<ArcingShot>(0);

  std::vector<Creature> target;
  target.push_back(ConstructCreature(Profession::Warrior, Sword()));
  //   Arcing shot is unblockable, prove that by giving the defender a stance
  //   that blocks all attacks.
  target[0].SetStance(
      Effect<Stance>(100 * Second, std::make_unique<BlockEverythingStance>()));

  OverrideRandomValueForTesting((22));
  ranger[0].target_ = &target[0];

  ranger[0].GetAction() = arcing_shot->Activate(ranger[0], ranger, target);

  int until_shot_fired =
      (2025 * Millisecond).value();  // flatbow attack duration
  for (int i = 0; i < until_shot_fired; ++i) {
    EXPECT_EQ(ranger[0].GetAction().GetType(), Action::Type::Busy);
    Tick();
  }
  EXPECT_EQ(ranger[0].GetAction().GetType(), Action::Type::Idle);

  int flight_time = 1320;  // 150% of flatbow flight time
  for (int i = 0; i < flight_time; ++i) {
    EXPECT_EQ(target[0].GetLostHealth(), 0);
    Tick();
  }

  int health_lost =
      22 + 16 +
      10;  // 22 from shot, 16 Marksmanship + 10 additional skill damage
  EXPECT_EQ(target[0].GetLostHealth(), health_lost);
}