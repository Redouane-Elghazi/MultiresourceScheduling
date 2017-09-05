#ifndef MULTIVECT_HPP_INCLUDED
#define MULTIVECT_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

template<typename T>
class multivect{

public:
    multivect(): S(vector<long long> (1,0)), underlying(){}

    multivect(const vector<long long>& P, T value): S(P), underlying(){
        long long underlying_size = 1;
        for(long long p:P){
            if(p == 0)
                throw invalid_argument( "the dimensions must be non zero while creating multivect\n\
                                   to create an empty multivect, use the default constructor" );
            underlying_size *= p;
        }
        underlying = vector<T> (underlying_size, value);
    }

    multivect(const vector<long long>& P): S(P), underlying(){
        long long underlying_size = 1;
        for(long long p:P){
            if(p == 0)
                throw invalid_argument( "the dimensions must be non zero while creating multivect\n\
                                   to create an empty multivect, use the default constructor" );
            underlying_size *= p;
        }
        underlying = vector<T> (underlying_size);
    }

    multivect(const multivect<T>& multiv): S(multiv.S), underlying(multiv.underlying){}

    T& operator[](const vector<long long>& P){
        if(P.size() != S.size()){
            throw invalid_argument( "wrong number of dimensions given while accessing multivect" );
        }
        long long ind = 0;
        for(long long i = 0; i<S.size(); ++i){
            if(P[i]>=S[i])
                throw invalid_argument( "index out of range while accessing multivect" );
            else
                ind = ind*S[i]+P[i];
        }
        return underlying[ind];
    }

    /*T& operator[](long long p){
        if(p>=underlying.size()){
            throw invalid_argument( "given index out of bound while accessing multivect" );
        }
        return underlying[p];
    }*/

    const T& operator[](const vector<long long>& P) const{
        if(P.size() != S.size()){
            throw invalid_argument( "wrong number of dimensions given while accessing multivect" );
        }
        long long ind = 0;
        for(long long i = 0; i<S.size(); ++i){
            if(P[i]>=S[i])
                throw invalid_argument( "index out of range while accessing multivect" );
            else
                ind = ind*S[i]+P[i];
        }
        return underlying[ind];
    }


    /*const T& operator[](long long p) const{
        if(p>=underlying.size()){
            throw invalid_argument( "given index out of bound while accessing multivect" );
        }
        return underlying[p];
    }*/

    const vector<long long>& size() const{
        return S;
    }

private:
    vector<long long> S;
    vector<T> underlying;
};
///OLD pas linarise
/*template<typename T>
class multivect{

public:
    multivect(): d(0){}

    multivect(int n, const vector<long long>& P, T value): d(n), s(0), underlying(vector<multivect<T>*>()), base(value){
        if(d>P.size())
            throw invalid_argument( "not enough dimensions given while creating multivect" );
        if(P[d-1] == 0)
            throw invalid_argument( "the dimensions must be non zero while creating multivect" );

        if(d != 0){
            s = P[d-1];
            underlying = vector<multivect<T>*> (P[d-1]);
            for(long long i = 0; i<P[d-1]; ++i){
                underlying[i] = new multivect(d-1, P, value);
            }
        }
    }

    multivect(const multivect<T>& multiv): d(multiv.d), s(multiv.s), underlying(vector<multivect<T>*>()), base(multiv.base){
        if(d != 0){
            underlying = vector<multivect<T>*> (s);
            for(long long i = 0; i<s; ++i){
                underlying[i] = new multivect(*multiv.underlying[i]);
            }
        }
    }

    ~multivect(){
        if(d != 0){
            for(long long i = 0; i<s; ++i){
                delete underlying[i];
            }
        }
    }

    T& operator[](const vector<long long>& P){
        if(d == 0)
            return base;
        else{
            if(d>P.size())
                throw invalid_argument( "not enough dimensions given while accessing multivect" );
            if(P[d-1]>=s)
                throw invalid_argument( "index out of range while accessing multivect" );
            return underlying[P[d-1]]->operator[](P);
        }
    }

    vector<long long> size(){
        vector<long long> res;
        size(res);
        return res;
    }

private:
    int d;
    long long s;
    vector<multivect<T>*> underlying;
    T base;
    void size(vector<long long>& res){
        if(d != 0){
            underlying[0]->size(res);
            res.push_back(s);
        }
    }
};*/


#endif // MULTIVECT_HPP_INCLUDED
