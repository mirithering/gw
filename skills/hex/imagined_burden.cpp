#include "imagined_burden.h"

#include "entities/hex.h"
#include "entities/skill.h"

namespace {
constexpr int kDuration[] = {8,  9,  10, 10, 11, 12, 13, 14, 14, 15, 16,
                             17, 18, 18, 19, 20, 21, 22, 22, 23, 24, 25};

class ImaginedBurdenHex : public Hex {
 public:
  void AddModifiers(Creature& creature) override {
    reference_ = creature.callbacks_walking_speed_.AddFunction(
        []() { return Percent(-50); });
  }

  Type GetType() const override { return Type::ImaginedBurden; }

 private:
  FunctionList<Percent()>::UniqueReference reference_;
};

}  // namespace

void ImaginedBurden::ActivationEnd(Creature& creature, World& world) {
  Time time =
      kDuration[creature.GetBuild().GetAttribute(Attribute::IllusionMagic)] *
      Second;

  target_->AddHex(
      EffectDeprecated<Hex>(time, std::make_unique<ImaginedBurdenHex>()));

  Skill::ActivationEnd(creature, world);
}