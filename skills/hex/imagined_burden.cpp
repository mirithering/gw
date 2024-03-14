#include "imagined_burden.h"

#include "character/hex.h"
#include "character/skill.h"

namespace {
constexpr int kDuration[] = {8,  9,  10, 10, 11, 12, 13, 14, 14, 15, 16,
                             17, 18, 18, 19, 20, 21, 22, 22, 23, 24, 25};

class ImaginedBurdenHex : public Hex {
 public:
  void AddModifiers(Character& character) override {
    reference_ = character.callbacks_walking_speed_.AddFunction(
        []() { return Percent(-50); });
  }

  Type GetType() const override { return Type::ImaginedBurden; }

 private:
  FunctionList<Percent()>::UniqueReference reference_;
};

}  // namespace

void ImaginedBurden::ActivationEnd(Character& character, World& world) {
  Time time =
      kDuration[character.GetBuild().GetAttribute(Attribute::IllusionMagic)] *
      Second;

  target_->AddHex(Effect<Hex>(time, std::make_unique<ImaginedBurdenHex>()));

  Skill::ActivationEnd(character, world);
}