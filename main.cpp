#include<bits/stdc++.h>
using namespace std;
#include "Bag.h"

int main(){
    unordered_set<int> s;
    s.insert(1);
    s.insert(2);
    s.insert(3);

    unordered_set<int> a;
    a.insert(1);
    a.insert(5);
    
    Bag<int> test = Bag<int>(s, 2);
    cout << test.containsAll(a) << ' ' << test.containsAll(s) << endl;
    return 0;
}