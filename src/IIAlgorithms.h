#pragma once

#include "State.h"

enum OperatorsAccuracy {
    LOW,
    MEDIUM,
    HIGH
};

State hillClimbing(InputInfo * info, int steps = 100, OperatorsAccuracy accuracy = OperatorsAccuracy::LOW);
State simulatedAnnealing(InputInfo * info, int maxSteps = 100, double maxTemperature = 10);
State tabuSearch(const State & initialState);

State higherValueNeighbour(const State & state, OperatorsAccuracy accuracy = OperatorsAccuracy::LOW);