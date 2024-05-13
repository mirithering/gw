#include "soothing_images.h"

#include "entities/hex.h"
#include "entities/skill.h"

namespace {
constexpr int kDuration[] = {8,  9,  10, 10, 11, 12, 13, 14, 14, 15, 16,
                             17, 18, 18, 19, 20, 21, 22, 22, 23, 24, 25};

class SoothingImagesHex : public Hex {
 public:
  void AddModifiers(Creature& creature) override {
    reference_ = creature.callbacks_can_gain_adrenaline_.AddFunction(
        []() { return false; });
  }

  Type GetType() const override { return Type::SoothingImages; }

 private:
  FunctionList<bool()>::UniqueReference reference_;
};

bool FirstOrSecondProfessionWarrior(const Creature& creature) {
  if (creature.GetBuild().GetFirstProfession() == Profession::Warrior ||
      creature.GetBuild().GetSecondProfession() == Profession::Warrior) {
    return true;
  }
  return false;
}

}  // namespace

Creature* SoothingImages::GetTarget(Creature& creature, World& world) const {
  if (FirstOrSecondProfessionWarrior(*creature.target_)) {
    return creature.target_;
  }

  auto& enemies = world.EnemiesOf(creature);
  for (const auto& enemy : enemies) {
    if (FirstOrSecondProfessionWarrior(*enemy)) {
      return enemy.get();
    }
  }
  return nullptr;
}

void SoothingImages::ActivationEnd(Creature& creature, World& world) {
  Time time =
      kDuration[creature.GetBuild().GetAttribute(Attribute::IllusionMagic)] *
      Second;

  auto& enemies = world.EnemiesOf(creature);
  for (auto& enemy : enemies) {
    if (InRange(target_->GetPosition(), enemy->GetPosition(), AdjacentRange)) {
      enemy->AddHex(
          EffectDeprecated<Hex>(time, std::make_unique<SoothingImagesHex>()));
    }
  }

  Skill::ActivationEnd(creature, world);
}

// TODO heroes will use this skill exactly on foes with warrior primary or
// secondary class
