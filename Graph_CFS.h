// #ifdef GRAPH_CFS_H
// #define GRAPH_CFS_H
#pragma once
#include<vector>
#include<algorithm>

class Graph_CFS{
    public:
        std::vector<int> h, to, nxt, weigh, cur, dis;
        int cnt = 1;
        const int max_edge = 2e6 + 5;
        

        Graph_CFS(int n){
            h.resize(n+5);
            cur.resize(n+5);
            dis.resize(n+5);
            
            int nn=n*n+n+n;
            to.resize(std::min(nn, max_edge));
            nxt.resize(std::min(nn, max_edge));
            weigh.resize(std::min(nn, max_edge));
        } 

        void add_edge(int u, int v, int w){
            to[++cnt] = v;
            nxt[cnt] = h[u];
            weigh[cnt] = w;
            h[u] = cnt;
        }
};

// #endif