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
            state(Bag<T> b, set<T> s, set<T> p):B(b),S(s),P(p){}
            bool operator<(const state &d)const{
                if(B != d.B) return B < d.B;
                if(S != d.S) return S < d.S;
                return P < d.P;
            }
        };

        struct dsu{
            vector<int> f;
            int tot;
            int find_(int x){return x == f[x] ? x : f[x] = find_(f[x]);}
            void union_(int x, int y){
                x = find_(x), y = find_(y);
                if(x == y) return;
                f[x] = y;
                --tot;
            }

            dsu(int n){
                tot = n;
                f.resize(n + 1);
                for(int i = 1; i <= n; ++i) f[i] = i;
            }
        };

        const int INF = 2147483647;
        vector<int> UpBound, LowBound, weight;
        // map<pair<int, int>, int> edge_weight; 
        map<pair<int, int>, int> edge_id;
        vector<pair<int,int>> edge;
        NiceTreeDecomposition<T> ntd;
        map<state, vector<state>> from, from_;
        map<state, int> fromTag;
        map<state, int> W, W_;
        map<Bag<T>, state> max_state;
        map<Bag<T>, int> max_value;
        map<Bag<T>, set<int>> lookup;
        state max_root_state;
        int H, Initiator, n;
        set<int> ans_vertices, ans_edge;
        
        DCSGQ(NiceTreeDecomposition<T> &x):ntd(x){}
        // DCSGQ(){}
        

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
            lookup[b].insert(v);
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


        void INTRODUCE_transfer(Bag<T> bx, Bag<T> by){ 
            int u = ntd.specialVertex[bx];
            lookup[bx] = lookup[by];
            lookup[bx].insert(u);
            W[state(bx, set<T>(), set<T>())] = 0;
            W_[state(bx, set<T>(), set<T>())] = 0; 
            vector<set<int>> ss = get_subset(bx.vertices);
            for(auto &s : ss){
                int N = s.size(), tmp_sum = 0;
                vector<int> v;//, elm(N);
                set<int> edges, u_edge;
                for(auto &i: s){
                    tmp_sum += weight[i];
                    for(auto j : ntd.treeDecomposition.graph.adj[i])
                        if(lookup[bx].count(j)) edges.insert(edge_id[{i, j}]);
                }
                state now = state(bx, s, set<T>());
                W_[now] = tmp_sum;
                
                for(auto &i: ntd.treeDecomposition.graph.adj[u])
                    if(s.count(i)) u_edge.insert(edge_id[{i, u}]);
                
                // for(int i = 0; i < N; ++i)
                //     for(int j = i + 1; j < N; ++j)
                //         v.emplace_back(edge_id[{i, j}]);
                for(auto &i: edges)
                    v.emplace_back(i);
                    
                int M = v.size();
                for(int mask = 0; mask < (1 << M); ++mask){
                    set<int> edge_set;
                    map<int, int> P, mapping;
                    int ind = 0;

                    for(int i = mask, j; i; i = (i - 1) & i){
                        j = v[__lg((i & (-i)))];
                        edge_set.insert(j);
                        int x = edge[j].first, y = edge[j].second;
                        ++P[x];
                        ++P[y];
                        if(!mapping[x]) mapping[x] = ++ind;
                        if(!mapping[y]) mapping[y] = ++ind; 
                    }
                    now.P = edge_set;
                    if(!s.count(u)){// u not in s
                        set<int> sy(s);
                        sy.erase(u);
                        state prv = state(by, sy, edge_set);
                        if(W.count(prv)){
                            int val = W[prv];
                            W[now] = val;
                            from[now] = vector<state>{prv};
                            if(val > max_value[bx]){
                                max_value[bx] = val;
                                max_state[bx] = now;
                            }
                            fromTag[now] = 1;
                        }
                        if(W_.count(prv)){
                            W_[now] = W_[prv];
                            from_[now] = vector<state>{prv};
                            fromTag[now] = 2;
                        }
                        continue;
                    }

                    dsu DSU(mapping.size());
                    for(auto &i : edge_set) 
                        DSU.union_(mapping[edge[i].first], mapping[edge[i].second]);

                    bool condition9 = 1;
                    for(auto &i: s){
                        auto it = P.find(i);
                        if(it == P.end() || !is_in_range(it->first, it->second)){
                            condition9 = 0;
                            break;
                        }
                    }
                    set<T> diff;
                    set_difference(edge_set.begin(), edge_set.end(), u_edge.begin(), u_edge.end(), inserter(diff, diff.end()));
                    set<int> sy(s);
                    sy.erase(u);
                    state prv = state(by, sy, diff);
                    if(condition9  && DSU.tot == 1  && !edge_set.empty()){
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
                            fromTag[now] = 2;
                            // if(bx.id == 7 && now.S.size() > 0){
                            //     for(auto &i:now.S) cout << i << ' ';
                            //     cout << "\n";
                            //     for(auto &i:now.P) cout << edge[i].first << ' ' << edge[i].second<<"\n";
                            //     cout << "-----------" << W_[prv] << ' ' <<  val << "\n";
                            //     for(auto &i:prv.S) cout << i << ' ';
                            //     cout << "\n";
                            //     for(auto &i:prv.P) cout << edge[i].first << ' ' << edge[i].second<<"\n";
                            //     cout << "\n\n\n";
                            // }
                        }
                    }else if(DSU.tot == 1){
                        if(W_.count(prv)){
                            W_[now] = W_[prv] + weight[u];
                            from_[now] = vector<state>{prv};
                            fromTag[now] = 2;
                        }
                    }
                }
            }
        }


        void FORGET_transfer(Bag<T> bx, Bag<T> by){
            int u = ntd.specialVertex[bx];
            lookup[bx] = lookup[by];
            W[state(bx, set<T>(), set<T>())] = 0;
            W_[state(bx, set<T>(), set<T>())] = 0; 
            vector<set<int>> ss = get_subset(bx.vertices);
            for(auto &s : ss){
                int N = s.size(), ind = 0, tmp_sum = 0;
                vector<int> v, u_v;//, elm(N) ;
                // map<int,int> mapping;
                set<int> edges, u_edge;
                for(auto &i: s){
                    // elm[ind++] = i;
                    // mapping[i] = ++ind;
                    tmp_sum += weight[i];
                    for(auto &j : ntd.treeDecomposition.graph.adj[i])
                        if(lookup[bx].count(j)) edges.insert(edge_id[{i, j}]);
                }
                for(auto &i:ntd.treeDecomposition.graph.adj[u])
                    u_edge.insert(edge_id[{i, u}]);

                // for(int i = 0; i < N; ++i)
                //     for(int j = i + 1; j < N; ++j)
                //         v.emplace_back(edge_id[{i, j}]);
                for(auto &i: edges)
                    v.emplace_back(i);

                for(auto &i: u_edge)
                    u_v.emplace_back(i);

                state now = state(bx, s, set<T>());
                W_[now] = tmp_sum;  
                                
                int M = v.size(), uM = u_v.size();
                for(int mask = 1; mask < (1 << M); ++mask){
                    set<int> edge_set;
                    map<int, int> P;
                    // dsu DSU(s.size());
                    for(int i = mask, j; i; i = (i - 1) & i){
                        j = v[__lg((i & (-i)))];
                        edge_set.insert(j);
                        if(s.count(edge[j].first)) ++P[edge[j].first];
                        if(s.count(edge[j].second)) ++P[edge[j].second];
                        
                        // int x=edge[j].first, y=edge[j].second;
                        // if(s.count(x) && s.count(y)) DSU.union_(mapping[x], mapping[y]);
                    }
                    now.P = edge_set;
                    set<int> sy(s);
                    sy.insert(u);
                    state prv = state(by, sy, set<T>()), from_state, from_state_;
                    int mx = -INF, mx_ = -INF;

                    for(int u_mask = 1; u_mask < (1 << uM); ++u_mask){ // OPT_II
                        set<int> prv_edge_set(edge_set);
                        for(int i = u_mask, j; i; i = (i - 1) & i){
                            j = u_v[__lg(i & (-i))];
                            prv_edge_set.insert(j);
                        }
                        // if(edge_set.empty()) continue;
                        int u_deg = 0;
                        map<int,int> prvP;
                        for(auto &i : prv_edge_set){
                            if(edge[i].first == u || edge[i].second == u) ++u_deg;
                            if(s.count(edge[i].first)) ++prvP[edge[i].first];
                            if(s.count(edge[i].second)) ++prvP[edge[i].second];
                        }
                        if(!is_in_range(u, u_deg)) continue;
                        bool condition = 1;
                        for(auto &i:s) 
                            if(P[i] != prvP[i]){
                                condition = 0;
                                break;
                            }
                        if(!condition) continue;
                        prv.P = prv_edge_set;
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

                    prv.S = s, prv.P = edge_set;
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
                        fromTag[now] = 1;
                        // if(bx.id == 6 && now.S.size() > 0){
                        //     for(auto &i:now.S) cout << i << ' ';
                        //         cout << "\n";
                        //         for(auto &i:now.P) cout << edge[i].first << ' ' << edge[i].second<<"\n";
                        //         cout << "-----------" << mx << "\n";
                        //         for(auto &i:from_state.S) cout << i << ' ';
                        //         cout << "\n";
                        //         for(auto &i:from_state.P) cout << edge[i].first << ' ' << edge[i].second<<"\n";
                        //         cout << "\n\n\n";
                        // }
                    }
                    if(mx_ != -INF){
                        W_[now] = mx_;
                        from_[now] = vector<state>{from_state_};
                        fromTag[now] = 2;
                    }
                }
            }
        }

 
        void JOIN_transfer(Bag<T> bx, Bag<T> by, Bag<T> bz){ // need optimize
            W[state(bx, set<T>(), set<T>())] = 0;
            W_[state(bx, set<T>(), set<T>())] = 0; 
            vector<int> tmp_v;
            merge(lookup[by].begin(), lookup[by].end(), lookup[bz].begin(), lookup[bz].end(), back_inserter(tmp_v));
            set<int> merge_set(tmp_v.begin(), tmp_v.end());
            lookup[bx] = merge_set;
            vector<set<int>> ss = get_subset(bx.vertices);
            for(auto &s : ss){
                int N = s.size(), ind = 0, tot_sum = 0;
                vector<int> v_y, v_z;//, elm(N) ;
                set<int> edges_y, edges_z;
                for(auto &i: s){
                    // elm[ind++] = i;
                    tot_sum += weight[i];
                    for(auto &j : ntd.treeDecomposition.graph.adj[i]){
                        if(lookup[by].count(j)) edges_y.insert(edge_id[{i, j}]);
                        if(lookup[bz].count(j)) edges_z.insert(edge_id[{i, j}]);
                    }
                }

                for(auto &i: edges_y)
                    v_y.emplace_back(i);

                for(auto &i: edges_z)
                    v_z.emplace_back(i);

                state now = state(bx, s, set<T>());
                // W_[now] = tot_sum;
                                
                int M = v_y.size(), MM = v_z.size();
                for(int mask = 0; mask < (1 << M); ++mask){
                    set<int> edge_set;
                    for(int i = mask, j; i; i = (i - 1) & i){
                        j = v_y[__lg((i & (-i)))];
                        edge_set.insert(j);
                    }
                    state py = state(by, s, edge_set);
                    
                    for(int mask_z = 0; mask_z < (1 << MM); ++mask_z){
                        set<int> edge_set_z;
                        for(int i = mask_z, j; i; i = (i - 1) & i){
                            j = v_z[__lg((i & (-i)))];
                            edge_set_z.insert(j);
                        }
                        tmp_v.clear();
                        merge(edge_set.begin(), edge_set.end(), edge_set_z.begin(), edge_set_z.end(), back_inserter(tmp_v));
                        merge_set = set<int>(tmp_v.begin(), tmp_v.end());
                        now.P = merge_set;

                        map<int,int> P;
                        for(auto &i:merge_set){
                            ++P[edge[i].first];
                            ++P[edge[i].second];
                        }
                        bool condition = 1;
                        for(auto &i: s){
                            auto it = P.find(i);
                            if(it == P.end() || !is_in_range(it->first, it->second)){
                                condition = 0;
                                break;
                            }
                        }
                        
                        state pz = state(bz, s, edge_set_z);
                        if(condition){
                            if(W_.count(py) && W_.count(pz)){
                                int val = W_[py] + W_[pz] - tot_sum;
                                if(val > W[now]){
                                    W[now] = val;
                                    from[now] = vector<state>{py, pz};
                                    fromTag[now] = 2;
                                    if(val > max_value[bx]){
                                        max_value[bx] = val;
                                        max_state[bx] = now;
                                    }
                                }
                                if(val > W_[now]){
                                    W_[now] = val;
                                    from_[now] = vector<state>{py, pz};
                                }
                            }
                        }else{
                            if(W_.count(py) && W_.count(pz)){
                                int val =W_[py] + W_[pz] - tot_sum;
                                if(val > W_[now]){
                                    W_[now] = val;
                                    from_[now] = vector<state>{py, pz};
                                    fromTag[now] = 2;
                                }
                            }
                        }
                    }

                }

            }
            
        }

        void BackTrack(state now){
            // cout << now.B.id << endl;
            int tag = static_cast<int>(ntd.bagType[now.B]);
            if(tag == 0){ // leaf
                for(auto &i : now.B.vertices) ans_vertices.insert(i);
                return;
            }
            for(auto &i : now.S)
                ans_vertices.insert(i);
            for(auto &i : now.P)
                ans_edge.insert(i);

            if(tag < 3){
                int fm = fromTag[now];
                if(fm == 1 && from.count(now))
                    BackTrack(from[now][0]);
                else if(fm == 2 && from_.count(now))
                    BackTrack(from_[now][0]); 
                return;

            }else if(tag == 3){//join
                int fm = fromTag[now];
                if(fm == 1 && from.count(now)){
                    BackTrack(from[now][0]);
                    BackTrack(from[now][1]);
                }else if(fm == 2 && from_.count(now)){
                    BackTrack(from_[now][0]);
                    BackTrack(from_[now][1]); 
                }
                return;
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
                // cout << v.id << endl;
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
            cout << "Ans: " << max_value[start] << '\n';
            BackTrack(max_state[start]);
            for(auto &i: ans_vertices) cout << i << ' ';
            cout << "\n";
            for(auto &i: ans_edge) cout << i << ' ';
            cout << "\n";

            // for(auto it = ntd.treeDecomposition.tree.adj.begin(); it != ntd.treeDecomposition.tree.adj.end(); ++it){
            //     cout << it->first.id << " :   " << max_value[it->first] << "\n";
            // }

            // cout << "W:\n";
            // for(auto it = W.begin(); it != W.end(); ++it){ 
            //     if(it->first.B.id != 2) continue;
            //     cout << "id: " << it->first.B.id << "  S: {";
            //     for(auto &i : it->first.S) cout << i << ' ';
            //     cout << "}  P: {";
            //     for(auto &i : it->first.P) cout << i << ", ";
            //     cout <<"}   ";
            //     cout << it->second << "\n";

            //     if(!from_.count(it->first) && !from.count(it->first)) continue;
                
            //     auto f = fromTag[it->first] == 2 ? from_[it->first] : from[it->first];
            //     int val = fromTag[it->first] == 2 ? W_[f[0]] : W[f[0]];
            //     cout << "from:   ";
            //     cout << "id: " << f[0].B.id << "  S: {";
            //     for(auto &i : f[0].S) cout << i << ' ';
            //     cout << "}  P: {";
            //     for(auto &i : f[0].P) cout << i << ", ";
            //     cout << "}  " << val << "\n--------------\n";
            //     if(f.size() == 2){
            //         val = fromTag[it->first] == 2 ? W_[f[1]] : W[f[1]];
            //         cout << "from:   ";
            //         cout << "id: " << f[1].B.id << "  S: {";
            //         for(auto &i : f[1].S) cout << i << ' ';
            //         cout << "}  P: {";
            //         for(auto &i : f[1].P) cout << i  << ", ";
            //         cout <<"}  " << val << "\n--------------\n";
            //     }
            // }

            // cout << "W_:\n";
            // for(auto it = W_.begin(); it != W_.end(); ++it){ 
            //     if(it->first.B.id != 4) continue;
            //     cout << "id: " << it->first.B.id << "  S: {";
            //     for(auto &i : it->first.S) cout << i << ' ';
            //     cout << "}  P: {";
            //     for(auto &i : it->first.P) cout << i << ", ";
            //     cout <<"}   ";
            //     cout << it->second << "\n";

            //     if(!from_.count(it->first) && !from.count(it->first)) continue;
                
            //     auto f = fromTag[it->first] == 2 ? from_[it->first] : from[it->first];
            //     int val = fromTag[it->first] == 2 ? W_[f[0]] : W[f[0]];
            //     cout << "from:   ";
            //     cout << "id: " << f[0].B.id << "  S: {";
            //     for(auto &i : f[0].S) cout << i << ' ';
            //     cout << "}  P: {";
            //     for(auto &i : f[0].P) cout << i << ", ";
            //     cout << "}  " << val << "\n--------------\n";
            //     if(f.size() == 2){
            //         val = fromTag[it->first] == 2 ? W_[f[1]] : W[f[1]];
            //         cout << "from:   ";
            //         cout << "id: " << f[1].B.id << "  S: {";
            //         for(auto &i : f[1].S) cout << i << ' ';
            //         cout << "}  P: {";
            //         for(auto &i : f[1].P) cout << i  << ", ";
            //         cout <<"}  " << val << "\n--------------\n";
            //     }
            // }
        
        }

        
};
