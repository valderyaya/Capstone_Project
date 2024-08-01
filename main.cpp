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
#include "Debug_func.h"
#include "DCSGQ.h"


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
    ori.add_edge(2, 5);
    ori.add_edge(6, 5);
    ori.add_edge(3, 6);
    
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
    set<int> s = {1, 2, 3, 4, 5, 6};
    td.create_Bag(s);
    ImproveTreeDecomposition itd(td);
    itd.improve_decomposition();
    // cout<< itd.treeDecomposition.isValid() << endl;
    // print_graph_bag(itd.treeDecomposition.tree);

    NiceTreeDecomposition ntd(itd.treeDecomposition);
    ntd.root = ntd.make_nice(ntd.findSuitableRoot());
    print_graph_bag(ntd.treeDecomposition.tree);
    // cout<< ntd.treeDecomposition.isValid() << endl;
    ntd.classify_bags();
    // print_bags_type(ntd);
    ntd.compute_tree_index();

    DCSGQ<int> solver(ntd);
    
    
    cout << "------------Finish--------------\n";
    return 0;
}

