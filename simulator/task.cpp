#include "task.hpp"

Task::Task(): exectime(), P(){}

Task::Task(const multivect<double>& etime): exectime(etime), A(etime), realalloc(etime.size()), P(etime.size()){
    long long P_lin = 1;
    int d = etime.size().size();
    for(long long& p:P){
        P_lin*=p;
        --p;
    }
    vector<long long> p(d, 0), p2(d);
    realalloc[p] = p;
    for(long long p_lin = 1; p_lin<P_lin; ++p_lin){
        long long aux = p_lin;
        double p_sum = 0;
        for(int i = 0; i<d; ++i){
            p[i] = aux%(P[i]+1);
            p2[i] = aux%(P[i]+1);
            aux /= P[i]+1;
            p_sum+=(double)p[i]/P[i];
        }
        realalloc[p] = p;
        for(int i = 0; i<d; ++i){
            if(p[i]!=0){
                --p2[i];
                if(exectime[p2] != -1 and (exectime[p] == -1 or exectime[p2] < exectime[p])){
                    realalloc[p] = realalloc[p2];
                    exectime[p] = exectime[p2];
                }
                ++p2[i];
            }
        }
        if(exectime[p] != -1)
            A[p] = p_sum*exectime[p];
    }
}

double Task::time(const vector<long long>& P) const{
    try{
        return exectime[P];
    } catch (...) {
        throw std::runtime_error( "error while trying to access the execution time of a task" );
    }
}

double Task::area(const vector<long long>& P) const{
    try{
        return A[P];
    } catch (...) {
        throw std::runtime_error( "error while trying to access the area of a task" );
    }
}

const vector<long long>& Task::size() const{
    return P;
}

bool fits(const vector<long long>& task_resources, const vector<long long>& available_resources){
    long long d = task_resources.size();
    for(long long i = 0; i<d; ++i){
        if(available_resources[i]<task_resources[i])
            return false;
    }
    return true;
}
