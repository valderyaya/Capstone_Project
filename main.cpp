#include<bits/stdc++.h>
using namespace std;
#include "Bag.h"
#include "Graph.h"

int main(){
    Graph<int> p, q(1);
    p.add_edge(1, 2);
    // p.add_vertex(3);

    cout <<  ' ' << p.m << ' ' << p.get_num_of_edges() << endl;


    cout << "finish\n";
    return 0;
}