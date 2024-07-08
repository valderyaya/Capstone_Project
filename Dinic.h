#include "Graph_CFS.h"


class Dinic{
    Graph_CFS gph;
    int st, ed;

    Dinic(Graph_CFS &g, int s, int e):gph(g), st(s), ed(e){}

    
};



// //#pragma GCC optimize("Ofast,unroll-loops,no-stack-protector,fast-math")
// #include<bits/stdc++.h>
// //#include<bits/extc++.h>
// //using namespace __gnu_pbds;
// using namespace std;
// typedef long long ll;
// #define F first
// #define S second
// #define REP(i,n) for(int i=0;i<(n);++i)
// #define REP1(i,a,b) for(int i=(a);i<=(b);++i)
// #define em emplace_back
// #define ALL(x) (x).begin(),(x).end()
// #define mkp make_pair
// #define pii pair<int,int>
// #define pll pair<ll,ll>
// #define Rosario ios::sync_with_stdio(0),cin.tie(0),cout.tie(0);
// //#define lb(x) (x&-x)
// #define wait system("pause");

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
// int main(){//Rosario
//     cin>>n>>m>>s>>t;  int a,b,c;
//     REP(i,m) cin>>a>>b>>c,add(a,b,c),add(b,a,0);
//     cout<<dinic()<<"\n";
// }
