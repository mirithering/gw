#ifndef BASE_RANDOM_H
#define BASE_RANDOM_H

#include <bits/stdc++.h>

class Chance {
 public:
  explicit Chance(int percentage) {
    assert((percentage >= 0 && percentage <= 100));
    percentage_ = percentage;
  }

  int GetPercentage() { return percentage_; }

 private:
  int percentage_;
};

bool RandomDecision(Chance chance_for_true);

// Returns a random value in [minimum, maximum].
int RandomValue(int minimum, int maximum);

void OverrideRandomRollForTesting(int roll_result);

#endif  // BASE_RANDOM_H