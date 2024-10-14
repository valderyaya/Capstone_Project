// #ifdef MINIMALSEPARATOR_H
// #define MINIMALSEPARATOR_H
#pragma once

#include "Graph.h"
#include<unordered_set>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include"Dinic_3.h"

template<typename T>
class MinimalSeparator{
    public:
        Graph<T> graph;
        const int inf = 2147483647;
        int N;

        MinimalSeparator(const Graph<T> &g):graph(g) {N = g.adj.size();}

        set<T> compute(){
            set<T> sep;
            int n = N, c = 0;
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


            for(int st = 1; st <= n; ++st)
                for(int ed = st + 1; ed <= n; ++ed)
                    if(!graph.is_adjacent(mq[st], mq[ed])){
                        // cout << mq[st] << ' ' << mq[ed] << endl;
                        Dinic dinic(n, st, ed);
                        for(auto it = graph.adj.begin(); it != graph.adj.end(); ++it){
                            int x = it->first;
                            for(auto &u: it->second)
                                dinic.setGraph(mp[x], mp[u]);
                        }
                        // for(int i=1;i<=n;++i)
                        //     for(int j=1;j<=n;++j)
                        //         cout << dinic.getGraph(i,j) << " \n"[j==n];
                        set<int> tsep = dinic.solve();
                        if(tsep.size() < sep.size()){
                            sep.clear();
                            for(auto &i: tsep)
                                sep.insert(mq[i]);
                        }
                    }
                
            return sep;
        }
            
};

// #endif 