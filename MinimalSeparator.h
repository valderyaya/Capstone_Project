#include "Graph.h"
#include<unordered_set>

template<typename T>
class MinimalSeparator{
    public:
        unordered_set<T> seperator;
        Graph<T> graph;

        MinimalSeparator(const Graph<T> &g):seperator(), graph(g) {}

        
};