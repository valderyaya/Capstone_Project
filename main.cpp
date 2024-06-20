#include<bits/stdc++.h>
using namespace std;
#include "Bag.h"
//#include "Graph.h"
#include<unordered_map>
#include<unordered_set>
#include<map>
#include<set>
#include "TreeDecomposition.h"

int main(){
    // testing code
set<int> s;
s.insert(1);
s.insert(2);
s.insert(3);

set<int> a;
a.insert(1);
a.insert(5);

Bag<int> test = Bag<int>(s, 2);
cout << test.containsAll(a) << ' ' << test.containsAll(s) << endl;
cout<<test.id<<endl;

    TreeDecomposition<Bag<int>> t;
    
    map<set<int>, set<Bag<int>>> mp;

    cout << "finish\n";
    return 0;
}