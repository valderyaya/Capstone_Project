#include "Graph.h"

template<typename T>
class MinimalSeparator{
    public:
        set<T> seperator;
        Graph<T> graph;

        MinimalSeparator(const Graph<T> &g):seperator(), graph(g) {}

        
};