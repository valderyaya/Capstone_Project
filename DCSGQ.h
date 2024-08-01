#pragma once
#include<map>
#include "Bag.h"
#include "Graph.h"
#include "TreeDecomposition.h"
#include "NiceTreeDecomposition.h"
#include "Graph_CFS.h"
#include "ImproveTreeDecomposition.h"
using namespace std;

template<typename T>
class DCSGQ{
    public:
        const int INF = 2147483647;
        vector<int> UpBound, LowBound, weight;
        NiceTreeDecomposition<T> ntd;
        int H, Initiator, n;
        struct state{
            Bag<T> B;
            set<T> S;
            map<T, T> P;
            state(Bag<T> b, set<T> s, map<T, T> p):B(b),S(s),P(p){}
            bool operator<(const state &d)const{
                if(B != d.B) return B < d.B;
                if(S != d.S) return S < d.S;
                if(P != d.P) return P < d.P;
            }
        };
        map<state, int> W, W_;
        DCSGQ(NiceTreeDecomposition<T> &x):ntd(x){}

        void LEAF_trasfer(Bag<T> b){
            T v = ntd.specialVertex[b];
            W[state(b, set<T>(), map<T,T>())] = 0;
            W_[state(b, set<T>(), map<T,T>())] = 0; 
            if((UpBound[v] >= 0 && 0 >= LowBound[v]) || LowBound[v] == 0){ // case 1
                state sta(b, b.vertices, map<T, T>());
                sta.P[v] = 0;
                W[sta] = weight[v];
                W_[sta] = weight[v];
                // for(int i = LowBound[v]; i <= UpBound[v]; ++i){
                //     sta.P[v] = i;
                //     if(i == 0){
                //         W[sta] = weight[v];
                //         W_[sta] = weight[v];
                //     }else{
                //         W[sta] = -INF;
                //         W_[sta] = -INF;
                //     }
                // }
            }else if(LowBound[v] > 0){ // case 2
                state sta(b, b.vertices, map<T, T>());
                sta.P[v] = 0;
                W_[sta] = weight[v];
                // for(int i = LowBound[v]; i <= UpBound[v]; ++i){
                //     sta.P[v] = i;
                //     if(i == 0){
                //         W[sta] = -INF;
                //         W_[sta] = weight[v];
                //     }else{
                //         W[sta] = -INF;
                //         W_[sta] = -INF;
                //     }
                // }
            }
        }

        void INTRODUCE_transfer(Bag<T> b){
            int v = ntd.specialVertex[b];

        }
};