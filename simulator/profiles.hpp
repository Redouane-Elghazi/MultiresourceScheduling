#ifndef PROFILES_HPP_INCLUDED
#define PROFILES_HPP_INCLUDED

#include <cmath>
#include <vector>
#include <stdexcept>
#include "multivect.hpp"
#include "task.hpp"

using namespace std;

void create_perfect_task(double, long long, Task&);

void create_sequential_task(double, long long, Task&);

void create_alpha1_task(double, long long, double, Task&);

void create_alphaprod_task(double, const vector<long long>&, const vector<double>&, Task&);

void create_alphamax_task(const vector<double>&, const vector<long long>&, const vector<double>&, Task&);

void create_amdahlsum_task(const vector<double>&, const vector<long long>&, Task&);

void create_amdahlmax_task(const vector<double>&, const vector<long long>&, Task&);

void transform_rooflinemodel_task(double, double, double, long long, const vector<long long>&, Task&);

#endif // PROFILES_HPP_INCLUDED
