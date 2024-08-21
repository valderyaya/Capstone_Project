#pragma once
#include<map>
#include "Bag.h"
#include "Graph.h"
#include<stack>
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
            int sz = s.size();
            vector<int> t(sz);
            int i = 0;
            for(auto it = s.begin(); it != s.end(); ++it, ++i)
                t[i] = *it;
            
            for(int i = 1; i < (1 << sz); ++i){
                set<int> tmp;
                for(int j = 0; j < sz; ++j)
                    if((i >> j) & 1) tmp.insert(t[j]);
                ret.emplace_back(tmp); 
            }
            return ret;
        }

        void LEAF_trasfer(Bag<T> b){
            T v = ntd.specialVertex[b];
            // W[state(b, set<T>(), map<T,T>())] = 0;
            // W_[state(b, set<T>(), map<T,T>())] = 0; 
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

        void Enumerate(){
            int N = s.size(), ind = 0, u_idx = -1;
            vector<int> v(N, -1), elm(N);
            for(auto &i: s){
                if(i == u) u_idx = ind;
                elm[ind++] = i;
            }
            state now = state(bx, s, map<T,T>());
            stack<int> st;
            st.push(0);

            while(!st.empty()){
                ind = st.top();
                if(ind == (int)v.size()){
                    st.pop();
                    // transform

                    continue;
                }
                if(v[ind] > max_dg){
                    st.pop();
                    v[ind] = 0;
                    continue;
                }
                ++v[ind];
                st.emplace(ind + 1);
            }
        }

        void INTRODUCE_transfer(Bag<T> bx, Bag<T> by){ // missing p_x = 0 cases ?
            int u = ntd.specialVertex[bx];
            // W[state(bx, set<T>(), map<T,T>())] = 0;
            // W_[state(bx, set<T>(), map<T,T>())] = 0; 
            vector<set<int>> ss = get_subset(bx.vertices);
            for(auto &s : ss){
                int N = s.size(), ind = 0, u_idx = -1;
                vector<int> v(N, -1), elm(N);
                for(auto &i: s){
                    if(i == u) u_idx = ind;
                    elm[ind++] = i;
                }
                state now = state(bx, s, map<T,T>());

                stack<int> st;
                st.push(0);


                while(!st.empty()){ // enumerate
                    ind = st.top();
                    if(ind == (int)v.size()){// transform
                        st.pop();
                        for(int i = 0; i < ind; ++i) now.P[elm[i]] = v[i];
                        
                        if(s.count(u)){
                            int f = 0;
                            for(auto it = s.begin(); it != s.end(); ++it) f += edge_weight[{v, *it}]; // calculate condition 8
                            if(f != i) continue;

                            set<int> sy(s);
                            sy.erase(u);
                            state prv = state(by, sy, map<T,T>());
                            for(int i = 0; i < ind; ++i) prv.P[elm[i]] = v[i];
                            
                            int tmp = v[u_idx];
                            for(auto &i : sy) tmp -= edge_weight[{u, i}];
                            prv.P[u] = tmp; // P_sy

                            if(f == i && is_in_range(u, v[u_idx])){
                                if(W_.count(prv)){
                                    W[now] = W_[prv] + weight[u];
                                    W_[now] = W_[prv] + weight[u];
                                }
                            }else if(f == i && !is_in_range(u, v[u_idx]))
                                if(W_.count(prv)) W_[now] = W_[prv] + weight[u];

                        }else{ // case 1
                            state prv = state(by, s, map<T,T>());
                            for(int i = 0; i < ind; ++i) prv.P[elm[i]] = v[i];
                            if(W.count(prv)) W[now] = W[prv];
                            if(W_.count(prv)) W_[now] = W_[prv];
                        }
                        continue;
                    }
                    if(v[ind] > max_dg){
                        st.pop();
                        v[ind] = 0;
                        continue;
                    }
                    ++v[ind];
                    st.emplace(ind + 1);
                }
                
            }
        }

        void FORGET_transfer(Bag<T> bx, Bag<T> by){
            int u = ntd.specialVertex[bx];
            // W[state(bx, set<T>(), map<T,T>())] = 0;
            // W_[state(bx, set<T>(), map<T,T>())] = 0; 
            vector<set<int>> ss = get_subset(bx.vertices);
            for(auto &s : ss){
                int N = s.size(), ind = 0, u_idx = -1;
                vector<int> v(N, -1), elm(N);
                for(auto &i: s){
                    if(i == u) u_idx = ind;
                    elm[ind++] = i;
                }
                state now = state(bx, s, map<T,T>());
                stack<int> st;
                st.push(0);

                while(!st.empty()){
                    ind = st.top();
                    if(ind == (int)v.size()){
                        st.pop();
                        // transform

                        continue;
                    }
                    if(v[ind] > max_dg){
                        st.pop();
                        v[ind] = 0;
                        continue;
                    }
                    ++v[ind];
                    st.emplace(ind + 1);
                }
            }
            
            // ignore
            for(auto &s : ss){
                state now = state(bx, s, map<T,T>());
                if(s.size() == 1){
                    for(int i = 0; i <= max_dg; ++i){
                        now.P[*s.begin()] = i;
                        
                        int mx = -INF, mx_ = -INF;
                        set<int> sy(s);
                        sy.insert(v);
                        state prv = state(by, sy, map<T,T>());

                        for(int j = 0; j <= max_dg; ++j)
                            for(int k = LowBound[v]; k <= UpBound[v]; ++k){
                                prv.P[*s.begin()] = j;
                                prv.P[v] = k;
                                if(W.count(prv)) mx = max(mx, W[prv]);
                                if(W_.count(prv)) mx_ = max(mx_, W_[prv]);
                            }
                        
                        prv = state(by, s, now.P);
                        if(W.count(prv)) mx = max(mx, W[prv]);
                        if(W_.count(prv)) mx_ = max(mx_, W_[prv]);

                        W[now] = mx;
                        W_[now] = mx_;
                    }
                }else if(s.size() == 2){ // need optimize
                    int a = *s.begin(), b = *s.rbegin();
                    if(a != v) swap(a, b);
                    for(int i = 0; i <= max_dg; ++i)
                        for(int j = 0; j <= max_dg; ++j){
                            now.P[a] = i, now.P[b] = j;

                            int mx = -INF, mx_ = -INF;
                            set<int> sy(s);
                            sy.insert(v);
                            state prv = state(by, sy, map<T,T>());

                            for(int ii = 0; ii <= max_dg; ++ii)
                                for(int jj = 0; jj <= max_dg; ++jj)
                                    for(int k = LowBound[v]; k <= UpBound[v]; ++k){
                                        prv.P[a] = i, prv.P[b] = j, prv.P[v] = k;
                                        if(W.count(prv)) mx = max(mx, W[prv]);
                                        if(W_.count(prv)) mx_ = max(mx_, W_[prv]);
                                    }

                            prv = state(by, s, now.P);
                            if(W.count(prv)) mx = max(mx, W[prv]);
                            if(W_.count(prv)) mx_ = max(mx_, W_[prv]);

                            if(mx != -INF) W[now] = mx;
                            if(mx_ != -INF) W_[now] = mx_;
                        }
                } //else if(s.size() == 3){}
            }
        }
 
        void JOIN_transfer(Bag<T> bx, Bag<T> by, Bag<T> bz){
            W[state(bx, set<T>(), map<T,T>())] = 0;
            W_[state(bx, set<T>(), map<T,T>())] = 0; 
            vector<set<int>> ss = get_subset(bx.vertices);
            for(auto &s : ss){
                state now = state(bx, s, map<T,T>());
                if(s.size() == 1){
                    for(int i = 0; i <= max_dg; ++i){
                        state py = state(by, s, map<T,T>());
                        py.P[*s.begin()] = i;
                        for(int j = 0; j <= max_dg; ++j){
                            if(i + j > max_dg) continue;
                            state pz = state(bz, s, map<T,T>());
                            pz.P[*s.begin()] = j;
                            
                            if(W_.count(py) && W_.count(pz)){
                                now.P[*s.begin()] = i + j;
                                W_[now] = max(W_[now], W_[py] + W_[pz] - weight[*s.begin()]);
                                if(is_in_range(*s.begin(), i + j))
                                    W[now] = max(W[now], W_[py] + W_[pz] - weight[*s.begin()]);
                            }
                        }
                    }
                }else if(s.size() == 2){
                    int a = *s.begin(), b = *s.rbegin();
                    for(int i = 0; i <= max_dg; ++i)
                        for(int j = 0; j <= max_dg; ++j){
                            state py = state(by, s, map<T,T>());
                            py.P[a] = i, py.P[b] = j;

                            for(int k = 0; k <= max_dg; ++k){
                                if(i + k > max_dg) continue;
                                for(int l = 0; l <= max_dg; ++l){
                                    if(j + l > max_dg) continue;
                                    state pz = state(bz, s, map<T,T>());
                                    pz.P[a] = k, pz.P[b] = l;

                                    if(W_.count(py) && W_.count(pz)){
                                        now.P[a] = i + k, now.P[b] = j + l;
                                        W_[now] = max(W_[now], W_[py] + W_[pz] - weight[a] - weight[b]);
                                        if(is_in_range(a, i + k) && is_in_range(b, j + l))
                                            W[now] = max(W[now], W_[py] + W_[pz] - weight[a] - weight[b]);
                                    }
                                }
                            }
                        }
                }else if(s.size() == 3){
                    int t[3], ind=0;
                    for(auto it = s.begin(); it != s.end(); ++it) t[ind++] = *it;
                    for(int i = 0; i <= max_dg; ++i)
                        for(int j = 0; j <= max_dg; ++j)
                            for(int k = 0; k <= max_dg; ++k){
                                state py = state(by, s, map<T,T>());
                                py.P[t[0]] = i, py.P[t[1]] = j, py.P[t[2]] = k;

                                for(int ii = 0; ii <= max_dg; ++ii){
                                    if(i + ii > max_dg) continue;
                                    for(int jj = 0; jj <= max_dg; ++jj){
                                        if(j + jj > max_dg) continue;
                                        for(int kk = 0; kk <= max_dg; ++kk){
                                            if(k + kk > max_dg) continue;
                                            state pz = state(bz, s, map<T,T>());
                                            pz.P[t[0]] = ii, pz.P[t[1]] = jj, pz.P[t[2]] = kk;

                                            if(W_.count(py) && W_.count(pz)){
                                                now.P[t[0]] = i + ii;
                                                now.P[t[1]] = j + jj;
                                                now.P[t[2]] = k + kk;
                                                W_[now] = max(W_[now], W_[py] + W_[pz] - weight[t[0]] - weight[t[1]] - weight[t[2]]);
                                                if(is_in_range(t[0], i + ii) && is_in_range(t[1], j + jj) && is_in_range(t[2], k + kk))
                                                    W[now] = max(W[now], W_[py] + W_[pz] - weight[t[0]] - weight[t[1]] - weight[t[2]]);
                                            }
                                        }
                                    }
                                }
                            }
                }
            }
        }
};