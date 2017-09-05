#ifndef APPROX_NONPACK_HPP_INCLUDED
#define APPROX_NONPACK_HPP_INCLUDED

#include <algorithm>
#include <vector>
#include <set>
#include "multivect.hpp"
#include "task.hpp"

using namespace std;

double approx_nonpack(const vector<Task>&, vector<long long>&, vector<vector<long long> >&, vector<double>&);

double approx_nonpack(const vector<Task>&, vector<long long>&);


#endif // APPROX_NONPACK_HPP_INCLUDED
