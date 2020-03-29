#pragma once

#include "State.h"

#include <unordered_set>

State hillClimbing(const State & initialState);
State simulatedAnnealing(const State & initialState, int maxSteps, int temperature);
State tabuSearch(const State & initialState);

State higherValueNeighbour(const State & state, bool findBest);

State breeding(const State &s1, const State &s2);
std::tuple<std::vector<std::vector<uint>>, std::vector<std::vector<uint>>, uint> divideState(const State &s, const uint &num_rows, const uint &mid_row, const uint &num_cols);

std::unordered_set<State> generate_states(InputInfo *global_info, int num_states);
State generate_state(InputInfo *global_info);