#include "effect.h"

#include <bits/stdc++.h>

bool IsCondition(Effect::Type type) {
  switch (type) {
    case Effect::Type::Bleeding:
    case Effect::Type::Crippled:
    case Effect::Type::Dazed:
    case Effect::Type::DeepWound:
      return true;
    default:
      return false;
  }
}