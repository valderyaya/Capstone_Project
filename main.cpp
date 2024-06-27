#include<bits/stdc++.h>
using namespace std;
//#include "Bag.h"
//#include "Graph.h"
#include<unordered_map>
#include<unordered_set>
#include<map>
#include<set>
//#include "TreeDecomposition.h"
//#include "NiceTreeDecomposition.h"
#include "ImproveTreeDecomposition.h"


int main(){
    // testing code
    // Bag<int> b1, b2;
    // b1.vertices.insert(1);
    // b1.id = 12;
    // b2.vertices.insert(2);

    // TreeDecomposition<int> o;
    // o.tree.add_edge(b1, b2);
    // cout<<o.tree.adj.size()<<endl;
    // cout<<*o.tree.adj.begin()->first.vertices.begin()<<endl;
    // NiceTreeDecomposition<int> n(o);

    // map<Bag<int>, set<Bag<int>>> mp;
    // mp[b1].insert(b2);
    // cout << n.treeDecomposition.tree.adj.begin()->first.id << endl;
    // b2 = n.findSuitableRoot();
    // cout<<b2.id<<endl;

    set<int> s1 = {2,3, 4}, s2={1,3,4}, s3;
    set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), inserter(s3, s3.begin()));
    for(auto &i:s3) cout<<i<<endl;
    cout<<endl;
    
    cout << "finish\n";
    return 0;
}