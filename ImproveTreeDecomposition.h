// #ifdef IMPROVETREEDECOMPOSITION_H
// #define IMPROVETREEDECOMPOSITION_H

#pragma once

#include "TreeDecomposition.h"
#include "MinimalSeparator.h"

template<typename T>
class ImproveTreeDecomposition{
    public:
        TreeDecomposition<T> treeDecomposition;

        void print_graph_bag(Graph<Bag<int>> &g){
            int c = 0;
            for(auto &[x, y] : g.adj){
                cout << x.id << ": {";
                for(auto &i : x.vertices) cout << i <<' ';
                cout << "}\n" ;
            }
            for(auto &[x, y] : g.adj){
                cout << x.id << ": ";
                for(auto &z : y) cout << z.id << ' ';
                cout << "\n";
            }
        }

        ImproveTreeDecomposition(const TreeDecomposition<T> &orignal){
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

        // void Impore_Decomposition(){
        //     set<int> s;
        //     int n= treeDecomposition.graph.adj.size();
        //     for(int i = 1; i <= n; ++i) s.insert(i);
        //     Bag<T> init(s, -1);

        //     stack<pair<Bag<T>, Bag<T>>> st;
        //     st.push({init, Bag<T>()});
            
        //     while(!st.empty()){
        //         auto [cur, par] = st.top(); st.pop();
        //         Graph<T> g = to_graph(cur);
        //         if(g.isClique()){
        //             Bag<T> curb = treeDecomposition.create_Bag(cur.vertices);
        //             if(par != Bag<T>()){
        //                 treeDecomposition.tree.add_edge(par, curb);
        //             }
        //             continue;
        //         }
        //         set<T> sep = MinimalSeparator<T>(g).compute();
        //         for(T v : sep) g.remove_vertex(v);
        //         vector<set<T>> cs =  g.get_connected_components();
        //         Bag<T> bsep = treeDecomposition.create_Bag(sep);
        //         if(par != Bag<T>()){
                    
        //             treeDecomposition.tree.add_edge(par, bsep);
        //         }
        //         for(set<T> s : cs){
        //             for(auto &i: sep) s.insert(i);
        //             Bag<T> bset(s, -1);
        //             st.push({bset ,bsep});
        //         }
        //     }
        // }

        void improve_decomposition(){
            bool flag = 1;
            int cnt =0;
            do{
                // cout<<++cnt<<endl;
                flag = 0;
                for(auto it = treeDecomposition.tree.adj.begin(); it != treeDecomposition.tree.adj.end(); ++it){
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
            // cout << "sep: ";
            // for(auto &i:sep) cout << i << ' ';
            // cout << endl;
            
            for(T v : sep) g.remove_vertex(v);

            vector<set<T>> cs =  g.get_connected_components();

            treeDecomposition.tree.remove_vertex(b);

            Bag<T> bsep = treeDecomposition.create_Bag(sep);
            treeDecomposition.tree.add_vertex(bsep);
            int cnt = 0;
            for(set<T> s : cs){
                for(auto &i: sep) s.insert(i);
                Bag<T> bset = treeDecomposition.create_Bag(s);
                treeDecomposition.tree.add_vertex(bset);
                treeDecomposition.tree.add_edge(bset, bsep);
                
                for(Bag<T> bx : neighbours){
                    // set<T> interset;
                    // set_intersection(sep.begin(), sep.end(), bx.vertices.begin(), bx.vertices.end(), inserter(interset, interset.begin()));

                    bool containsALL =  includes(s.begin(), s.end(), bx.vertices.begin(), bx.vertices.end());
                    if(containsALL && cnt < 1) ++cnt, treeDecomposition.tree.add_edge(bset, bx);
                }

                // for(Bag<T> bx : neighbours){
                //     set<T> interset(sep);
                //     for(auto &i : bx.vertices)
                //         interset.erase(i);
                    
                    
                //     bool containsALL =  includes(s.begin(), s.end(), interset.begin(), interset.end());
                //     if(containsALL) treeDecomposition.tree.add_edge(bset, bx);
                // }
            }

            // for(Bag<T> bx : neighbours)
            //     treeDecomposition.tree.add_edge(bsep, bx);
        }
};

// #endif 