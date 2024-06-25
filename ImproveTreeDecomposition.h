#include "TreeDecomposition.h"

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

        void improve_bag(Bag<T>){

        }
};