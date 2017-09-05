#include "profiles.hpp"


void create_perfect_task(double w, long long P, Task& res){
    multivect<double> execution_time(vector<long long>({P+1}));

    execution_time[vector<long long>({0})] = -1;
    for(vector<long long> p({1}); p[0]<=P; ++p[0])
        execution_time[p] = w/p[0];
    res = Task(execution_time);
}

void create_sequential_task(double w, long long P, Task& res){
    multivect<double> execution_time(vector<long long>({P+1}));

    execution_time[vector<long long>({0})] = -1;
    for(vector<long long> p({1}); p[0]<=P; ++p[0])
        execution_time[p] = w;
    res = Task(execution_time);
}

void create_alpha1_task(double w, long long P, double alpha, Task& res){
    multivect<double> execution_time(vector<long long>({P+1}));

    execution_time[vector<long long>({0})] = -1;
    for(vector<long long> p({1}); p[0]<=P; ++p[0])
        execution_time[p] = w/pow(p[0], alpha);
    res = Task(execution_time);
}

void create_alphaprod_task(double w, const vector<long long>& P, const vector<double>& alpha, Task& res){
    long long d = P.size();
    vector<long long> P2(P);
    for(long long& p:P2){
        ++p;
    }
    multivect<double> execution_time(P2, -1);
    long long i = 0;
    vector<long long> p(d, 1);
    while(i!=d){
        double t = w;
        double div = 1;
        for(long long i = 0; i<d; ++i){
            div *= pow(p[i], alpha[i]);
        }
        t = t/div;
        execution_time[p] = t;
        i = 0;
        while(i<d and p[i] == P[i]){
            p[i] = 1;
            ++i;
        }
        if(i!=d)
            ++p[i];
    }
    res = Task(execution_time);
}

void create_alphamax_task(const vector<double>& W, const vector<long long>& P, const vector<double>& alpha, Task& res){
    long long d = P.size();
    vector<long long> P2(P);
    for(long long& p:P2){
        ++p;
    }
    multivect<double> execution_time(P2, -1);
    long long i = 0;
    vector<long long> p(d, 1);
    while(i!=d){
        double t = W[0]/pow(p[0], alpha[0]);
        for(long long i = 0; i<d; ++i){
            t = max(t, W[i]/pow(p[i], alpha[i]));
        }
        execution_time[p] = t;
        i = 0;
        while(i<d and p[i] == P[i]){
            p[i] = 1;
            ++i;
        }
        if(i!=d)
            ++p[i];
    }
    res = Task(execution_time);
}

void create_amdahlsum_task(const vector<double>& w, const vector<long long>& P, Task& res){
    long long d = P.size();
    if(w.size() != P.size() + 1)
        throw std::runtime_error( "error while creating a task respecting the extended amdhal's law" );
    vector<long long> P2(P);
    for(long long& p:P2){
        ++p;
    }
    multivect<double> execution_time(P2, -1);
    long long i = 0;
    vector<long long> p(d, 1);
    while(i!=d){
        double t = w[0];
        for(long long i = 0; i<d; ++i){
            t += w[i+1]/p[i];
        }
        execution_time[p] = t;
        i = 0;
        while(i<d and p[i] == P[i]){
            p[i] = 1;
            ++i;
        }
        if(i!=d)
            ++p[i];
    }
    res = Task(execution_time);
}

void create_amdahlmax_task(const vector<double>& w, const vector<long long>& P, Task& res){
    long long d = P.size();
    if(w.size() != P.size() + 1)
        throw std::runtime_error( "error while creating a task respecting the extended amdhal's law" );
    vector<long long> P2(P);
    for(long long& p:P2){
        ++p;
    }
    multivect<double> execution_time(P2, -1);
    long long i = 0;
    vector<long long> p(d, 1);
    while(i!=d){
        double t = w[0];
        for(long long i = 0; i<d; ++i){
            t = max(t,w[i+1]/p[i]);
        }
        execution_time[p] = t;
        i = 0;
        while(i<d and p[i] == P[i]){
            p[i] = 1;
            ++i;
        }
        if(i!=d)
            ++p[i];
    }
    res = Task(execution_time);
}

/// For now, b is constant as a function of c
void transform_rooflinemodel_task(double OI, double bs, double bf, long long Dmax, const vector<long long>& P, Task& res){
    vector<long long> P2(P);
    ++P2[0]; ++P2[1];
    multivect<double> execution_time(P2, -1);

    for(long long p_lin = 0; p_lin<P2[0]*P2[1]; ++p_lin){
        long long c = p_lin%P2[0], m = (p_lin/P2[0])%P2[1];
        if(res.time(vector<long long> ({c})) != -1){
            double w = c*res.time(vector<long long> ({c}));
            double b;
            if(m<Dmax){
                b = Dmax/((Dmax-m)/bs + m/bf);
            }
            else{
                b = bf;
            }
            execution_time[vector<long long>({c, m})] = max(w/c, w/(OI*b));
        }
    }
    res = Task(execution_time);
}
