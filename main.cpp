#include<bits/stdc++.h>
using namespace std;
#include "Bag.h"
#include "Graph.h"
#include<unordered_map>
#include<unordered_set>
#include<map>
#include<set>
#include "TreeDecomposition.h"
#include "NiceTreeDecomposition.h"
#include "ImproveTreeDecomposition.h"
#include "Graph_CFS.h"
#include "Dinic.h"


void print_graph_CFS(Graph_CFS &g){
    int n = g.h.size();
    for(int i = 0; i < n; ++i){
        cout << i << ": ";
        for(int j = g.h[i]; j; j = g.nxt[j])
            cout << g.to[j] << ' ';
        cout << "\n";
    }
}

void print_graph_int(Graph<int> &g){
    for(auto &[x, y] : g.adj){
        cout << x << ": ";
        for(auto &k : y) cout << k << ' ';
        cout << "\n";
    }
}

void print_graph_bag(Graph<Bag<int>> &g){
    int c = 0;
    for(auto &[x, y] : g.adj){
        cout << x.id << ": {";
        for(auto &i : x.vertices) cout << i <<' ';
        cout << "}\n" ;
    }
    for(auto &[x, y] : g.adj){
        cout << x.id << ": ";
        for(auto &z : y) cout << z.id << ' ';
        cout << "\n";
    }
}

int main(){
    cout << "------------Start--------------\n";

    Graph<int> ori;
    // ori.add_edge(1, 2);
    // ori.add_edge(2, 3);
    // ori.add_edge(2, 7);
    // ori.add_edge(2, 8);
    // ori.add_edge(3, 6);
    // ori.add_edge(3, 5);
    // ori.add_edge(5, 6);
    // ori.add_edge(3, 4);
    // ori.add_edge(3, 8);

    ori.add_edge(1, 2);
    ori.add_edge(2, 3);
    ori.add_edge(3, 4);
    ori.add_edge(1, 4);
    // ori.add_edge(2, 5);
    // ori.add_edge(6, 5);
    // ori.add_edge(3, 6);
    
    // ori.add_edge(1, 2);
    // ori.add_edge(2, 4);
    // ori.add_edge(1, 3);
    // ori.add_edge(3, 2);
    // ori.add_edge(5, 2);
    // ori.add_edge(4, 5);
    // ori.add_edge(7, 2);
    // ori.add_edge(3, 7);
    // ori.add_edge(3, 6);
    // ori.add_edge(6, 7);
    // ori.add_edge(5, 7);
    // ori.add_edge(5, 8);
    // ori.add_edge(7, 8);

    // print_graph_int(ori);

    TreeDecomposition<int> td(ori);
    // set<int> s = {1, 2, 3, 4, 5, 6, 7, 8};
    set<int> s = {1, 2, 3, 4};
    td.create_Bag(s);
    ImproveTreeDecomposition itd(td);
    itd.improve_decomposition();
    //print_graph_bag(itd.treeDecomposition.tree);

    NiceTreeDecomposition ntd(itd.treeDecomposition);
    ntd.root = ntd.make_nice(ntd.findSuitableRoot());
    print_graph_bag(ntd.treeDecomposition.tree);

    // check renumber

    cout << "------------Finish--------------\n";
    return 0;
}
