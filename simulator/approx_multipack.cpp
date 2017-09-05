#include "approx_multipack.hpp"

extern bool FFP;

double greedypacks(const set<pair<pair<double, vector<long long> >, long long> >& L, vector<long long>& P, vector<double>& starting_times){
    long long d = P.size();
    double res = 0;
    if(FFP){
        vector<vector<long long> > remain;
        vector<double> t_packs;
        starting_times = vector<double> (L.size());
        for(set<pair<pair<double, vector<long long> >, long long> >::reverse_iterator it = L.rbegin(); it!=L.rend(); ++it){
            long long q = 0, Q = remain.size();
            while(q<Q and !fits(it->first.second,remain[q]))
                ++q;
            if(q==Q){
                t_packs.push_back(res);
                res += it->first.first;
                remain.emplace_back(P);
            }
            for(long long i = 0; i<d; ++i){
                remain[q][i]-=it->first.second[i];
            }
            starting_times[it->second] = t_packs[q];
        }
    }
    else{
        double curr_pack = 0;
        vector<long long> remain(d,-1);
        starting_times = vector<double> (L.size());
        for(set<pair<pair<double, vector<long long> >, long long> >::reverse_iterator it = L.rbegin(); it!=L.rend(); ++it){
            if(!fits(it->first.second,remain)){
                curr_pack = res;
                res += it->first.first;
                remain = P;
            }
            for(long long i = 0; i<d; ++i){
                remain[i]-=it->first.second[i];
            }
            starting_times[it->second] = curr_pack;
        }
    }
    return res;
}

double approx_multipack(const vector<Task>& I, vector<long long>& P, vector<vector<long long> >& allocation, vector<double>& starting_times){
    double res = -1;
    long long n = I.size(), d = P.size();
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

    vector<vector<long long> > current_allocation(n);
    vector<double> current_starting_times;
    for(long long j = 0; j<n; ++j)
        current_allocation[j] = sorted_allocations[j].back();

    set<pair<pair<double, vector<long long> >, long long> > L;
    //double A = 0, t_max = 0;
    for(long long j = 0; j<n; ++j){
        L.emplace(make_pair(I[j].time(current_allocation[j]), current_allocation[j]), j);
        //A += I[j].area(current_allocation[j]);
        sorted_allocations[j].pop_back();
    }
    //t_max = I[L.rbegin()->second].time(current_allocation[L.rbegin()->second]);

    while(!sorted_allocations[L.rbegin()->second].empty()){
        long long j = L.rbegin()->second;
        double t_prev = L.rbegin()->first.first;
        while(!sorted_allocations[j].empty() and I[j].time(sorted_allocations[j].back())>t_prev){
            sorted_allocations[j].pop_back();
        }
        if(sorted_allocations[j].empty())
            break;

        /*A = A-I[j].area(current_allocation[j])+I[j].area(sorted_allocations[j].back());
        if(n>1){
            t_max = max(I[j].time(sorted_allocations[j].back()), next(L.rbegin())->first.first);
        }
        else
            t_max = I[j].time(sorted_allocations[j].back());*/
        double aux = greedypacks(L, P, current_starting_times);
        if(res == -1 or aux<res){
            res = aux;
            allocation = current_allocation;
            starting_times = current_starting_times;
        }

        L.erase(make_pair(make_pair(I[j].time(current_allocation[j]), current_allocation[j]), j));
        current_allocation[j]=sorted_allocations[j].back();
        L.emplace(make_pair(I[j].time(current_allocation[j]), current_allocation[j]), j);
        sorted_allocations[j].pop_back();
    }
    double aux = greedypacks(L, P, current_starting_times);
    if(res == -1 or aux<res){
        res = aux;
        allocation = current_allocation;
        starting_times = current_starting_times;
    }
    return res;
}

double approx_multipack(const vector<Task>& I, vector<long long>& P){
    vector<vector<long long> > allocation;
    vector<double> starting_times;
    return approx_multipack(I, P, allocation, starting_times);
}
