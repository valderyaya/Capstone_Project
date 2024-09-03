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
        struct state{
            Bag<T> B;
            set<T> S;
            set<T> P;
            state() : B(), S(), P() {}
            state(Bag<T> b, set<T> s, map<T, T> p):B(b),S(s),P(p){}
            bool operator<(const state &d)const{
                if(B != d.B) return B < d.B;
                if(S != d.S) return S < d.S;
                return P < d.P;
            }
        };

        const int INF = 2147483647;
        vector<int> UpBound, LowBound, weight;
        map<pair<int, int>, int> edge_weight; 
        map<pair<int, int>, int> edge_id;
        vector<pair<int,int>> edge;
        NiceTreeDecomposition<T> ntd;
        map<state, vector<state>> from, from_;
        map<state, int> fromTag;
        map<state, int> W, W_;
        map<Bag<T>, state> max_state;
        map<Bag<T>, int> max_value;
        state max_root_state;
        int H, Initiator, n;
        
        
        DCSGQ(NiceTreeDecomposition<T> &x):ntd(x){}
        DCSGQ(){}
        

        // void initialization(){}

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
            sort(ret.begin(), ret.end(), [&](const set<int>& a, const set<int>& b){return a.size() != b.size() ? a.size() < b.size() : a < b;});
            return ret;
        }

        void LEAF_trasfer(Bag<T> b){
            T v = ntd.specialVertex[b];
            W[state(b, set<T>(), set<T>())] = 0;
            W_[state(b, set<T>(), set<T>())] = 0; 
            if((UpBound[v] >= 0 && 0 >= LowBound[v]) || LowBound[v] == 0){ // case 1
                state sta(b, b.vertices, set<T>());
                W[sta] = weight[v];
                W_[sta] = weight[v];
                max_state[b] = sta;
                max_value[b] = weight[v];
            }else if(LowBound[v] > 0){ // case 2
                state sta(b, b.vertices, set<T>());
                W_[sta] = weight[v];
            }
        }


        void INTRODUCE_transfer(Bag<T> bx, Bag<T> by){ // missing p_x = 0 cases ?
            int u = ntd.specialVertex[bx];
            W[state(bx, set<T>(), map<T,T>())] = 0;
            W_[state(bx, set<T>(), map<T,T>())] = 0; 
            vector<set<int>> ss = get_subset(bx.vertices);
            for(auto &s : ss){
                int N = s.size(), ind = 0, u_idx = -1, tmp_sum = 0;
                vector<int> v, elm(N);
                for(auto &i: s){
                    if(i == u) u_idx = ind;
                    elm[ind++] = i;
                    tmp_sum += weight[i];
                }
                state now = state(bx, s, set<T>());
                W_[now] = tmp_sum;
                
                for(int i = 0; i < ind; ++i)
                    for(int j = i + 1; j < ind; ++j)
                        if(edge_id.count({i, j}))
                            v.emplace_back(edge_id[{i, j}]);
                
                int M = v.size();
                for(int mask = 0; mask < (1 << M); ++mask){
                    set<int> edge_set;
                    int f = 0;
                    map<int, int> P;
                    for(int i = mask, j; i; i = (i - 1) & i){
                        j = v[__lg((i & (-i)))];
                        edge_set.insert(j);
                        ++P[edge[j][0]];
                        ++P[edge[j][1]];
                        if(edge[j][0] == u || edge[j][1] == u){
                            f += edge_weight[edge[j]];
                        }
                    }
                    if(!s.count(u)){// u not in s
                        state prv = state(by, s, edge);
                        if(W.count(prv)){
                            int val = W[prv];
                            W[now] = val;
                            from[now] = vector<state>{prv};
                            if(val > max_value[bx]){
                                max_value[bx] = val;
                                max_state[bx] = now;
                            }
                        }
                        if(W_.count(prv)){
                            W_[now] = W_[prv];
                            from_[now] = vector<state>{prv};
                        }
                        continue;
                    }
                    bool condition9 = 1;
                    for(auto it = P.begin(); it != P.end(); ++it)
                        if(!is_in_range(it->first, it->second)){
                            condition9 = 0;
                            break;
                        }
                    
                    
                }


                
                stack<int> st;
                st.push(0);
                while(!st.empty()){ // enumerate
                    ind = st.top();
                    if(ind == (int)v.size()){// transform
                        st.pop();
                        bool condition9 = 1;
                        for(int i = 0; i < ind; ++i){
                            now.P[elm[i]] = v[i];
                            if(!is_in_range(elm[i], v[i])) condition9 = 0;
                        }
                        
                        if(s.count(u)){
                            int f = 0;
                            for(auto it = s.begin(); it != s.end(); ++it) f += edge_weight[{u, *it}]; // calculate condition 8
                            if(f != v[u_idx]) continue;

                            set<int> sy(s);
                            sy.erase(u);
                            state prv = state(by, sy, map<T,T>());
                            if(!sy.empty()){
                                prv.P = now.P;
                                prv.P.erase(u);
                                for(auto it = prv.P.begin(); it != prv.P.end(); ++it)
                                    it->second -= edge_weight[{it->first, u}];
                            }

                            if(f == v[u_idx] && condition9){
                                if(W_.count(prv)){
                                    int val = W_[prv] + weight[u] ;
                                    W[now] = val;
                                    W_[now] = val;
                                    from[now] = vector<state>{prv};
                                    from_[now] = vector<state>{prv};
                                    if(val > max_value[bx]){
                                        max_value[bx] = val;
                                        max_state[bx] = now;
                                    }
                                }
                            }else if(f == v[u_idx] && !condition9)
                                if(W_.count(prv)){
                                    W_[now] = W_[prv] + weight[u];
                                    from_[now] = vector<state>{prv};
                                }
                        }else{ // case 1
                            state prv = state(by, s, map<T,T>());
                            for(int i = 0; i < ind; ++i) prv.P[elm[i]] = v[i];
                            if(W.count(prv)){
                                int val = W[prv];
                                W[now] = val;
                                from[now] = vector<state>{prv};
                                if(val > max_value[bx]){
                                    max_value[bx] = val;
                                    max_state[bx] = now;
                                }
                            }
                            if(W_.count(prv)) W_[now] = W_[prv], from_[now] = vector<state>{prv};
                        }
                        continue;
                    }
                    if(v[ind] >= max_dg){
                        st.pop();
                        v[ind] = -1;
                        continue;
                    }
                    ++v[ind];
                    st.emplace(ind + 1);
                }
                
            }
        }


        void FORGET_transfer(Bag<T> bx, Bag<T> by){
            int u = ntd.specialVertex[bx];
            W[state(bx, set<T>(), map<T,T>())] = 0;
            W_[state(bx, set<T>(), map<T,T>())] = 0; 
            vector<set<int>> ss = get_subset(bx.vertices);
            for(auto &s : ss){
                int N = s.size(), ind = 0, u_idx = -1, tmp_sum = 0;
                vector<int> v(N, -1), elm(N);
                map<T,T> tmp_;
                for(auto &i: s){
                    if(i == u) u_idx = ind;
                    elm[ind++] = i;
                    tmp[i] = 0;
                    tmp_sum += weight[i];
                }
                state now = state(bx, s, tmp);
                W_[now] = tmp_sum;
                
                now.P = map<T,T>();
                stack<int> st;
                st.push(0);

                while(!st.empty()){
                    ind = st.top();
                    if(ind == (int)v.size()){
                        st.pop();
                        for(int i = 0; i < ind; ++i) now.P[elm[i]] = v[i];

                        set<int> sy(s);
                        sy.insert(u);
                        state prv = state(by, sy, now.P), from_state, from_state_;
                        int mx = -INF, mx_ = -INF;
                        for(int i = LowBound[u]; i <= UpBound[u]; ++i){
                            prv.P[u] = i;
                            if(W.count(prv)){
                                int val = W[prv];
                                if(val > mx){
                                    mx = val;
                                    from_state = prv;
                                }
                            }
                            if(W_.count(prv)){
                                int val = W_[prv];
                                if(val > mx_){
                                    mx_ = val;
                                    from_state_ = prv;
                                }
                            }
                        }
                        prv.P = now.P;
                        prv.S = s;
                        if(W.count(prv)){
                            int val = W[prv];
                            if(val > mx){
                                mx = val;
                                from_state = prv;
                            }
                        }
                        if(W_.count(prv)){
                                int val = W_[prv];
                                if(val > mx_){
                                    mx_ = val;
                                    from_state_ = prv;
                                }
                            }
                        if(mx != -INF){
                            W[now] = mx;
                            from[now] = vector<state>{from_state};
                            if(mx > max_value[bx]){
                                max_value[bx] = mx;
                                max_state[bx] = now;
                            }
                        }
                        if(mx_ != -INF){
                            W_[now] = mx_;
                            from_[now] = vector<state>{from_state_};
                        }
                        continue;
                    }
                    if(v[ind] >= max_dg){
                        st.pop();
                        v[ind] = -1;
                        continue;
                    }
                    
                    ++v[ind];
                    st.emplace(ind + 1);
                }
            }
        }

        // void Enumerate(){
        //     int N = s.size(), ind = 0, u_idx = -1;
        //     vector<int> v(N, -1), elm(N);
        //     for(auto &i: s){
        //         if(i == u) u_idx = ind;
        //         elm[ind++] = i;
        //     }
        //     state now = state(bx, s, map<T,T>());
        //     stack<int> st;
        //     st.push(0);

        //     while(!st.empty()){
        //         ind = st.top();
        //         if(ind == (int)v.size()){
        //             st.pop();
                    
        //             continue;
        //         }
        //         if(v[ind] > max_dg){
        //             st.pop();
        //             v[ind] = 0;
        //             continue;
        //         }
        //         ++v[ind];
        //         st.emplace(ind + 1);
        //     }
        // }

 
        void JOIN_transfer(Bag<T> bx, Bag<T> by, Bag<T> bz){ // need optimize
            W[state(bx, set<T>(), map<T,T>())] = 0;
            W_[state(bx, set<T>(), map<T,T>())] = 0; 
            vector<set<int>> ss = get_subset(bx.vertices);
            for(auto &s : ss){
                int N = s.size(), ind = 0,  tot = 0;
                vector<int> v(N, -1), elm(N);
                for(auto &i: s){
                    elm[ind++] = i;
                    tot += weight[i];
                }
                state now = state(bx, s, map<T,T>());
                stack<int> st;
                st.push(0);

                while(!st.empty()){
                    ind = st.top();
                    if(ind == (int)v.size()){
                        st.pop();
                        // transform
                        int ind2 = 0;
                        vector<int> v2(N, -1);
                        stack<int> st2;
                        st2.push(0);
                        state py = state(by, s, map<T,T>());
                        for(int i = 0; i < ind; ++i) py.P[elm[i]] = v[i];
                        while(!st2.empty()){
                            ind2 = st2.top();
                            if(ind2 == (int)v2.size()){
                                st2.pop();
                                state pz = state(bz, s, map<T,T>());
                                bool flag = 1;
                                for(int i = 0, k; i < ind2; ++i){
                                    pz.P[elm[i]] = v2[i];

                                    k = v[i] + v2[i];
                                    for(auto &j : elm) k -= edge_weight[{elm[i], j}];
                                    
                                    now.P[elm[i]] = k;
                                    if(!is_in_range(elm[i], k)) flag = 0;
                                }
                                if(flag){
                                    if(W_.count(py) && W_.count(pz)){
                                        int val = W_[py] + W_[pz] - tot;
                                        if(val > W[now]){
                                            W[now] = val;
                                            from[now] = vector<state>{py, pz};
                                            fromTag[now] = 2;
                                            if(val > max_value[bx]){
                                                max_value[bx] = val;
                                                max_state[bx] = now;
                                            }
                                        }
                                        val =  W_[py] + W_[pz] - tot;
                                        if(val > W_[now]){
                                            W_[now] = val;
                                            from_[now] = vector<state>{py, pz};
                                        }
                                    }
                                }else{
                                    if(W_.count(py) && W_.count(pz)){
                                        int val =W_[py] + W_[pz] - tot;
                                        if(val > W_[now]){
                                            W_[now] = val;
                                            from_[now] = vector<state>{py, pz};
                                        }
                                    }
                                }
                                continue;
                            }
                            if(v2[ind2] >= max_dg){
                                st2.pop();
                                v2[ind2] = -1;
                                continue;
                            }
                            ++v2[ind2];
                            st2.emplace(ind2 + 1);
                        }
                        continue;
                    }
                    if(v[ind] >= max_dg){
                        st.pop();
                        v[ind] = -1;
                        continue;
                    }
                    ++v[ind];
                    st.emplace(ind + 1);
                }
            }
            
        }

        set<int> BackTrack(state now){
            // cout << now.B.id << endl;
            int tag = static_cast<int>(ntd.bagType[now.B]);
            if(tag == 0)
                return now.B.vertices; // leaf
            
            if(tag == 1){ // introduce
                int u = ntd.specialVertex[now.B];
                set<T> ret = (from.count(now) ? BackTrack(from[now][0]) : set<T>());
                if(now.S.count(u))
                    ret.insert(u);
                return ret;
                
            }else if(tag == 2){//forget
                if(from.count(now))
                    return BackTrack(from[now][0]);
                return set<T>();
            }else if(tag == 3){//join
                set<T> x = BackTrack(from[now][0]), y = BackTrack(from[now][1]);
                for(auto &i:y) x.insert(i);
                return x;
            }
        }

        void solve(){
            stack<Bag<T>> st;
            set<Bag<T>> vis;
            st.push(ntd.root);
            vis.insert(ntd.root);

            while(!st.empty()){
                Bag<T> v = st.top();
                int tag = static_cast<int>(ntd.bagType[v]);
                
                if(tag == 0){
                    LEAF_trasfer(v);
                    st.pop();
                    continue;
                }

                bool flag = 0;
                for(Bag<T> w : ntd.treeDecomposition.tree.adj[v]){
                    if(vis.count(w)) continue;
                    st.push(w);
                    vis.insert(w);
                    flag = 1;
                }


                if(flag) continue;
                // cout << v.id << ' ' << max_value[v] << endl;
                if(tag == 1)// introduce
                    INTRODUCE_transfer(v, ntd.childrenBag[v][0]);
                else if(tag == 2)// forget
                    FORGET_transfer(v, ntd.childrenBag[v][0]);
                else if(tag == 3)
                    JOIN_transfer(v, ntd.childrenBag[v][0], ntd.childrenBag[v][1]);
                
                // state tmp = max_state[v];
                st.pop();
            }
            
            // cout << W.size() << ' ' << W_.size() << "\n";
            Bag<T> start = ntd.childrenBag[ntd.root][0];
            set<T> ans = BackTrack(max_state[start]);
            cout << "Ans: " << max_value[start] << '\n'; 
            // cout << ans.size() << ": ";
            // for(auto &i: ans ) cout << i << ' ';
            cout << "\n";

            cout << "W_:\n";
            for(auto it = W_.begin(); it != W_.end(); ++it){
                cout << "id: " << it->first.B.id << "  S: {";
                for(auto &i : it->first.S) cout << i << ' ';
                cout << "}  P: {";
                for(auto &i : it->first.P) cout << i.first << " : " <<i.second << ", ";
                cout <<"}   ";
                cout << it->second << "\n";

                if(!from_.count(it->first)) continue;
                auto f = from_[it->first];
                cout << "from:   ";
                cout << "id: " << f[0].B.id << "  S: {";
                for(auto &i : f[0].S) cout << i << ' ';
                cout << "}  P: {";
                for(auto &i : f[0].P) cout << i.first << " : " <<i.second << ", ";
                cout <<"}\n--------------\n";
                if(f.size() == 2){
                    cout << "from:   ";
                    cout << "id: " << f[1].B.id << "  S: {";
                    for(auto &i : f[1].S) cout << i << ' ';
                    cout << "}  P: {";
                    for(auto &i : f[1].P) cout << i.first << " : " <<i.second << ", ";
                    cout <<"}\n--------------\n";
                }
            }
        }

        
};
