// #ifdef MINIMALSEPARATOR_H
// #define MINIMALSEPARATOR_H
#pragma once

#include "Graph.h"
#include<unordered_set>
#include<unordered_map>
#include<vector>
#include"Dinic.h"

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

            Graph_CFS g(2*n);
            // initialize graph
            for(auto &i: v){
                int mu = mp[i];
                g.add_edge(mu, mu + n, inf);
                g.add_edge(mu + n, mu, 0);
                for(auto &j : v){
                    if(graph.is_adjacent(i, j)){
                        int mv = mp[j];
                        g.add_edge(mu + n, mv, 1);
                        g.add_edge(mv, mu + n, 0);
                        // g.add_edge(mv + n, mu, inf);
                        // g.add_edge(mu, mv + n, 0);
                    }
                }
            }

            for(int i = 1; i <= n; ++i)
                for(int j = i + 1; j <= n; ++j)
                    if(!graph.is_adjacent(mq[i], mq[j])){
                        // cout << mq[i] << ' ' << mq[j] <<endl;
                        Dinic dc(g, i, j + n, 2*n + 1);
                        map<int, set<pair<int,int>>> another_g = dc.cal();

                        set<T> tmp;
                        for(int k = 1; k <= n; ++k)
                            if(k != i && k != j){
                                // check
                                for(auto &[x, y] : another_g[k + n])
                                    if(x == k && y){
                                        tmp.insert(mq[k]);
                                        break;
                                    }
                            }
                        if(tmp.size() < sep.size()) sep = tmp;
                    }
            return sep;
        }
            
};

// #endif 