#include <cmath>
#include <ctime>
#include <random>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include "multivect.hpp"
#include "task.hpp"
#include "lowerbound.hpp"
#include "approx_transfopack.hpp"
#include "approx_transfononpack.hpp"
#include "approx_multipack.hpp"
#include "approx_nonpack.hpp"
#include "profiles.hpp"

using namespace std;

bool LTF, STF, FFP;

void show(vector<Task>& I, vector<vector<long long> >& allocation, vector<double>& starting_times, ostream* out){
    vector<pair<double, long long> > tasks;
    long long n = I.size();
    for(long long j = 0; j<n; ++j){
        tasks.emplace_back(starting_times[j], j);
    }
    sort(tasks.begin(), tasks.end());
    for(long long i = 0; i<n; ++i){
        long long j = tasks[i].second;
        *out << j+1 << " " << starting_times[j] << " ";
        for(long long p:allocation[j])
            *out << p << " ";
        *out << I[j].time(allocation[j]) << endl;
    }
}


int main(int argc, char* argv[]){
    LTF = STF = FFP = false;
    srand(time(NULL));
    long long seed = rand();
    long long n = 10, d = 2, default_P = -1, amplitude_P =-1, algo = 0;
    string input_file = "";
    ostream* out = &cout;
    ofstream streamout;
    bool taskcreator = false, detailed = true;
    for(int i = 1; i<argc; ++i){
        string s = (string)argv[i];
        if(s=="--tasknumber" or s=="-n"){
            ++i;
            n = atoi(argv[i]);
        }
        else if(s=="--resourcenumber" or s=="-d"){
            ++i;
            d = atoi(argv[i]);
        }
        else if(s=="--resourceamount" or s=="-p"){
            ++i;
            default_P = atoi(argv[i]);
        }
        else if(s=="--algorithm" or s=="-a"){
            ++i;
            s = (string)argv[i];
            if(s=="list"){
                algo += 1;
            }
            else if(s=="pack"){
                algo += 2;
            }
            else if(s=="transfolist"){
                algo += 4;
            }
            else if(s=="transfopack"){
                algo += 8;
            }
            else{
                *out << "algorithm " << s << " not recognized" << endl;
                *out << "use \"./simulator --help\" for usage" << endl;
                exit(1);
            }
        }
        else if(s=="--variant" or s=="-v"){
            ++i;
            s = (string)argv[i];
            if(s=="ffp"){
                FFP = true;
            }
            else if(s=="stf"){
                STF = true;
            }
            else if(s=="ltf"){
                LTF = true;
            }
            else{
                *out << "variant " << s << " not recognized" << endl;
                *out << "use \"./simulator --help\" for usage" << endl;
                exit(1);
            }
        }
        else if(s=="--infile" or s=="-i"){
            ++i;
            input_file = argv[i];
        }
        else if(s=="--outfile" or s=="-o"){
            ++i;
            streamout.open(argv[i]);
            out = &streamout;
        }
        else if(s=="--taskcreator" or s=="-t"){
            taskcreator = true;
        }
        else if(s=="--undetailed" or s=="-u"){
            detailed = false;
        }
        else if(s=="--inputformat"){
            *out << "an input file should be as follows:" << endl;
            *out << "\ton a first line, the seed used for the creation of the data" << endl;
            *out << "\ton a second line, n and d" << endl;
            *out << "\ton a third line, P1 ... Pd" << endl;
            *out << "\tthen, using one line per task, all the execution times of the task in increasing lexicographic order of allocation" << endl;
            exit(0);
        }
        else if(s=="--outputformat"){
            *out << "an output file is as follows:" << endl;
            *out << "\ton a first line, the seed used for the execution" << endl;
            *out << "\ton a second line, a lower bound on the optimal makespan" << endl;
            *out << "\ton a third line, the time taken to compute this lower bound" << endl;
            *out << "\tthen, for each algorithm used:" << endl;
            *out << "\t\tspace separated, the makespan of the computed schedule, the initials of the algorithm" << endl;
            *out << "\t\tthe time taken by the algorithm in seconds" << endl;
            *out << "\t\tthen, using on line per task, d+3 space separated values representing in this order:" << endl;
            *out << "\t\t\tthe id of the task, the starting time of the task, the allocation (space separated), and the execution time of the task" << endl;
            exit(0);
        }
        else if(s=="--help" or s=="-h"){
            *out << "usage: ./schedule_sim [-n n] [-d d] [-p p] [-a list|pack|transfolist|transfopack] [-v ffp|stf|ltf] [-i file] [-o file] [-t] [-u] [-h]" << endl;
            *out << "\t--tasknumber or -n       : specifies the number of tasks of the instance" << endl;
            *out << "\t--resourcenumber or -d   : specifies the number of kind of resources" << endl;
            *out << "\t--resourceamount or -p   : specifies the amount of available resource" << endl;
            *out << "\t--algorithm or -a        : specifies an algorithm to use - if no algorithm is given, all will be used" << endl;
            *out << "\t--variant or -v          : specifies a variant to use - if no variant is given, non sorted will be used for list based algorithms, and NFP for pack based" << endl;
            *out << "\t--infile or -i           : specifies an input file (see --inputformat for format), overrides -n -d -p and -t" << endl;
            *out << "\t--outfile or -o          : specifies an output file (see --outputformat for format)" << endl;
            *out << "\t--taskscreator or -t     : if specified, outputs the instance of the problem but doesn't give a solution" << endl;
            *out << "\t--undetailed or -u       : if specified, only the makespan will be prompted, not the associated schedule" << endl;
            *out << "\t--inputformat            : prompts the format of input files" << endl;
            *out << "\t--outputformat           : prompts the format of output files" << endl;
            *out << "\t--help or -h             : prompts this message" << endl;
            exit(0);
        }
        else{
            *out << "argument " << s << " not recognized" << endl;
            *out << "use \"./simulator --help\" for usage" << endl;
            exit(1);
        }
    }
    *out << fixed << setprecision(3);
    //seed = 878775210;
    srand(seed);
    vector<Task> I;
    vector<long long> P;
    if(input_file == ""){
        *out << seed << endl;
        I = vector<Task>(n);
        if(default_P==-1){
            amplitude_P = pow(10000, 1./d);
        }
        //long long C = rand()%300+1;
        //long long M = C;
        P = vector<long long> (d);//({C,M}), P1({C});
        for(int i = 0; i<d; ++i){
            if(default_P == -1)
                P[i] = rand()%amplitude_P+1;
            else
                P[i] = default_P;
        }
        //double bs = 1, bf = 2;
        for(long long j = 0; j<n; ++j){
            vector<double> w(d+1);//alpha(d);//{(double)rand()/RAND_MAX});
            /*for(int i = 0; i<d; ++i){
                alpha[i] = (double)rand()/RAND_MAX;
            }*/
            for(int i = 0; i<d+1; ++i){
                w[i] = rand()%40+1;
            }
            create_amdahlmax_task(w, P, I[j]);
            //create_alpha_task(rand()%40+1, P, alpha, I[j]);
            //transform_rooflinemodel_task((rand()%301 + 50)/(sqrt(50*350)), bs, bf, rand()%M+1, P, I[j]);
        }
        if(taskcreator){
            *out << n << " " << d << endl;
            for(long long p:P)
                *out << p << " ";
            *out << endl;
            for(long long j = 0; j<n; ++j){
                vector<long long> p(d, 0);
                long long i = 0;
                while(i!=d){
                    *out << I[j].time(p) << " ";
                    i = 0;
                    while(i<d and p[i] == P[i]){
                        p[i] = 0;
                        ++i;
                        //*out << endl;
                    }
                    if(i!=d)
                        ++p[i];
                }
                *out << endl;
            }
            return 0;
        }
    }
    else{
        istream* in = &cin;
        ifstream streamin;
        if(input_file!="cin"){
            streamin.open(input_file);
            in = &streamin;
        }
        *in >> seed >> n >> d;
        I = vector<Task> (n);
        P = vector<long long> (d);
        for(long long& p:P)
            *in >> p;
        vector<long long> P2(P);
        for(long long& p:P2){
            ++p;
        }
        for(long long j = 0; j<n; ++j){
            multivect<double> execution_time(P2, -1);
            vector<long long> p(d, 0);
            long long i = 0;
            while(i!=d){
                *in >> execution_time[p];
                i = 0;
                while(i<d and p[i] == P[i]){
                    p[i] = 0;
                    ++i;
                }
                if(i!=d)
                    ++p[i];
            }
            I[j] = Task(execution_time);
        }
    }
    srand(seed);
    vector<vector<long long> > allocation;
    vector<double> starting_times;
    auto t = clock();
    if(algo == 0)
        algo = 15;


    t = clock();
    *out << lowerbound(I, P) << " (lower bound on the optimal solution)" << endl;
    *out << (clock() - t)/((double)CLOCKS_PER_SEC) << endl;

    if(algo%2==1){
        t = clock();
        *out << approx_nonpack(I, P, allocation, starting_times) << " Ld" << endl;
        *out << (clock() - t)/((double)CLOCKS_PER_SEC) << endl;
        if(detailed){
            show(I, allocation, starting_times, out);
        }
    }
    algo /=2;

    if(algo%2==1){
        t = clock();
        *out << approx_multipack(I, P, allocation, starting_times) << " Pd" << endl;
        *out << (clock() - t)/((double)CLOCKS_PER_SEC) << endl;
        if(detailed){
            show(I, allocation, starting_times, out);
        }
    }
    algo /=2;

    if(algo%2==1){
        t = clock();
        *out << approx_transfononpack(I, P, allocation, starting_times) << " Lt" << endl;
        *out << (clock() - t)/((double)CLOCKS_PER_SEC) << endl;
        if(detailed){
            show(I, allocation, starting_times, out);
        }
    }
    algo /=2;

    if(algo%2==1){
        t = clock();
        *out << approx_transfopack(I,P,allocation,starting_times) << " Pt" << endl;
        *out << (clock() - t)/((double)CLOCKS_PER_SEC) << endl;
        if(detailed){
            show(I, allocation, starting_times, out);
        }
    }
    algo/=2;

    return 0;
}
