#ifndef TASK_HPP_INCLUDED
#define TASK_HPP_INCLUDED

#include <vector>
#include <stdexcept>
#include "multivect.hpp"
#include "task.hpp"

using namespace std;

class Task{
public:
    Task();
    Task(const multivect<double>&);
    double time(const vector<long long>&) const;
    double area(const vector<long long>&) const;
    const vector<long long>& real_allocation(const vector<long long>& p);
    const vector<long long>& size() const;

private:
    multivect<double> exectime; //the execution time as a function of the resources
    multivect<double> A; //the area as a function of the resources
    multivect<vector<long long> > realalloc;
    vector<long long> P;
};

bool fits(const vector<long long>&, const vector<long long>&);

#endif // TASK_HPP_INCLUDED
