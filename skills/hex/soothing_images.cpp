#include "soothing_images.h"

#include "character/hex.h"
#include "character/skill.h"

namespace {
constexpr int kDuration[] = {8,  9,  10, 10, 11, 12, 13, 14, 14, 15, 16,
                             17, 18, 18, 19, 20, 21, 22, 22, 23, 24, 25};

class SoothingImagesHex : public Hex {
 public:
  void AddModifiers(Character& character) override {
    character.callbacks_can_gain_adrenaline_.AddFunction(
        []() { return false; });
  }

  Type GetType() const override { return Type::SoothingImages; }

 private:
  FunctionList<Percent()>::UniqueReference reference_;
};

bool FirstOrSecondProfessionWarrior(const Character& character) {
  if (character.GetBuild().GetFirstProfession() == Profession::Warrior ||
      character.GetBuild().GetSecondProfession() == Profession::Warrior) {
    return true;
  }
  return false;
}

}  // namespace

Character* SoothingImages::GetTarget(Character& character, World& world) const {
  if (FirstOrSecondProfessionWarrior(*character.target_)) {
    return character.target_;
  }

  auto& enemies = world.EnemiesOf(character);
  for (const auto& enemy : enemies) {
    if (FirstOrSecondProfessionWarrior(*enemy)) {
      return enemy.get();
    }
  }
  return nullptr;
}

void SoothingImages::ActivationEnd(Character& character, World& world) {
  Time time =
      kDuration[character.GetBuild().GetAttribute(Attribute::IllusionMagic)] *
      Second;

  auto& enemies = world.EnemiesOf(character);
  for (auto& enemy : enemies) {
    if (InRange(target_->GetPosition(), enemy->GetPosition(), AdjacentRange)) {
      enemy->AddHex(Effect<Hex>(time, std::make_unique<SoothingImagesHex>()));
    }
  }

  Skill::ActivationEnd(character, world);
}

// TODO heroes will use this skill exactly on foes with warrior primary or
// secondary class
