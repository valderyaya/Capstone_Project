#pragma once
#include "Dinic_2.h"
#include<unordered_map>
#include<set>
#include<vector>
template<typename T>
class MinimalSeparator{
    public:
        Graph<T> graph;
        const int inf = 2147483647;
        
        MinimalSeparator(const Graph<T> &g):graph(g) {}

        set<T> compute(){
            set<T> sep;
            int n = graph.adj.size(), c = 0;
            unordered_map<T, int> mp;
            unordered_map<int, T> mq;
            vector<T> v;
            for(auto it = graph.adj.begin(); it != graph.adj.end(); ++it){
                ++c;
                mp[it->first] = c;
                mq[c] = it->first;
                sep.insert(it->first);
                v.emplace_back(it->first);
            }

            vector<vector<int>> g(n * 2 + 1, vector<int>(n * 2 + 1));
            for(auto &i: v){
                int mu = mp[i];
                g[mu][mu + n] = 1;
                for(auto &j : v){
                    int mv = mp[j];
                    g[mu + n][mv] = inf;
                    g[mv + n][mu] = inf;
                }
            }

            for(int i = 1; i <= n; ++i)
                for(int j = i + 1; j <= n; ++j)
                    if(!graph.is_adjacent(mq[i], mq[j])){
                        set<T> tmp;
                        Dinic dc(g, i, j + n);
                        vector<vector<bool>> res = dc.start();
                        for(int l = 1; l <= n; ++l)
                            if(l != i && l != j)
                                if(res[l][l + n]) tmp.insert(mq[l]);

                        if(tmp.size() < sep.size()) sep = tmp;
                    }

            return sep;
        }
            
};

// #endif 