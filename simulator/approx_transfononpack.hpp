#ifndef APPROX_TRANSFONONPACK_HPP_INCLUDED
#define APPROX_TRANSFONONPACK_HPP_INCLUDED

#include <vector>
#include <set>
#include "multivect.hpp"
#include "task.hpp"
#include "approx_nonpack.hpp"
#include "transformation.hpp"

using namespace std;

//input: a vector of tasks of the same instance (same dimensions)
//output: a 3d-approximation of the optimal co-schedule and (by ref) the packs and allocation of the co-schedule
double approx_transfononpack(const vector<Task>&, vector<long long>&, vector<vector<long long> >&, vector<double>&);

//input: a vector of tasks of the same instance (same dimensions)
//output: a 3d-approximation of the optimal co-schedule
double approx_transfononpack(const vector<Task>&, vector<long long>&);


#endif // APPROX_TRANSFONONPACK_HPP_INCLUDED
