#include "Graph.h"
#include "Bag.h"
#include<stack>
#include<set>
#include<queue>

template<typename T>
class TreeDecomposition{
    public:
        int numOfBags;
        int width;
        int n; // size of original graph
        Graph<Bag<T>> tree; // The tree of the tree-decomposition
        Graph<T> graph; // The original graph

        TreeDecomposition():numOfBags(0), width(-1), n(0), tree(), graph(){}

        TreeDecomposition(const Graph<T> &graph){
            graph = graph;
            tree = Graph<Bag<T>>();
            numOfBags = 0;
            width = -1;
            n = graph.get_num_of_vertices();
        }

        int get_width(){
            return width;
        }

        /**
         * Add an edge to the tree. This will _not_ add the two bags, if the corresponding nodes
         * are not already part of the decomposition. In this, the edge is not added.
         */
        void add_tree_edge(Bag<T> bi, Bag<T> bj){
            if(bi.isEmpty() || bj.isEmpty()) return;
            // if(!tree.containsNode(bi)) return;
            // if(!tree.containsNode(bj)) return;
            if(tree.adj[bi].find(bj) != tree.adj[bi].end()) return;
            tree.add_edge(bi, bj);
        }

        Bag<T> create_Bag(const set<T> &vertices){
            Bag<T> bag(vertices, 1 + numOfBags++);
            //tree.add_vertex(bag);
            if((int)vertices.size() > width) width = (int)vertices.size();
            return bag;
        }

        stack<Bag<T>> connectedComponents(T x){
            stack<Bag<T>> ret;
            set<Bag<T>> vis;
            for(auto &b : tree){
                if(!vis.count(b) || b.contains(x)) continue;
                ret.emplace(b);
                vis.insert(b);

                stack<Bag<T>> s;
                s.emplace(b);
                while(!s.empty()){
                    auto v = s.top(); 
                    s.pop();
                    for(auto &w : tree.adj[v]){
                        if(!vis.count(w) && w.contains(x)){
                            vis.insert(w);
                            s.emplace(w);
                        }
                    }
                }
            }
            return ret;
        }
        
        bool isValid(){
            // every vertex is in a bag
            for(auto &v : graph){
                bool contained = 0;
                for(auto &b : tree)
                    if(b.contains(v)){
                        contained = 1;
                        break;
                    }
                if(!contained){
                    cerr << "Vertex " << v << " not in bag\n";
                    return 0;
                }
            }

            // every edge is in a bag
            for(auto &v : graph){
                for(auto &w : graph.adj[v]){
                    if(v.compareTo(w) >= 0) continue;
                    bool contained = 0;
                    for(auto &b : tree)
                        if(b.contains(v) && b.contains(w)){
                            contained = 1;
                            break;
                        }
                    if(!contained){
                        cerr << "Edge " << v << ", " << w << " not in bag\n";
                        return 0;
                    }
                }
            }

            // subtrees connected
            for(auto &v : graph){
                stack<Bag<T>> s = connectedComponents(v);
                if(s.size() != 1){
                    cerr << "Vertex " << v << "is not connected\n";
                    return 0;
                }
            }

            return 1;
        }

        void connect_Components(){
            vector<set<Bag<T>>> comps = tree.get_connected_components();
            vector<Bag<T>> v;
            for(auto &i : comps)
                for(auto &j : i)
                    v.emplace_back(j);
            for(int i = 0; i < v.size() - 1; ++i)
                add_tree_edge(v[i], v[i+1]);
        }


        void contractDuplicateBags(){ // from FlattenTreeDecomposition.h
            Graph<Bag<T>> new_tree = tree;
            queue<Bag<T>> q;
            for(auto &v: new_tree.adj) q.emplace(v->first);
            set<Bag<T>> removed;
            while(!q.empty()){
                Bag<T> v = q.front();
                q.pop();
                if(removed.count(v)) continue;
                bool flag = 1;
                while(flag){
                    flag = 0;
                    for(Bag<T> &w : new_tree.adj[v]){
                        if(v.containsAll(w.vertices)){
                            new_tree.contract(v, w);
                            removed.insert(w);
                            flag = 1;
                            break;
                        }
                    }
                }
            }
            
            // need to renumber ?
            // .........
            
            tree = new_tree;
        }

        void renumber(){
            numOfBags = tree.adj.size();
            map<Bag<T>, int> mp;
            int id = 0;
            for(auto &[i, j] : tree.adj)
                if(!mp.count(i)) mp[i] = ++id;
            for(auto &[i, j] : tree.adj){
                i.id = mp[i];
                for(Bag<T> &w : j) w->id = mp[w];
            }
        }
};