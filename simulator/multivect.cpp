#include "multivect.hpp"

/*template<typename T>
multivect<T>::multivect(int n, vector<int>& P, T value): d(n), underlying(vector<multivect<T>*>()), base(value){
    if(n != 0){
        underlying = vector<multivect<T>*> (P[n-1]);
        for(int i = 0; i<P[n-1]; ++i){
            underlying[i] = new multivect(n-1, P, value);
        }
    }
}

template class multivect<double>;*/
