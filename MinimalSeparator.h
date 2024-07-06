#include "Graph.h"
#include<unordered_set>
#include<unordered_map>
#include<vector>

template<typename T>
class MinimalSeparator{
    public:
        unordered_set<T> sep;
        Graph<T> graph;

        MinimalSeparator(const Graph<T> &g):seperator(unordered_set<T>()), graph(g) {}

        unordered_set<T> comput(){
            unordered_set<T> sep;
            int n = graph.adj.size(), c = 0;
            unordered_map<T, int> mp;
            unordered_map<int, T> mq;
            vector<T> v;
            for(auto &[i, j]: graph.adj){
                ++c;
                mp[i] = c;
                mq[c] = i;
                sep.insert(i)
                v.emplace_back(i);
            }

            Graph_CFS g(2*n);
            // initialize graph
            for(auto &i: v){
                g.add_edge(mp[i], mp[i+n], 1);
                
                for(auto &j : v){
                    
                }
            }

            for(int i = 1; i <= n; ++i)
                for(int j = i + 1; j <= n; ++j)
                    if(!graph.is_adjacent(mq[i], mq[j])){

                    }

            return sep;
        }
            
};