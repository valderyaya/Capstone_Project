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
        bool veryNice;

        NiceTreeDecomposition(const TreeDecomposition<T> &orignal){
            treeDecomposition = orignal;
        }

        NiceTreeDecomposition(const TreeDecomposition<T> &orignal, bool verynice){
            treeDecomposition = orignal;
            veryNice = verynice;
        }

        // TreeDecomposition<T> postprocessTreeDecomposition() , unfinish

        Bag<T> findSuitableRoot(){
            return treeDecomposition.tree.adj.begin()->first;
        }

};