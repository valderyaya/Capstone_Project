#include<vector>
#include<algorithm>

class Graph_CFS{
    public:
        std::vector<int> h, to, nxt, weigh, cur, dis;
        int cnt = 1;
        const int max_edge = 2e6 + 5;
        

        Graph_CFS(int n){
            h.resize(n+1);
            cur.resize(n+1);
            dis.resize(n+1);
            
            int nn=n*n;
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