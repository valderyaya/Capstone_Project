#include "Graph.h"
#include "Bag.h"


template<typename T>
class TreeDecomposition{
    public:
        int numOfBags;
        int width;
        int n; // size of original graph
        Graph<Bag<T>> tree; // The tree of the tree-decomposition
        Graph<T> graph; // The original graph

        TreeDecomposition(const Graph<T> &graph){
            graph = graph;
            tree = Graph<Bag<T>>();
            numOfBags = 0;
            width = -1;
            n = graph.get_num_of_vertices();
        }

        int get_width(){
            return width;
        }

        void add_tree_edge(Bag<T> bi, Bag<T> bj){
            
        }
};