// #ifdef NICETREEDECOMPOSITION_H
// #define NICETREEDECOMPOSITION_H
#pragma once

#include "TreeDecomposition.h"
//#include<map>

template<typename T>
class NiceTreeDecomposition{
    public:
        enum class BagType{
            LEAF,
            INTRODUCE,
            FORGET,
            JOIN,
            EDGE
        };

        TreeDecomposition<T> treeDecomposition;
        map<T, int> treeIndex;
        map<Bag<T>, BagType> bagType;
        map<Bag<T>, T> specialVertex;
        map<Bag<T>, T> secondSpecialVertex;
        Bag<T> root;
        map<Bag<T>, vector<Bag<T>>> childrenBag;
        bool veryNice;

        NiceTreeDecomposition(const TreeDecomposition<T> &orignal){
            treeDecomposition = orignal;
        }

        NiceTreeDecomposition(const TreeDecomposition<T> &orignal, bool verynice){
            treeDecomposition = orignal;
            veryNice = verynice;
        }


        Bag<T> findSuitableRoot(){
            return treeDecomposition.tree.adj.begin()->first;
        }

        Bag<T> make_nice(Bag<T> suitableRoot){
            //Graph<Bag<T>> tree = &treeDecomposition.tree;
            Bag<T> rt = treeDecomposition.create_Bag(set<T>());
            treeDecomposition.tree.add_vertex(rt);
            treeDecomposition.tree.add_edge(rt, suitableRoot);

            stack<Bag<T>> st;
            set<Bag<T>> vis;
            st.push(rt);
            // int c = 0;
            while(!st.empty()){
                // cout << ++c <<endl;
                Bag<T> v =st.top();
                st.pop();
                vis.insert(v);
                
                int cnt = 0;
                Bag<T> ww;
                for(auto &i : treeDecomposition.tree.adj[v])
                    if(!vis.count(i)){
                        ++cnt;
                        ww = i;
                    }
                if(cnt == 0 && !v.vertices.empty()){
                    Bag<T> leaf = treeDecomposition.create_Bag(set<T>());
                    treeDecomposition.tree.add_vertex(leaf);
                    treeDecomposition.tree.add_edge(v, leaf);
                    st.push(v);
                }else if(cnt == 1){
                    if(v.vertices == ww.vertices){
                        treeDecomposition.tree.contract(v, ww);
                        st.push(v);
                        continue;
                    }
                    set<T> newBag, diff;
                    set_difference(v.vertices.begin(), v.vertices.end(), ww.vertices.begin(), ww.vertices.end(), inserter(diff, diff.end()));
                    if(diff.size() > 0){
                        newBag = v.vertices;
                        newBag.erase(*diff.begin());
                    }else{
                        diff.clear();
                        set_difference(ww.vertices.begin(), ww.vertices.end(), v.vertices.begin(), v.vertices.end(), inserter(diff, diff.end()));
                        newBag = v.vertices;
                        newBag.insert(*diff.begin());
                    }
                    Bag<T> NEWBag = treeDecomposition.create_Bag(newBag);
                    treeDecomposition.tree.remove_edge(v, ww);
                    treeDecomposition.tree.add_vertex(NEWBag);
                    treeDecomposition.tree.add_edge(v, NEWBag);
                    treeDecomposition.tree.add_edge(NEWBag, ww);
                    st.push(NEWBag);
                }else if(cnt >= 2){
                    Bag<T> left = treeDecomposition.create_Bag(v.vertices);
                    Bag<T> right = treeDecomposition.create_Bag(v.vertices);
                    set<Bag<T>> neighbors;
                    for(auto &i: treeDecomposition.tree.adj[v])
                        if(!vis.count(i)) neighbors.insert(i);
                    for(Bag<T> w : neighbors) 
                        treeDecomposition.tree.remove_edge(v, w);
                    treeDecomposition.tree.add_edge(v, left);
                    treeDecomposition.tree.add_edge(v, right);
                    int i = 0;
                    for(Bag<T> w: neighbors){
                        if(i < neighbors.size() / 2)
                            treeDecomposition.tree.add_edge(left, w);
                        else
                            treeDecomposition.tree.add_edge(right, w);
                        ++i;
                    }
                    st.push(left);
                    st.push(right);
                }
            }
            vector<Bag<T>> empt;
            for(auto it = treeDecomposition.tree.adj.begin(); it != treeDecomposition.tree.adj.end(); ++it) 
                if(it->first.vertices.empty() && it->first != rt) 
                    empt.emplace_back(it->first);
            for(auto &i : empt) treeDecomposition.tree.remove_vertex(i);
            treeDecomposition.renumber();
            return treeDecomposition.tree.adj.begin()->first;
            // return rt;
        }

        void classify_bags(){
            stack<Bag<T>> st;
            set<Bag<T>> vis;
            st.push(root);
            vis.insert(root);

            while(!st.empty()){
                Bag<T> v = st.top();
                st.pop();
                vector<Bag<T>> children;

                if(treeDecomposition.tree.adj[v].size() == 3){
                    bagType[v] = BagType::JOIN;
                    for(Bag<T> w : treeDecomposition.tree.adj[v])
                        if(!vis.count(w)) children.emplace_back(w);
                }
                for(Bag<T> w : treeDecomposition.tree.adj[v]){
                    if(vis.count(w)) continue;
                    if(bagType.find(v) == bagType.end()){
                        children.emplace_back(w);
                        set<T> diff;
                        set_difference(v.vertices.begin(), v.vertices.end(), w.vertices.begin(), w.vertices.end(), inserter(diff, diff.end()));
                        if(diff.size() == 1){
                            bagType[v] = BagType::INTRODUCE;
                            specialVertex[v] = *diff.begin();
                        }else{
                            diff.clear();
                            set_difference(w.vertices.begin(), w.vertices.end(), v.vertices.begin(), v.vertices.end(), inserter(diff, diff.end()));
                            bagType[v] = BagType::FORGET;
                            specialVertex[v] = *diff.begin();
                        }
                    }
                    vis.insert(w);
                    st.push(w);
                }
                if(bagType.find(v) == bagType.end()){
                    bagType[v] = BagType::LEAF;
                    specialVertex[v] = *v.vertices.begin();
                }
                childrenBag[v] = children;
            }
        }

        void compute_tree_index(){
            stack<int> index;
            int d = treeDecomposition.get_width() + 1;
            while(d-- > 0) index.push(d);

            stack<Bag<T>> st;
            set<Bag<T>> vis, finish;
            st.push(root);
            vis.insert(root);
            while(!st.empty()){
                Bag<T> v = st.top();
                if(finish.count(v)){
                    st.pop();
                    if(bagType[v] == BagType::FORGET)
                        index.push(treeIndex[specialVertex[v]]);
                    else if(bagType[v] == BagType::INTRODUCE)
                        index.pop();
                    continue;
                }
                finish.insert(v);
                if(bagType[v] == BagType::FORGET){
                    treeIndex[specialVertex[v]] = index.top();
                    index.pop();
                }else if(bagType[v] == BagType::INTRODUCE)
                    index.push(treeIndex[specialVertex[v]]);
                
                for(Bag<T> w : treeDecomposition.tree.adj[v]){
                    if(vis.count(w)) continue;
                    st.push(w);
                    vis.insert(w);
                }
            }
        }

        void compute_edge_bags(){
            stack<Bag<T>> st;
            set<Bag<T>> vis;
            st.emplace(root);
            vis.insert(root);

            while(!st.empty()){
                Bag<T> v = st.top();
                st.pop();
                if(bagType[v] == BagType::FORGET){
                    T x = specialVertex[v];
                    Bag<T> child;
                    for(auto &w : treeDecomposition.tree.adj[v])
                        if(!vis.count(w)){
                            child = w;
                            break;
                        }
                    
                    for(T &u : treeDecomposition.graph.adj[x]){
                        if(child.contains(u)){
                            Bag<T> newChild = treeDecomposition.create_Bag(child.vertices);
                            bagType[newChild] = BagType::EDGE;
                            secondSpecialVertex[newChild] = x;
                            secondSpecialVertex[newChild] = u;
                            treeDecomposition.add_tree_edge(child, newChild);
                            treeDecomposition.add_tree_edge(newChild, v);
                            treeDecomposition.tree.remove_edge(child, v);
                            child = newChild;
                        }
                    }
                }
                for(Bag<T> w : treeDecomposition.tree.adj[v])
                    if(!vis.count(w)){
                        vis.insert(w);
                        st.push(w);
                    }
            }
        }

        Bag<T> get_root(){
            return root;
        }
};

// #endif 