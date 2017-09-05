#include <random>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <exception>

#include "../simulator/task.hpp"
#include "../simulator/profiles.hpp"

using namespace std;



int main(int argc, char* argv[]){
    cout << fixed << setprecision(3);
    if(argc < 2)
        throw runtime_error( "not the right amount of arguments given while using ahmdalmax" );
    long long d = atoi(argv[1]);
    if(argc != 3 + 2*d)
        throw runtime_error( "not the right amount of arguments given while using ahmdalmax" );
    //ofstream out(argv[1], ofstream::app);
    vector<long long> P(d);
    double w;
    vector<double> alpha(d);
    for(long long i = 0; i<d; ++i)
        P[i] = atoll(argv[2+i]);
    w = atof(argv[2+d]);
    for(long long i = 0; i<d; ++i)
        alpha[i] = atof(argv[3+d+i]);
    Task T;
    create_alphaprod_task(w, P, alpha, T);
    vector<long long> p(d, 0);
    long long i = 0;
    while(i!=d){
        cout << T.time(p) << " ";
        i = 0;
        while(i<d and p[i] == P[i]){
            p[i] = 0;
            ++i;
            //out << endl;
        }
        if(i!=d)
            ++p[i];
    }
    cout << endl;
    return 0;
}
