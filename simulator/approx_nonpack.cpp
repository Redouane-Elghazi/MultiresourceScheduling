#include "approx_nonpack.hpp"

extern bool STF, LTF;

long long fittingtask(const vector<vector<long long> >& allocation, set<pair<double, long long> >& remaining_tasks,
                      vector<long long>& remaining_resources, vector<long long>& P){
    for(auto& x:remaining_tasks)
        if(fits(allocation[x.second], remaining_resources))
            return x.second;
    return -1;
}

double rigidschedule(const vector<Task>& I, const vector<vector<long long> >& allocation, vector<long long>& P, vector<double>& starting_times){
    long long d = P.size(), n = I.size();
    vector<long long> remaining_resources(P);
    set<pair<double, long long> > events;
    set<pair<double, long long> > remaining_tasks;
    vector<double> key(n);
    if(STF){
        for(long long j = 0; j<n; ++j){
            remaining_tasks.emplace(I[j].time(allocation[j]), j);
            key[j] = I[j].time(allocation[j]);
        }
    }
    else if(LTF){
        for(long long j = 0; j<n; ++j){
            remaining_tasks.emplace(-I[j].time(allocation[j]), j);
            key[j] = -I[j].time(allocation[j]);
        }
    }
    else{
        for(long long j = 0; j<n; ++j){
            remaining_tasks.emplace(0, j);
            key[j] = 0;
        }
    }
    double res = 0, t = 0;
    starting_times = vector<double> (n);
    while(!remaining_tasks.empty()){
        long long j = fittingtask(allocation, remaining_tasks, remaining_resources, P);
        while(j!=-1){
            starting_times[j] = t;
            events.emplace(t+I[j].time(allocation[j]), j);
            res = max(res, t+I[j].time(allocation[j]));
            for(long long i = 0; i<d; ++i){
                remaining_resources[i]-=allocation[j][i];
            }
            remaining_tasks.erase(make_pair(key[j], j));
            j = fittingtask(allocation, remaining_tasks, remaining_resources, P);
        }
        //nextevent
        for(long long i = 0; i<d; ++i){
            remaining_resources[i]+=allocation[events.begin()->second][i];
        }
        t = events.begin()->first;
        events.erase(events.begin());
    }
    return res;
}

double approx_nonpack(const vector<Task>& I, vector<long long>& P, vector<vector<long long> >& allocation, vector<double>& starting_times){
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
            double obj = 0;
            for(long long i = 0; i<d; ++i){
                p[i] = aux%(P[i]+1);
                obj = obj + log(1+(double)p[i]/(P[i]+1));
                aux /= P[i]+1;
            }
            if(I[j].time(p)!=-1){
                area_allocations[j].emplace_back(-obj*I[j].time(p),p);//-I[j].area(p), p);
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

    set<pair<double, long long> > L;
    for(long long j = 0; j<n; ++j){
        L.emplace(I[j].time(current_allocation[j]), j);
        sorted_allocations[j].pop_back();
    }

    res = rigidschedule(I, current_allocation, P, current_starting_times);
    allocation = current_allocation;
    starting_times = current_starting_times;
    while(!sorted_allocations[L.rbegin()->second].empty()){
        long long j = L.rbegin()->second;
        double t_prev = L.rbegin()->first;
        L.erase(make_pair(I[j].time(current_allocation[j]), j));
        while(!sorted_allocations[j].empty() and I[j].time(sorted_allocations[j].back())>t_prev){
            sorted_allocations[j].pop_back();
        }
        if(sorted_allocations[j].empty())
            break;
        current_allocation[j]=sorted_allocations[j].back();

        L.emplace(I[j].time(current_allocation[j]), j);
        sorted_allocations[j].pop_back();
        double aux = rigidschedule(I, current_allocation, P, current_starting_times);
        if(aux<res){
            res = aux;
            allocation = current_allocation;
            starting_times = current_starting_times;
        }
    }
    return res;
}

double approx_nonpack(const vector<Task>& I, vector<long long>& P){
    vector<vector<long long> > allocation;
    vector<double> starting_times;
    return approx_nonpack(I, P, allocation, starting_times);
}
