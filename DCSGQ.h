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
        map<pair<int, int>, int> edge_weight; 
        NiceTreeDecomposition<T> ntd;
        int H, Initiator, n, max_dg;
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

        void initialization(){
            max_dg = 0;
            for(auto &it : ntd.treeDecomposition.graph.adj)
                max_dg = max(max_dg, it->second.size());
        }

        bool is_in_range(int v, int w){
            return (LowBound[v] <= w && w <= UpBound[v]);
        }

        vector<set<int>> get_subset(set<int> s){
            vector<set<int>> ret;
            if(s.size() == 2){
                int t[2] = {*s.begin(), *s.rbegin()};
                ret.emplace_back(set<int>({t[0]}));
                ret.emplace_back(set<int>({t[1]}));
                ret.emplace_back(set<int>({t[0], t[1]}));
            }else if(s.size() == 3){    
                int t[3], i = 0;
                for(auto it=s.begin(); it != s.end(); ++it, ++i) t[i] = *it;
                ret.emplace_back(set<int>({t[0]}));
                ret.emplace_back(set<int>({t[1]}));
                ret.emplace_back(set<int>({t[2]}));
                ret.emplace_back(set<int>({t[0], t[1]}));
                ret.emplace_back(set<int>({t[0], t[2]}));
                ret.emplace_back(set<int>({t[1], t[2]}));
                ret.emplace_back(set<int>({t[0], t[1], t[3]}));
            }
            return ret;
        }

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

        void INTRODUCE_transfer(Bag<T> bx, Bag<T> by){ // missing p_x = 0 cases ?
            int v = ntd.specialVertex[bx];
            W[state(bx, set<T>(), map<T,T>())] = 0;
            W_[state(bx, set<T>(), map<T,T>())] = 0; 
            vector<set<int>> ss = get_subset(bx.vertices);
            for(auto &s : ss){
                if(s.count(v)){
                    int f = 0;
                    for(auto it = s.begin(); it != s.end(); ++it)
                        if(*it != v) f += edge_weight[{v, *it}];  // calculate condition 8
                    
                    state prv = state(by, s, map<T,T>()), now = state(bx, s, map<T,T>());
                    if(s.size() == 1){
                        for(int o = *s.begin(), i = 0; i <= max_dg; ++i){
                            prv.P[o] = now.P[o] = i;
                            if(f != i) continue;
                            else if(f == i && is_in_range(o)){
                                
                            }else if(f == i && !is_in_range(o)){

                            }
                        }
                    }else if(s.size() == 2){

                    }else if(s.size() == 3){

                    }

                }else{
                    state prv = state(by, s, map<T,T>()), now = state(bx, s, map<T,T>());
                    if(s.size() == 1){
                        for(int o = *s.begin(), i = 0; i <= max_dg; ++i){
                            prv.P[o] = now.P[o] = i;
                            if(W.count(prv)) W[now] = W[prv];
                            if(W_.count(prv)) W_[now] = W_[prv];
                        }
                    }else if(s.size() == 2){
                        int a = *s.begin(), b = *s.rbegin();
                        for(int i = 0; i <= max_dg; ++i)
                            for(int j = 0; j <= max_dg; ++j){
                                prv.P[a] = i, prv.P[b] = j;
                                now.P[a] = i, now.P[b] = j;
                                if(W.count(prv)) W[now] = W[prv];
                                if(W_.count(prv)) W_[now] = W_[prv];
                            }
                    }else if(s.size() == 3){
                        int t[3], ind=0;
                        for(auto it = s.begin(); it != s.end(); ++it) t[ind++] = *it;
                        for(int i = 0; i <= max_dg; ++i)
                            for(int j = 0; j <= max_dg; ++j)
                                for(int k = 0; k <= max_dg; ++k){
                                    prv.P[t[0]] = i, prv.P[t[1]] = j, prv.P[t[2]] = k;
                                    now.P[t[0]] = i, now.P[t[1]] = j, now.P[t[2]] = k;
                                    if(W.count(prv)) W[now] = W[prv];
                                    if(W_.count(prv)) W_[now] = W_[prv];
                                }
                    }
                }
            }
        }
};