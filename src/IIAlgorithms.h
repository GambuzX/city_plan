#pragma once

#include "State.h"

enum OperatorsAccuracy {
    LOW,
    MEDIUM,
    HIGH
};

State hillClimbing(InputInfo * info, int steps = 100, OperatorsAccuracy accuracy = OperatorsAccuracy::LOW);
State simulatedAnnealing(InputInfo * info, int maxSteps = 100, double maxTemperature = 10);
State tabuSearch(InputInfo * info, int maxSteps = 100, double maxTemperature = 10, int maxTabus = 2);

State higherValueNeighbour(const State & state, OperatorsAccuracy accuracy = OperatorsAccuracy::LOW);