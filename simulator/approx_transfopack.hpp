#ifndef APPROX_TRANSFOPACK_HPP_INCLUDED
#define APPROX_TRANSFOPACK_HPP_INCLUDED

#include <vector>
#include <set>
#include <ctime>
#include "multivect.hpp"
#include "task.hpp"
#include "approx_multipack.hpp"
#include "transformation.hpp"

using namespace std;

//input: a vector of tasks of the same instance (same dimensions)
//output: a 3d-approximation of the optimal co-schedule and (by ref) the packs and allocation of the co-schedule
double approx_transfopack(const vector<Task>&, vector<long long>&, vector<vector<long long> >&, vector<double>&);

//input: a vector of tasks of the same instance (same dimensions)
//output: a 3d-approximation of the optimal co-schedule
double approx_transfopack(const vector<Task>&, vector<long long>&);


#endif // APPROX_HPP_INCLUDED
