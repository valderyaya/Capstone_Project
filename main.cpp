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
#include "MaximumClique.h"
// #include "PartialTreeDecomposition.h"


// int main(){ // for partial tree decomposition
//     Graph<int> ori;
//     ori.add_edge(1, 2);
//     ori.add_edge(1, 3);
//     ori.add_edge(1, 5);
//     ori.add_edge(2, 3);
//     ori.add_edge(2, 4);
//     ori.add_edge(2, 5);
//     ori.add_edge(3, 4);
//     ori.add_edge(3, 5);
//     ori.add_edge(4, 5);

//     TreeDecomposition<int> td(ori);

//     PartialTreeDecomposition<int> ptd(td);
//     ptd.build_partial_treedecomposition();
// }

int main(){
    cout << "------------Start--------------\n";

    freopen("testcase6.txt", "r", stdin);
    // ori.add_edge(1, 2);
    // ori.add_edge(2, 3);
    // ori.add_edge(2, 7);
    // ori.add_edge(2, 8);
    // ori.add_edge(3, 6);
    // ori.add_edge(3, 5);
    // ori.add_edge(5, 6);
    // ori.add_edge(3, 4);
    // ori.add_edge(3, 8);

    // ori.add_edge(1, 2);
    // ori.add_edge(2, 3);
    // ori.add_edge(2, 7);
    // ori.add_edge(2, 8);
    // ori.add_edge(3, 6);
    // ori.add_edge(3, 5);
    // ori.add_edge(5, 6);
    // ori.add_edge(3, 4);
    // ori.add_edge(3, 8);
    
    
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

    int n, m;
    vector<int> UpBound, LowBound, weight;
    map<pair<int, int>, int> edge_weight;
    Graph<int> ori;
    cin >> n >> m;
    UpBound.resize(n + 1);
    LowBound.resize(n + 1);
    weight.resize(n + 1);
    for(int i = 1; i <= n; ++i) cin >> weight[i];
    for(int i = 1; i <= n; ++i) cin >> UpBound[i];
    for(int i = 1; i <= n; ++i) cin >> LowBound[i];
    for(int x, y, z; m--;){
        cin >> x >> y >> z;
        ori.add_edge(x, y);
        edge_weight[{x, y}] = z;
        edge_weight[{y, x}] = z;
    }
    TreeDecomposition<int> td(ori);
    set<int> s;
    for(int i = 1; i <= n; ++i) s.insert(i);
    td.create_Bag(s);
    ImproveTreeDecomposition itd(td);
    itd.improve_decomposition();
    // cout<< itd.treeDecomposition.isValid() << endl;
    // print_graph_bag(itd.treeDecomposition.tree);

    NiceTreeDecomposition ntd(itd.treeDecomposition);
    ntd.root = ntd.make_nice(ntd.findSuitableRoot());
    print_graph_bag(ntd.treeDecomposition.tree);
    ntd.classify_bags();
    // cout<< ntd.treeDecomposition.isValid() << endl;
    // print_bags_type(ntd);
    // ntd.compute_tree_index();
    
    DCSGQ<int> solver(ntd);
    solver.edge_weight = edge_weight;
    solver.n = n;
    solver.UpBound = UpBound;
    solver.LowBound = LowBound;
    solver.weight = weight;
    solver.initialization();
    solver.solve();
    
    cout << "------------Finish--------------\n";
    return 0;
}


// int main(){ // for maximum clique
//     cout << "start---" << endl;
//     MaximumClique<int> solver(5);
//     solver.add_edge(1, 2);
//     solver.add_edge(1, 3);
//     solver.add_edge(1, 5);
//     solver.add_edge(2, 3);
//     solver.add_edge(2, 4);
//     // solver.add_edge(2, 5);
//     solver.add_edge(3, 4);
//     solver.add_edge(3, 5);
//     solver.add_edge(4, 5);
    
//     cout << solver.maximum_clique() << endl;
//     for(auto &i:solver.ans) cout << i << ' ';

//     return 0;
// }