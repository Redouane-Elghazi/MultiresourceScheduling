#ifndef TRANSFORMATION_HPP_INCLUDED
#define TRANSFORMATION_HPP_INCLUDED

#include <vector>
#include "multivect.hpp"
#include "task.hpp"

//outputs the gcd of 2 numbers a and b
long long gcd(long long, long long);

//outputs the lcm of the elements of a vector of numbers
long long lcm(const vector<long long>&);

//input: a task and the LCM of the dimensions
//output (by ref): the transformed task as defined in the paper
void unify(const Task&, vector<long long>&, long long, Task&);

//input: a vector of tasks of the same instance (same dimensions)
//output (by ref): the vector of the transformed tasks as defined in the paper
void unify(const vector<Task>&, vector<long long>&, vector<Task>&);


#endif // TRANSFORMATION_HPP_INCLUDED
