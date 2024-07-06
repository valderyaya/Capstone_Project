#include "TreeDecomposition.h"
#include "MinimalSeparator.h"

template<typename T>
class ImproveTreeDecomposition{
    public:
        TreeDecomposition<T> treeDecomposition;

        ImproveTreeDecomposition(const TreeDecomposition<T> &orignal){
            treeDecomposition = orignal;
        }

        bool in_another_bag(T u, T v, Bag<T> b){
            for(Bag<T> &d : treeDecomposition.tree.adj)
                if(b != d && d.contains(u) && d.contains(v))
                    return 1;
            return 0;
        }

        Graph<T> to_graph(Bag<T> b){
            Graph<T> g = Graph<T>();
            for(T &v : b.vertices)
                g.add_vertex(v);
            for(T &u : b.vertices)
                for(T &v : b.vertices)
                    if(treeDecomposition.graph.is_adjacent(u, v) || in_another_bag(u, v, b))
                        g.add_edge(u, v);
            return g;
        }

        void improve_decomposition(){
            bool flag = 1;
            do{
                flag = 0;
                for(auto &[b, s] : treeDecomposition.tree.adj){
                    Graph<T> g = to_graph(b);
                    if(!g.isClique()){
                        improve_bag(b);
                        flag = 1;
                        break;
                    }
                }
            }while(flag);
        }

        void improve_bag(Bag<T> b){
            Graph<T> g = to_graph(b);
            set<Bag<T>> neighbours;
            for(auto &i: treeDecomposition.tree.adj[b]) neighbours.insert(i);
            
            set<T> sep = MinimalSeparator<T>(g).get_seperator();
            for(T &v : sep) g.remove_vertex(v);

            vector<set<T>> cs =  g.get_connected_components();
            treeDecomposition.tree.remove_vertex(b);

            Bag<T> bsep = treeDecomposition.create_Bag(sep);
            treeDecomposition.tree.add_vertex(bsep);

            for(set<T> s : cs){
                set<T> tset(s);
                for(auto &i: sep) s.insert(i);
                Bag<T> bset = treeDecomposition.create_Bag(s);
                treeDecomposition.tree.add_vertex(bset);
                treeDecomposition.add_tree_edge(bset, bsep);

                for(Bag<T> bx : neighbours){
                    set<T> tmp(sep), remain;
                    set_intersection(tmp.begin(), tmp.end(), bx.vertices.begin(), bx.vertices.end(), inserter(remain, remain.begin()));
                    set<T> union_(sep);
                    for(auto &i:tset) union_.insert(i);
                    bool flag = 1;
                    for(auto &i: remain)
                        if(union_.find(i) == union_.end()){
                            flag = 0;
                            break;
                        }
                    if(flag) treeDecomposition.add_tree_edge(bset, bx);
                }
            }

        }
};