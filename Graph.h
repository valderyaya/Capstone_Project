#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<set>

template<typename T>
class Graph{
    public:
        map<T, set<T>> adj; // adjacencies matrix
        //unordered_map<T, unordered_set<T>> adj; // adjacencies matrix
        //unordered_map<T, int> vertex_degree;  // log vertex degree, only for undirected graph?
        int m; // vertex and edge number
        bool isDirected;

        Graph(bool Directed = false): adj(), isDirected(Directed), m(0){}


        Graph(bool Directed, int n, int m): adj(), isDirected(Directed), m(m){}

        Graph(const Graph<T> &original){
            isDirected = original.isDirected;
            //logVertexDegree = original.logVertexDegree;
            adj = original.adj;
            //vertex_degree = original.vertex_degree;
            m = original.m;
        }

        unordered_set<T> get_copy_vertices(){
            unordered_set<T> ret;
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


        bool is_adjacent(T u, T v){ // work properly in undirected graph
            return adj[u].find(v) != adj[u].end();
        }


        void add_vertex(T v){
            if(adj.find(v) != adj.end()) return;
            adj[v];
        }


        void add_directed_edge(T u, T v){
            isDirected = 1;
            adj[u].insert(v);
        }

        void add_half_of_edge(T u, T v){
            add_vertex(u);
            add_vertex(v);
            if(adj[u].find(v) != adj[u].end()) return;
            ++m;
            adj[u].insert(v);
        }


        void add_edge(T u, T v){
            if(!isDirected && adj[u].find(v) != adj[u].end()) 
                return;
            
            add_half_of_edge(u, v);
            add_half_of_edge(v, u);
        
            // for(auto &i: adj[u])
            //     if(is_adjacent(i, v)){
            //         ++vertex_degree[i];
            //         ++vertex_degree[u];
            //         ++vertex_degree[v];
            //     }
        }


        void remove_directed_edge(T u, T v){
            isDirected = 1;
            adj[u].erase(v);
        }

        void remove_half_of_edge(T u, T v){
            --m;
            adj[u].erase(v);
        }

        void remove_edge(T u, T v){
            remove_half_of_edge(u, v);
            remove_half_of_edge(v, u);
        }

        int get_num_of_edges(){
            return m >> 1;
        }

        void remove_vertex(T v){
            if(isDirected){
                for(auto &it : adj)
                    if(it->first != v && it->second.find(v) != it->second.end())
                        it->second.erase(v);
            }else{
                for(auto &i : adj[v])
                    adj[i].erase(v);
            }
            adj.erase(v);
        }

        int get_num_of_vertices(){
            return adj.size();
        }

        // need to implement contractionInformantion
};