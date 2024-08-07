// #ifdef DINIC_H
// #define DINIC_H
#pragma once

#include "Graph_CFS.h"
#include <queue>
#include <map>

class Dinic{
    public:
        Graph_CFS gph;
        int st, ed, n;
        const int inf = 2147483647;

        Dinic(Graph_CFS &g, int s, int e, int N):gph(g), st(s), ed(e), n(N){}

        bool bfs(){
            queue<int> q;
            q.emplace(st);
            for(int i = 1; i <= n; ++i)
                gph.dis[i] = 0, gph.cur[i] = gph.h[i];
            gph.dis[st] = 1;
            while(!q.empty()){
                int x = q.front();
                q.pop();
                for(int i = gph.h[x]; i; i = gph.nxt[i]){
                    int v = gph.to[i];
                    if(!gph.dis[v] && gph.weigh[i]){
                        q.emplace(v);
                        gph.dis[v] = gph.dis[x] + 1;
                    }
                }
            }
            return gph.dis[ed];
        }

        int dfs(int x, int flow){
            if(x == ed || flow == 0) return flow;
            int sum = 0, mn;
            for(int i = gph.cur[x]; i; i = gph.nxt[i]){
                int v = gph.to[i];
                gph.cur[x] = i;
                if(gph.dis[v] == gph.dis[x] + 1 && (mn = dfs(v, min(flow, gph.weigh[i])))){
                    gph.weigh[i] -= mn;
                    gph.weigh[i ^ 1] += mn;
                    sum += mn;
                    flow -= mn;
                    if(!flow) break;
                }
            }
            return sum;
        }

        int dinic(){
            int sum = 0;
            while(bfs())
                sum += dfs(st, inf);
            // cout << "Dinic: " << sum <<endl;
            return sum;
        }
        
        map<int, set<pair<int,int>>> cal(){
            dinic();
            map<int, set<pair<int,int>>> ret;
            for(int i = 1; i <= n; ++i){
                //ret[i] = set<pair<int,int>>();
                for(int j = gph.h[i]; j; j = gph.nxt[j])
                    ret[i].insert({gph.to[j], gph.weigh[j]});
            }
            return ret;
        }
};

// #endif 