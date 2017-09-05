#include "approx_transfopack.hpp"

double greedypacks(const set<pair<pair<double, long long>, long long> >& L, long long P, vector<double>& starting_times){
    long long remain = 0;
    double res = 0, curr_pack = 0;
    starting_times = vector<double> (L.size());
    for(set<pair<pair<double, long long>, long long> >::reverse_iterator it = L.rbegin(); it!=L.rend(); ++it){
        if(remain<it->first.second){
            curr_pack = res;
            res += it->first.first;
            remain = P-it->first.second;
        }
        else
            remain-=it->first.second;
        starting_times[it->second] = curr_pack;
    }
    return res;
}

double approx1(const vector<Task>& I, long long P, vector<vector<long long> >& allocation, vector<double>& starting_times){
    double res = -1;
    long long n = I.size();
    vector<vector<vector<long long> > > local_minimum(n);

    for(long long j = 0; j<n; ++j){
        double mini = -1;
        for(vector<long long> p({P}); p[0]>=0; --p[0]){
            if(I[j].time(p)!=-1 and (mini==-1 or p[0]*I[j].time(p)<mini)){
                mini = p[0]*I[j].time(p);
                local_minimum[j].emplace_back(p);
            }
        }
    }

    vector<vector<long long> > current_allocation(n);
    vector<double> current_starting_times;
    for(long long j = 0; j<n; ++j)
        current_allocation[j] = local_minimum[j].back();

    set<pair<pair<double, long long>, long long> > L;
    for(long long j = 0; j<n; ++j){
        L.emplace(make_pair(I[j].time(current_allocation[j]), current_allocation[j][0]), j);
        local_minimum[j].pop_back();
    }
    res = greedypacks(L, P, current_starting_times);
    allocation = current_allocation;
    starting_times = current_starting_times;

    while(!local_minimum[L.rbegin()->second].empty()){
        long long j = L.rbegin()->second;
        L.erase(make_pair(make_pair(I[j].time(current_allocation[j]), current_allocation[j][0]), j));
        current_allocation[j]=local_minimum[j].back();
        L.emplace(make_pair(I[j].time(current_allocation[j]), current_allocation[j][0]), j);
        local_minimum[j].pop_back();
        double aux = greedypacks(L, P, current_starting_times);
        if(aux<res){
            res = aux;
            allocation = current_allocation;
            starting_times = current_starting_times;
        }
    }
    return res;
}

double approx_transfopack(const vector<Task>& I, vector<long long>& P, vector<vector<long long> >& allocation, vector<double>& starting_times){
    long long n = I.size(), d = P.size();
    vector<Task> I2;
    vector<long long> P2 ({lcm(P)});
    unify(I, P, I2);

    vector<vector<long long> > allocation2;
    double res = approx_multipack(I2, P2, allocation2, starting_times);
    allocation = vector<vector<long long> >(n);
    for(long long j = 0; j<n; ++j){
        allocation[j] = vector<long long> (d);
        for(long long i = 0; i<d; ++i){
            allocation[j][i] = (allocation2[j][0]*P[i])/P2[0];
        }
    }
    return res;
}

double approx_transfopack(const vector<Task>& I, vector<long long>& P){
    vector<vector<long long> > allocation;
    vector<double> starting_times;
    return approx_transfopack(I, P, allocation, starting_times);
}
