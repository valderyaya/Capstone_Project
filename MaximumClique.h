#pragma once

#include<vector>
#include<unordered_set>

template<typename T>
class MaximumClique{
    public:
        int n, best;
        vector<unordered_set<T>> adjList;
        unordered_set<T> ans;
        vector<T> num;

        MaximumClique(const int N){
            n = N;
            best = 0;
            num.resize(n + 1);
            adjList.resize(n + 1);
        }

        void add_edge(int u, int v){
            adjList[u].insert(v);
            adjList[v].insert(u);
        }

        bool is_adjacent(int u, int v)const{
            return adjList[u].find(v) != adjList[u].end();
        }
        
        bool dfs(vector<int> &adj, int tot, int cnt, unordered_set<int> &s){ // need optimize , segmentation fault if n is large
            if(tot == 0){
                if(cnt > best){
                    ans = s;
                    return best = cnt, 1;
                }
                return 0;
            }
            for(int i = 0; i < tot; ++i){
                if(cnt + tot - i <= best) return 0;
                if(cnt + num[adj[i]] <= best) return 0;
                vector<T> t;
                for(int j = i + 1; j < tot; ++j)
                    if(is_adjacent(adj[i], adj[j]))
                        t.emplace_back(adj[j]);
                s.insert(adj[i]);
                if(dfs(t, t.size(), cnt + 1, s)) return 1;
                s.erase(adj[i]);
            }
            return 0;
        }

        int maximum_clique(){
            unordered_set<int> s;
            for(int i = n; i >= 1; --i){
                vector<T> v;
                for(int j = i + 1; j <= n; ++j)
                    if(is_adjacent(i, j))
                        v.emplace_back(j);
                s.insert(i);
                dfs(v, v.size(), 1, s);
                s.erase(i);
                num[i] = best;
            }
            return best;
        }
};