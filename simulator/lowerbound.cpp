#include "lowerbound.hpp"


double lowerbound(const vector<Task>& I, vector<long long>& P){
    long long n = I.size(), d = P.size();
    double X = -1;
    vector<vector<pair<double, vector<long long> > > > area_allocations(n);
    vector<vector<vector<long long> > > sorted_allocations(n);

    long long prod = 1;
    for(long long p:P)
        prod *= p+1;

    for(long long j = 0; j<n; ++j){
        for(long long p_lin=0; p_lin<prod; ++p_lin){
            vector<long long> p(d);
            long long aux = p_lin;
            for(long long i = 0; i<d; ++i){
                p[i] = aux%(P[i]+1);
                aux /= P[i]+1;
            }
            if(I[j].time(p)!=-1){
                area_allocations[j].emplace_back(-I[j].area(p), p);
            }
        }
        sort(area_allocations[j].begin(), area_allocations[j].end());
        sorted_allocations[j].reserve(area_allocations[j].size());
        for(auto& p:area_allocations[j])
            sorted_allocations[j].emplace_back(p.second);
    }

    vector<vector<long long> > allocation(n);
    for(long long j = 0; j<n; ++j)
        allocation[j] = sorted_allocations[j].back();

    set<pair<pair<double, vector<long long> >, long long> > L;
    double A = 0, t_max = 0;
    for(long long j = 0; j<n; ++j){
        L.emplace(make_pair(I[j].time(allocation[j]), allocation[j]), j);
        A += I[j].area(allocation[j]);
        sorted_allocations[j].pop_back();
    }
    t_max = I[L.rbegin()->second].time(allocation[L.rbegin()->second]);

    X = max(A/d, t_max);
    while(!sorted_allocations[L.rbegin()->second].empty()){
        long long j = L.rbegin()->second;
        double t_prev = L.rbegin()->first.first;
        while(!sorted_allocations[j].empty() and I[j].time(sorted_allocations[j].back())>t_prev){
            sorted_allocations[j].pop_back();
        }
        if(sorted_allocations[j].empty())
            break;

        A = A-I[j].area(allocation[j])+I[j].area(sorted_allocations[j].back());
        if(n>1){
            t_max = max(I[j].time(sorted_allocations[j].back()), next(L.rbegin())->first.first);
        }
        else
            t_max = I[j].time(sorted_allocations[j].back());
        X = min(X, max(A/d, t_max));


        L.erase(make_pair(make_pair(I[j].time(allocation[j]), allocation[j]), j));
        allocation[j]=sorted_allocations[j].back();
        L.emplace(make_pair(I[j].time(allocation[j]), allocation[j]), j);
        sorted_allocations[j].pop_back();
    }
    return X;
}
