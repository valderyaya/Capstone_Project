// #ifdef DINIC_H
// #define DINIC_H
#pragma once

#include "Graph_CFS.h"
#include <queue>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>
using namespace std;

class Dinic {
private:
    const int inf = 1e9;
    int n, st, en;
    vector<vector<int>> start;
    vector<bool> del;
    set<int> path;
    struct G {
        int v, cap, next;
        G() {}
        G(int v, int cap, int next) : v(v), cap(cap), next(next) {}
    };
    vector<G> E;
    vector<int> p, d, temp_p;

    void init() {
        p.assign(2*n+2, -1);
    }

    void add(int u, int v, int cap) {
        E.push_back(G(v, cap, p[u]));
        p[u] = E.size() - 1;
        E.push_back(G(u, 0, p[v]));
        p[v] = E.size() - 1;
    }

    bool bfs(int st, int en, int n) {
        d.assign(2*n+2, -1);
        d[st] = 0;
        queue<int> qw;
        qw.push(st);
        while(!qw.empty()) {
            int u = qw.front(); qw.pop();
            for(int i = p[u]; i != -1; i = E[i].next) {
                int v = E[i].v;
                if(d[v] == -1 && E[i].cap > 0) {
                    d[v] = d[u] + 1;
                    qw.push(v);
                }
            }
        }
        return (d[en] != -1);
    }

    int dfs(int u, int en, int f) {
        if(u == en || f == 0) return f;
        int flow = 0, temp;
        for(int& i = temp_p[u]; i != -1; i = E[i].next) {
            G& e = E[i];
            if(d[u] + 1 == d[e.v]) {
                temp = dfs(e.v, en, min(f, e.cap));
                if(temp > 0) {
                    e.cap -= temp;
                    E[i ^ 1].cap += temp;
                    flow += temp;
                    f -= temp;
                    if(f == 0) break;
                }
            }
        }
        return flow;
    }

    void build() {
        init();
        for(int i=1; i<=n; i++) {
            if(!del[i]) add(i,i+n,1);
        }
        for(int i=1; i<=n; i++)
            for(int j=1; j<=n; j++)
                if(start[i][j])
                    add(i+n, j, inf);
    }

    int dinic(int st, int en, int n) {
        int ans = 0;
        while(bfs(st, en, n)) {
            temp_p = p;
            ans += dfs(st, en, inf);
        }
        return ans;
    }

public:
    Dinic(int n, int st, int en) : n(n), st(st), en(en) {
        start.resize(n+1, vector<int>(n+1));
        del.resize(n+1, false);
        p.resize(2*n+2);
        d.resize(2*n+2);
        temp_p.resize(2*n+2);
        E.reserve(4*n*n);
    }

    void setGraph(int x, int y) {
        start[x][y] = 1;
    }

    int getGraph(int x, int y){return start[x][y];}

    set<int> solve() {
        st = st+n;
        build();
        int ans = dinic(st, en, 2*n+1);
        // cout << ans << endl;
        for(int i=1; i<=n; i++) {
            if(i==st-n||i==en) continue;
            del[i] = true;
            build();
            int t = dinic(st, en, 2*n+1);
            // cout << i << ' ' << t << endl;
            if(t < ans) {
                ans--;
                path.insert(i);
                if(ans==0) break;
            } else
                del[i] = false;
        }
        return path;
    }
};