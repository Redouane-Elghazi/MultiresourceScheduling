#ifndef APPROX_MULTIPACK_HPP_INCLUDED
#define APPROX_MULTIPACK_HPP_INCLUDED

#include <algorithm>
#include <vector>
#include <set>
#include "multivect.hpp"
#include "task.hpp"

using namespace std;

//input: a vector of tasks of the same instance (same dimensions)
//output: a 3d-approximation of the optimal co-schedule and (by ref) the packs and allocation of the co-schedule
double approx_multipack(const vector<Task>&, vector<long long>&, vector<vector<long long> >&, vector<double>&);

//input: a vector of tasks of the same instance (same dimensions)
//output: a 3d-approximation of the optimal co-schedule
double approx_multipack(const vector<Task>&, vector<long long>&);


#endif // APPROX_MULTIPACK_HPP_INCLUDED
