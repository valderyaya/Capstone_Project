#include "Graph.h"
#include<unordered_set>
#include<unordered_map>
#include<vector>

template<typename T>
class MinimalSeparator{
    public:
        unordered_set<T> sep;
        Graph<T> graph;
        const int inf = 2147483647;
        
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
                int mu = mp[i];
                g.add_edge(mu, mu + n, 1);
                g.add_edge(mn + n, mu, 0);
                for(auto &j : v){
                    if(graph.is_adjacent(i, j)){
                        int mv = mp[j];
                        g.add_edge(mu + n, mv, inf);
                        g.add_edge(mv, mu + n, 0);
                        g.add_edge(mv + n, mu, inf);
                        g.add_edge(mu, mv + n, 0);
                    }
                }
            }

            for(int i = 1; i <= n; ++i)
                for(int j = i + 1; j <= n; ++j)
                    if(!graph.is_adjacent(mq[i], mq[j])){
                        // dinic

                        unordered_set<T> tmp;
                        for(int k = 1; k <= n; ++k)
                            if(k != i && k != j){
                                // check
                            }
                        if(tmp.size() < sep.size()) sep = tmp;
                    }

            return sep;
        }
            
};