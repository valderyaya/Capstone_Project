#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
using namespace std;

// 前向声明 DFS 类
class DFS;

class Dinic {
private:
    vector<vector<int>> g;
    vector<vector<int>> f;
    int s, t;

public:
    Dinic(const vector<vector<int>>& graph, int source, int sink)
        : g(graph), f(graph.size(), vector<int>(graph.size(), 0)), s(source), t(sink) {}

    vector<vector<bool>> start() {
        vector<vector<int>> ncf = improve();
        while (!ncf.empty()) {
            augment(ncf);
            ncf = improve();
        }
        return toSet();
    }

private:
    vector<vector<bool>> toSet() {
        vector<vector<bool>> s(g.size(), vector<bool>(g.size(), false));
        for (size_t i = 0; i < g.size(); i++) {
            for (size_t j = 0; j < g.size(); j++) {
                s[i][j] = (f[i][j] == g[i][j] && g[i][j] > 0 && g[i][j] < numeric_limits<int>::max());
            }
        }
        return s;
    }

    void augment(const vector<vector<int>>& cf) {
        for (size_t i = 0; i < g.size(); i++) {
            for (size_t j = 0; j < g.size(); j++) {
                f[i][j] = g[i][j] - cf[i][j];
            }
        }
    }

    vector<vector<int>> improve();  // 声明，实现移到类外

    vector<int> revDijkstra(int t, const vector<vector<int>>& cf) {
        vector<int> distances(g.size(), numeric_limits<int>::max());
        distances[t] = 0;

        queue<int> queue;
        queue.push(t);
        while (!queue.empty()) {
            int u = queue.front();
            queue.pop();
            for (size_t i = 0; i < g.size(); i++) {
                if (g[i][u] > 0 && cf[i][u] > 0) {
                    int d = distances[u] + 1;
                    if (d < distances[i]) {
                        distances[i] = d;
                        queue.push(i);
                    }
                }
            }
        }
        return distances;
    }
};

class DFS {
private:
    vector<vector<int>>& g;
    vector<vector<int>>& cf;
    vector<int>& distances;
    vector<int> prev;
    int end;

public:
    DFS(vector<vector<int>>& g, vector<vector<int>>& cf, vector<int>& distances)
        : g(g), cf(cf), distances(distances), prev(g.size(), -1) {}

    vector<vector<int>>& start(int s, int t) {
        end = t;
        return dfs(s);
    }

private:
    vector<vector<int>>& dfs(int x) {
        if (x == end) {
            int delta = delta_path();
            int act = x;
            int pre = prev[x];
            while (pre > -1) {
                cf[pre][act] -= delta;
                cf[act][pre] += delta;

                if (cf[pre][act] == 0) {
                    x = pre;
                }
                act = pre;
                pre = prev[pre];
            }
        }

        for (size_t i = 0; i < g.size(); i++) {
            if (g[x][i] > 0 && cf[x][i] != 0 && distances[x] == distances[i] + 1) {
                prev[i] = x;
                dfs(i);
            }
        }
        return cf;
    }

    int delta_path() {
        int act = end;
        int prev_node = prev[act];

        int delta = numeric_limits<int>::max();
        while (prev_node > -1) {
            int upd = cf[prev_node][act];
            if (delta > upd) {
                delta = upd;
            }
            act = prev_node;
            prev_node = prev[act];
        }
        return delta;
    }
};

// Dinic::improve() 的实现移到这里
vector<vector<int>> Dinic::improve() {
    vector<vector<int>> cf(g.size(), vector<int>(g.size()));
    for (size_t i = 0; i < g.size(); i++) {
        for (size_t j = 0; j < g.size(); j++) {
            cf[i][j] = g[i][j] - f[i][j];
        }
    }

    vector<int> distances = revDijkstra(t, cf);

    if (distances[s] == numeric_limits<int>::max()) {
        return vector<vector<int>>();
    } else {
        DFS dfs(g, cf, distances);
        dfs.start(s, t);
        return cf;
    }
}