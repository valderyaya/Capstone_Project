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
            MaximumClique<int> clq(treeDecomposition.graph.adj.size());

            for(auto it = treeDecomposition.graph.adj.begin(); it != treeDecomposition.graph.adj.end(); ++it)
                for(auto jt : it->second) clq.add_edge(it->first, jt);
            clq.maximum_clique();
            set<int> ss;
            for(auto &i: clq.ans) ss.insert(i);
            Bag<T> tag(ss, 0);
            Graph<T> g = to_graph(tag);

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
            print_graph_bag(niceTreeDecomposition.treeDecomposition.tree);
            // return niceTreeDecomposition;
        }
};