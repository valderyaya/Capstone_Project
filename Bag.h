#include<unordered_set>

template<typename T>
class Bag{
    public:
        unordered_set<T> vertices;
        int id;
        Bag(unordered_set<T> V, int id):vertices(V), id(id){}

        bool contains(T v){
            return vertices.find(v) != vertices.end();
        }

        bool containsAll(unordered_set<T> s){
            if((int)vertices.size() < (int)s.size()) return 0;
            for(auto &v : s)
                if(vertices.find(v) == vertices.end()) return 0;
            return 1;
        }
};