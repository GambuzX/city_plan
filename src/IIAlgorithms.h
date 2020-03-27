#pragma once

#include "State.h"

State hillClimbing(const State & initialState);
State simulatedAnnealing(const State & initialState);
State tabuSearch(const State & initialState);

State higherValueNeighbour(const State & state, bool findBest);

State breeding(const State &s1, const State &s2);
std::tuple<std::vector<std::vector<uint>>, std::vector<std::vector<uint>>, uint> divideState(const State &s, const uint &num_rows, const uint &mid_row, const uint &num_cols);

State generateState(InputInfo *global_info);
std::vector<State> generatePopulation(InputInfo *global_info, int populationSize);