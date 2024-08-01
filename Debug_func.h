#pragma once

#include<iostream>
#include "Bag.h"
#include "Graph.h"
#include "TreeDecomposition.h"
#include "NiceTreeDecomposition.h"
#include "Graph_CFS.h"
#include "ImproveTreeDecomposition.h"
using namespace std;

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

void print_bags_type(NiceTreeDecomposition<int> &ntd){
    for(auto it = ntd.bagType.begin(); it != ntd.bagType.end(); ++it){
        string tmp;
        if(static_cast<int>(it->second) == 0) tmp = "LEAF";
        else if(static_cast<int>(it->second) == 1) tmp = "INTRODUCE";
        else if(static_cast<int>(it->second) == 2) tmp = "FORGET";
        else if(static_cast<int>(it->second) == 3) tmp = "JOIN";
        
        cout << it->first.id << ' ' << tmp <<endl;
    }
}

void print_tree_index(NiceTreeDecomposition<int> &ntd){
    for(auto it = ntd.bagType.begin(); it != ntd.bagType.end(); ++it){
        cout << it->first.id << ' ' << ntd.specialVertex[it->first] << ' ' << ntd.treeIndex[ntd.specialVertex[it->first]] <<endl;
    }
}