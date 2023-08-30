#ifndef BASE_RANDOM_H
#define BASE_RANDOM_H

#include <bits/stdc++.h>

#include "base/units.h"

bool RandomDecision(Percent chance_for_true);

// Returns a random value in [minimum, maximum].
int RandomValue(int minimum, int maximum);

void OverrideRandomValueForTesting(int roll_result);

void OverrideRandomDecisionForTesting(
    Percent everythinig_greater_than_this_will_be_true);

#endif  // BASE_RANDOM_H