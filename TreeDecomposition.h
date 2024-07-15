// #ifdef TREEDECOMPOSITION_H
// #define TREEDECOMPOSITION_H
#pragma once

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

        TreeDecomposition(const Graph<T> &grh){
            graph = grh;
            tree = Graph<Bag<T>>();
            numOfBags = 0;
            width = -1;
            n = grh.get_num_of_vertices();
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
            tree.add_vertex(bag);
            if((int)vertices.size() > width) width = (int)vertices.size();
            return bag;
        }

        stack<Bag<T>> connectedComponents(T x){
            stack<Bag<T>> ret;
            set<Bag<T>> vis;
            for(auto b = tree.adj.begin(); b != tree.adj.end(); ++b){
                if(vis.count(b->first) || !b->first.contains(x)) continue;
                ret.emplace(b->first);
                vis.insert(b->first);

                stack<Bag<T>> s;
                s.emplace(b->first);
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
            for(auto it = graph.adj.begin(); it != graph.adj.end(); ++it){
                bool contained = 0;
                for(auto jt = tree.adj.begin(); jt != tree.adj.end(); ++jt)
                    if(jt->first.contains(it->first)){
                        contained = 1;
                        break;
                    }
                if(!contained){
                    cerr << "Vertex "<<it->first<<" not in bag\n";
                    return 0;
                }
            }

            // every edge is in a bag
            for(auto it = graph.adj.begin(); it != graph.adj.end(); ++it){
                for(auto &w : it->second){
                    if(it->first >= w) continue;
                    bool contained = 0;
                    for(auto b = tree.adj.begin(); b != tree.adj.end(); ++b)
                        if(b->first.contains(it->first) && b->first.contains(w)){
                            contained = 1;
                            break;
                        }
                    if(!contained){
                        cerr << "Edge "<<it->first<<' '<<w<<" not in bag\n";
                        return 0;
                    }
                }
            }

            // subtrees connected
            for(auto it = graph.adj.begin(); it != graph.adj.end(); ++it){
                stack<Bag<T>> s = connectedComponents(it->first);
                if(s.size() != 1){
                    cerr << "Vertex " << it->first<< " is not connected\n";
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
            Graph<Bag<T>> g;
            for(auto it = tree.adj.begin(); it != tree.adj.end(); ++it){
                mp[it->first] = ++id;
                g.add_vertex(Bag<T>(it->first.vertices, id));
            }
            
            for(auto it = tree.adj.begin(); it != tree.adj.end(); ++it){
                Bag<T> u(it->first.vertices, mp[it->first]);
                for(auto &w : it->second){
                    Bag<T> v(w.vertices, mp[w]);
                    g.add_edge(u, v);
                }
            }
            tree = g;
        }
};

// #endif 