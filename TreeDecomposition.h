#include "Graph.h"
//#include "Bag.h"


template<typename T>
class TreeDecomposition{
    public:
        int numOfBags;
        int width;
        int n; // size of original graph
        Graph<Bag<T>> tree; // The tree of the tree-decomposition
        Graph<T> graph; // The original graph

        TreeDecomposition():numOfBags(0), width(-1), n(0), tree(), graph(){}

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

        /**
         * Add an edge to the tree. This will _not_ add the two bags, if the corresponding nodes
         * are not already part of the decomposition. In this, the edge is not added.
         */
        void add_tree_edge(Bag<T> bi, Bag<T> bj){
            if(bi.isEmpty() || bj.isEmpty()) return;
            if(!tree.containsNode(bi)) return;
            if(!tree.containsNode(bj)) return;
            if(tree.adj[bi].find(bj) != tree.adj[bi].end()) return;
            tree.add_edge(bi, bj);
        }
};