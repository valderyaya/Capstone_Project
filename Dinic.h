#include "Graph_CFS.h"
#include <queue>

class Dinic{
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



// const int inf=2147483647;
// int cnt=1,n,m,s,t,d[205],h[205],cur[205];
// struct edge{int to,nxt,w;}e[10005];
// bitset<205> vis;
// void add(int u,int v,int w){
//     e[++cnt]=edge{v,h[u],w};
//     h[u]=cnt;
// }
// bool bfs(){
//     queue<int> q; q.emplace(s);
//     REP1(i,1,n) d[i]=0,cur[i]=h[i];
//     d[s]=1;
//     while(!q.empty()){
//         int x=q.front();q.pop();
//         for(int i=h[x];i;i=e[i].nxt){
//             int v=e[i].to;
//             if(!d[v]&&e[i].w) q.emplace(v),d[v]=d[x]+1;
//         }
//     }
//     return d[t];
// }
// ll dfs(int x,int flow){
//     if(x==t||flow==0) return flow;
//     int mi; ll sum=0;
//     for(int i=cur[x];i;i=e[i].nxt){
//         int v=e[i].to; cur[x]=i;
//         if(d[v]==d[x]+1&&(mi=dfs(v,min(flow,e[i].w)))){
//             e[i].w-=mi; e[i^1].w+=mi;
//             sum+=mi; flow-=mi;
//             if(!flow) break;
//         }
//     }return sum;
// }
// ll dinic(){
//     ll sum=0;
//     while(bfs()) sum+=dfs(s,inf);
//     return sum;
// }