#include "approx_transfononpack.hpp"

double approx_transfononpack(const vector<Task>& I, vector<long long>& P, vector<vector<long long> >& allocation, vector<double>& starting_times){
    long long n = I.size(), d = P.size();
    vector<Task> I2;
    vector<long long> P2 ({lcm(P)});
    unify(I, P, I2);

    vector<vector<long long> > allocation2;
    double res = approx_nonpack(I2, P2, allocation2, starting_times);
    allocation = vector<vector<long long> >(n);
    for(long long j = 0; j<n; ++j){
        allocation[j] = vector<long long> (d);
        for(long long i = 0; i<d; ++i){
            allocation[j][i] = (allocation2[j][0]*P[i])/P2[0];
        }
    }
    return res;
}

double approx_transfononpack(const vector<Task>& I, vector<long long>& P){
    vector<vector<long long> > allocation;
    vector<double> starting_times;
    return approx_transfononpack(I, P, allocation, starting_times);
}
