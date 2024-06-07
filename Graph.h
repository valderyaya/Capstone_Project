#include<vector>
#include<unordered_map>
#include<unordered_set>

template<typename T>
class Graph{
    public:
        unordered_map<T, unordered_set<T>> adj; // adjacencies matrix
        unordered_map<T, int> vertex_degree;  // log vertex degree, only for undirected graph?
        int n, m; // vertex and edge number
        bool logVertexDegree, isDirected;

        Graph(bool logEdge, bool Directed): adj(), edgesInNeighborhood(), logEdgesInNeighbourhood(logEdge), isDirected(Directed){}

        Graph(Graph<T> original){
            isDirected = original.isDirected;
            logVertexDegree = original.logVertexDegree;
            adj = original.adj;
            vertex_degree = original.vertex_degree;
            n = original.n;
            m = original.m;
        }


        unordered_set<T> get_copy_vertices(){
            unordered_set ret;
            for(auto &i: adj) ret.insert(i.first);
            return ret;
        }


        bool containsNode(T v){
            return adj.find(v) != adj.end();
        }


        int get_max_degree(){
            int ret = 0;
            for(auto &i : adj)
                if((int)i.second.size() > ret)
                    ret = (int)i.second.size();
            return ret;
        }

        // vector<T> get_neighbourhood_as_vector(T v){}


        bool is_adjacent(T u, T v){
            return adj[u].find(v) != adj[u].end();
        }


        void add_vertex(T v){
            if(!adj[v]) vertex_degree[v] = 0;
        }


        void add_directed_edge(T u, T v){
            ++m;
            adj[u].insert(v); // may not consider multi edge
        }


        void add_edge(T u, T v){
            if(adj[u].find(v) != adj.end()) 
                return;
            
            add_directed_edge(u, v);
            add_directed_edge(v, u);
        
            for(auto &i: adj[u])
                if(is_adjacent(i, v)){
                    ++vertex_degree[i];
                    ++vertex_degree[u];
                    ++vertex_degree[v];
                }
        }


        void remove_edge(T u, T v){
            
        }


        void remove_vertex(T v){
            for(auto )
        }
};