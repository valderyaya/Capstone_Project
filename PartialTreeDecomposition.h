#pragma once


#include "TreeDecomposition.h"
#include "NiceTreeDecomposition.h"
#include "MinimalSeparator.h"
#include "MaximumClique.h"
#include "Debug_func.h"

template<typename T>
class PartialTreeDecomposition{
    public:
        TreeDecomposition<T> treeDecomposition;

        PartialTreeDecomposition(const TreeDecomposition<T> &orignal){
            treeDecomposition = orignal;
        }

        bool in_another_bag(T u, T v, Bag<T> b){
            vector<Bag<T>> comp;
            for(auto it = treeDecomposition.tree.adj.begin(); it != treeDecomposition.tree.adj.end(); ++it)
                comp.emplace_back(it->first);
            for(Bag<T> d : comp)
                if(d != b && d.contains(u) && d.contains(v))
                    return 1;
            return 0;
        }

        Graph<T> to_graph(Bag<T> b){
            Graph<T> g = Graph<T>();
            for(T v : b.vertices)
                g.add_vertex(v);
            for(T u : b.vertices)
                for(T v : b.vertices)
                    if(u != v){
                        if(treeDecomposition.graph.is_adjacent(u, v) || in_another_bag(u, v, b))
                            g.add_edge(u, v);
                    }
            return g;
        }

        void improve_decomposition(Bag<T> tag = Bag<T>()){
            bool flag = 1;
            Bag<T> ept = Bag<T>();
            do{
                flag = 0;
                for(auto it = treeDecomposition.tree.adj.begin(); it != treeDecomposition.tree.adj.end(); ++it){
                    if(tag != ept && it->first == tag) continue;
                    Graph<T> g = to_graph(it->first);
                    if(!g.isClique()){
                        improve_bag(it->first, g);
                        flag = 1;
                        break;
                    }
                }
                // print_graph_bag(treeDecomposition.tree);
                // cout << "----------------" << endl;
            }while(flag);
        }

        void improve_bag(Bag<T> b, Graph<T> g){
            // Graph<T> g = to_graph(b);
            set<Bag<T>> neighbours;
            for(auto &i: treeDecomposition.tree.adj[b]) neighbours.insert(i);

            set<T> sep = MinimalSeparator<T>(g).compute();
            for(T v : sep) g.remove_vertex(v);

            vector<set<T>> cs =  g.get_connected_components();

            treeDecomposition.tree.remove_vertex(b);

            Bag<T> bsep = treeDecomposition.create_Bag(sep);
            treeDecomposition.tree.add_vertex(bsep);

            for(set<T> s : cs){
                for(auto &i: sep) s.insert(i);
                Bag<T> bset = treeDecomposition.create_Bag(s);
                treeDecomposition.tree.add_vertex(bset);
                treeDecomposition.tree.add_edge(bset, bsep);
                
                for(Bag<T> bx : neighbours){
                    // set<T> interset;
                    // set_intersection(sep.begin(), sep.end(), bx.vertices.begin(), bx.vertices.end(), inserter(interset, interset.begin()));

                    bool containsALL =  includes(s.begin(), s.end(), bx.vertices.begin(), bx.vertices.end());
                    if(containsALL) treeDecomposition.tree.add_edge(bset, bx);
                }
            }

        }

        void build_partial_treedecomposition(){
            // set<Bag<T>> neighbours;
            // for(auto &i: treeDecomposition.tree.adj[b]) neighbours.insert(i);
            int n = treeDecomposition.graph.adj.size();
            MaximumClique<int> clq(n);

            for(auto it = treeDecomposition.graph.adj.begin(); it != treeDecomposition.graph.adj.end(); ++it)
                for(auto jt : it->second) clq.add_edge(it->first, jt);
            clq.maximum_clique();
            set<int> ss;
            for(auto &i: clq.ans) ss.insert(i);
            set<int> all;
            for(int i = 1; i <= n; ++i) all.insert(i);

            Bag<T> tag(ss, 0);
            Graph<T> g = to_graph(Bag<T>(all, 0));

            for(auto &i : clq.ans) g.remove_vertex(i);
            vector<set<T>> cs =  g.get_connected_components();

            Bag<T> bsep = treeDecomposition.create_Bag(ss);
            treeDecomposition.tree.add_vertex(bsep);
            for(set<T> s : cs){
                for(auto &i: ss) s.insert(i);
                Bag<T> bset = treeDecomposition.create_Bag(s);
                treeDecomposition.tree.add_vertex(bset);
                treeDecomposition.tree.add_edge(bset, bsep);
                
                // for(Bag<T> bx : neighbours){
                //     // set<T> interset;
                //     // set_intersection(sep.begin(), sep.end(), bx.vertices.begin(), bx.vertices.end(), inserter(interset, interset.begin()));

                //     bool containsALL =  includes(s.begin(), s.end(), bx.vertices.begin(), bx.vertices.end());
                //     if(containsALL) treeDecomposition.tree.add_edge(bset, bx);
                // }
            }
            improve_decomposition(bsep);

            NiceTreeDecomposition<T> niceTreeDecomposition(treeDecomposition);
            
            niceTreeDecomposition.root = niceTreeDecomposition.make_nice(niceTreeDecomposition.findSuitableRoot(), bsep);
            int flag;
            do{
                flag = 0;
                // print_graph_bag(niceTreeDecomposition.treeDecomposition.tree);
                for(auto it = niceTreeDecomposition.treeDecomposition.tree.adj.begin(); it != niceTreeDecomposition.treeDecomposition.tree.adj.end(); ++it){
                    if(!niceTreeDecomposition.partial_check.count(it->first)){ cout << 33<<endl;
                        flag = 1;
                        niceTreeDecomposition.make_nice(it->first, bsep);
                        break;
                    }
                }
            }while(flag == 1);
            niceTreeDecomposition.treeDecomposition.renumber();
            cout << niceTreeDecomposition.treeDecomposition.isValid() << endl;
            print_graph_bag(niceTreeDecomposition.treeDecomposition.tree);
            // return niceTreeDecomposition;
        }
};


// // #ifdef NICETREEDECOMPOSITION_H
// // #define NICETREEDECOMPOSITION_H
// #pragma once

// #include "TreeDecomposition.h"
// //#include<map>

// template<typename T>
// class NiceTreeDecomposition{
//     public:
//         enum class BagType{
//             LEAF,
//             INTRODUCE,
//             FORGET,
//             JOIN,
//             EDGE
//         };

//         TreeDecomposition<T> treeDecomposition;
//         map<T, int> treeIndex;
//         map<Bag<T>, BagType> bagType;
//         map<Bag<T>, T> specialVertex;
//         map<Bag<T>, T> secondSpecialVertex;
//         Bag<T> root;
//         set<Bag<T>> partial_check; // for partial tree decomposition
//         bool veryNice;

//         NiceTreeDecomposition(const TreeDecomposition<T> &orignal){
//             treeDecomposition = orignal;
//         }

//         NiceTreeDecomposition(const TreeDecomposition<T> &orignal, bool verynice){
//             treeDecomposition = orignal;
//             veryNice = verynice;
//         }


//         Bag<T> findSuitableRoot(int initiator = -1){
//             if(initiator != -1)            
//                 for(auto it = treeDecomposition.tree.adj.begin(); it != treeDecomposition.tree.adj.end(); ++it)
//                     if(it->first.contains(initiator)) return it->first;
                    
//             return treeDecomposition.tree.adj.begin()->first;
//         }

//         Bag<T> make_nice(Bag<T> suitableRoot, Bag<T> tag = Bag<T>()){
//             //Graph<Bag<T>> tree = &treeDecomposition.tree;
//             Bag<T> rt = treeDecomposition.create_Bag(set<T>());
//             treeDecomposition.tree.add_vertex(rt);
//             treeDecomposition.tree.add_edge(rt, suitableRoot);

//             stack<Bag<T>> st;
//             set<Bag<T>> vis;
//             st.push(rt);
//             Bag<T> ept = Bag<T>();
//             // int c = 0;
//             while(!st.empty()){
//                 // cout << ++c <<endl;
//                 Bag<T> v =st.top();
//                 st.pop();
//                 vis.insert(v);
                
//                 if(tag != ept) {
//                     partial_check.insert(v);
//                     if(v == tag) continue;
//                 }
//                 int cnt = 0;
//                 Bag<T> ww;
//                 for(auto &i : treeDecomposition.tree.adj[v])
//                     if(!vis.count(i)){
//                         ++cnt;
//                         ww = i;
//                     }
//                 if(cnt == 0 && !v.vertices.empty()){
//                     Bag<T> leaf = treeDecomposition.create_Bag(set<T>());
//                     treeDecomposition.tree.add_vertex(leaf);
//                     treeDecomposition.tree.add_edge(v, leaf);
//                     st.push(v);
//                 }else if(cnt == 1){
//                     if(v.vertices == ww.vertices){
//                         treeDecomposition.tree.contract(v, ww);
//                         st.push(v);
//                         continue;
//                     }
//                     set<T> newBag, diff;
//                     set_difference(v.vertices.begin(), v.vertices.end(), ww.vertices.begin(), ww.vertices.end(), inserter(diff, diff.end()));
//                     if(diff.size() > 0){
//                         newBag = v.vertices;
//                         newBag.erase(*diff.begin());
//                     }else{
//                         diff.clear();
//                         set_difference(ww.vertices.begin(), ww.vertices.end(), v.vertices.begin(), v.vertices.end(), inserter(diff, diff.end()));
//                         newBag = v.vertices;
//                         newBag.insert(*diff.begin());
//                     }
//                     Bag<T> NEWBag = treeDecomposition.create_Bag(newBag);
//                     treeDecomposition.tree.remove_edge(v, ww);
//                     treeDecomposition.tree.add_vertex(NEWBag);
//                     treeDecomposition.tree.add_edge(v, NEWBag);
//                     treeDecomposition.tree.add_edge(NEWBag, ww);
//                     st.push(NEWBag);
//                 }else if(cnt >= 2){
//                     Bag<T> left = treeDecomposition.create_Bag(v.vertices);
//                     Bag<T> right = treeDecomposition.create_Bag(v.vertices);
//                     set<Bag<T>> neighbors;
//                     for(auto &i: treeDecomposition.tree.adj[v])
//                         if(!vis.count(i)) neighbors.insert(i);
//                     for(Bag<T> w : neighbors) 
//                         treeDecomposition.tree.remove_edge(v, w);
//                     treeDecomposition.tree.add_edge(v, left);
//                     treeDecomposition.tree.add_edge(v, right);
//                     int i = 0;
//                     for(Bag<T> w: neighbors){
//                         if(i < neighbors.size() / 2)
//                             treeDecomposition.tree.add_edge(left, w);
//                         else
//                             treeDecomposition.tree.add_edge(right, w);
//                         ++i;
//                     }
//                     st.push(left);
//                     st.push(right);
//                 }
//             }
//             vector<Bag<T>> empt;
//             for(auto it = treeDecomposition.tree.adj.begin(); it != treeDecomposition.tree.adj.end(); ++it) 
//                 if(it->first.vertices.empty()) 
//                     empt.emplace_back(it->first);
//             for(auto &i : empt) treeDecomposition.tree.remove_vertex(i);
//             if(tag == ept) treeDecomposition.renumber();
//             return treeDecomposition.tree.adj.begin()->first;
//             //return rt;
//         }

//         void classify_bags(){
//             stack<Bag<T>> st;
//             set<Bag<T>> vis;
//             st.push(root);
//             vis.insert(root);

//             while(!st.empty()){
//                 Bag<T> v = st.top();
//                 st.pop();
//                 if(treeDecomposition.tree.adj[v].size() == 3) bagType[v] = BagType::JOIN;
//                 for(Bag<T> w : treeDecomposition.tree.adj[v]){
//                     if(vis.count(w)) continue;
//                     if(bagType.find(v) == bagType.end()){
//                         set<T> diff;
//                         set_difference(v.vertices.begin(), v.vertices.end(), w.vertices.begin(), w.vertices.end(), inserter(diff, diff.end()));
//                         if(diff.size() == 1){
//                             bagType[v] = BagType::INTRODUCE;
//                             specialVertex[v] = *diff.begin();
//                         }else{
//                             diff.clear();
//                             set_difference(w.vertices.begin(), w.vertices.end(), v.vertices.begin(), v.vertices.end(), inserter(diff, diff.end()));
//                             bagType[v] = BagType::FORGET;
//                             specialVertex[v] = *diff.begin();
//                         }
//                     }
//                     vis.insert(w);
//                     st.push(w);
//                 }
//                 if(bagType.find(v) == bagType.end()) bagType[v] = BagType::LEAF;
//             }
//         }

//         void compute_tree_index(){
//             stack<int> index;
//             int d = treeDecomposition.get_width() + 1;
//             while(d-- > 0) index.push(d);

//             stack<Bag<T>> st;
//             set<Bag<T>> vis, finish;
//             st.push(root);
//             vis.insert(root);
//             while(!st.empty()){
//                 Bag<T> v = st.top();
//                 if(finish.count(v)){
//                     st.pop();
//                     if(bagType[v] == BagType::FORGET)
//                         index.push(treeIndex[specialVertex[v]]);
//                     else if(bagType[v] == BagType::INTRODUCE)
//                         index.pop();
//                     continue;
//                 }
//                 finish.insert(v);
//                 if(bagType[v] == BagType::FORGET){
//                     treeIndex[specialVertex[v]] = index.top();
//                     index.pop();
//                 }else if(bagType[v] == BagType::INTRODUCE)
//                     index.push(treeIndex[specialVertex[v]]);
                
//                 for(Bag<T> w : treeDecomposition.tree.adj[v]){
//                     if(vis.count(w)) continue;
//                     st.push(w);
//                     vis.insert(w);
//                 }
//             }
//         }

//         void compute_edge_bags(){
//             stack<Bag<T>> st;
//             set<Bag<T>> vis;
//             st.emplace(root);
//             vis.insert(root);

//             while(!st.empty()){
//                 Bag<T> v = st.top();
//                 st.pop();
//                 if(bagType[v] == BagType::FORGET){
//                     T x = specialVertex[v];
//                     Bag<T> child;
//                     for(auto &w : treeDecomposition.tree.adj[v])
//                         if(!vis.count(w)){
//                             child = w;
//                             break;
//                         }
                    
//                     for(T &u : treeDecomposition.graph.adj[x]){
//                         if(child.contains(u)){
//                             Bag<T> newChild = treeDecomposition.create_Bag(child.vertices);
//                             bagType[newChild] = BagType::EDGE;
//                             secondSpecialVertex[newChild] = x;
//                             secondSpecialVertex[newChild] = u;
//                             treeDecomposition.add_tree_edge(child, newChild);
//                             treeDecomposition.add_tree_edge(newChild, v);
//                             treeDecomposition.tree.remove_edge(child, v);
//                             child = newChild;
//                         }
//                     }
//                 }
//                 for(Bag<T> w : treeDecomposition.tree.adj[v])
//                     if(!vis.count(w)){
//                         vis.insert(w);
//                         st.push(w);
//                     }
//             }
//         }

//         Bag<T> get_root(){
//             return root;
//         }
// };

// // #endif 