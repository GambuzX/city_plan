#pragma once

#include "State.h"

State hillClimbing(const State & initialState);
State simulatedAnnealing(const State & initialState);
State tabuSearch(const State & initialState);

State getHigherValueNeighbour(const State & state);
State getHighestValueNeighbour(const State & state);