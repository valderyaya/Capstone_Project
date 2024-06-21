#include<bits/stdc++.h>
using namespace std;
//#include "Bag.h"
//#include "Graph.h"
#include<unordered_map>
#include<unordered_set>
#include<map>
#include<set>
#include "TreeDecomposition.h"

int main(){
    // testing code
    Bag<int> a, b;
    a.vertices.insert(1);
    if(a != b) cout<<"no";
    b.vertices.insert(1);
    if(a==b) cout<<"yes";
    cout << "finish\n";
    return 0;
}