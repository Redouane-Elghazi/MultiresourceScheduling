#include "transformation.hpp"

long long gcd(long long a, long long b){
    if(a==0)
        return b;
    else
        return gcd(b%a, a);
}

long long lcm(const vector<long long>& numbers){
    long long res = 1;
    for(long long x:numbers)
        res = (res*x)/gcd(res, x);
    return res;
}

void unify(const Task& t, vector<long long>& P, long long P2, Task& res){
    vector<long long> p_old(P.size());
    multivect<double> execution_time(vector<long long>({P2+1}));
    for(vector<long long> p_new(1,0); p_new[0]<=P2; ++p_new[0]){
        for(unsigned long long i = 0; i<P.size(); ++i){
            p_old[i] = (p_new[0]*P[i])/P2;
        }
        execution_time[p_new] = t.time(p_old);
    }
    res = Task(execution_time);
}

void unify(const vector<Task>& in, vector<long long>& P, vector<Task>& res){
    long long n = in.size();
    if(n == 0)
        return;
    int P2 = lcm(P);
    res = vector<Task> (n);
    for(long long j = 0; j<n; ++j){
        unify(in[j], P, P2, res[j]);
    }
}
