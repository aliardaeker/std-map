#include "Map.hpp"
#include <map>
#include <cassert>

#define MAX_ELEM_VAL 10000

void check(std::map<int, int> &stdmap, cs540::Map<int, int> &cs540map) {
    assert(stdmap.size() == cs540map.size());

    if (stdmap.size() == 0) {
        return;
    }

    int checkCount = rand() % stdmap.size();

    for (int i = 0; i < checkCount; ++i) {
        int elem = rand() % MAX_ELEM_VAL;
        auto iter1 = stdmap.find(elem);
        auto iter2 = cs540map.find(elem);

        if (iter1 == stdmap.end()) {
            assert(iter2 == cs540map.end());
        } else {
            assert(stdmap[elem] == cs540map[elem]);
            assert((*iter1).second == (*iter2).second);
        }
    }

}

int main() {
    cs540::Map<int, int> Map;
    std::map<int, int> map;

    for (int i = 0; i < 1000; ++i) {
        int choice = rand() % 4;
        check(map, Map);

        if (choice == 0) {
            for (int j = 0; j < 10; ++j) {
                int elem = rand() % MAX_ELEM_VAL;
                std::pair<int, int> pr = std::make_pair(elem, elem);
                Map.insert(pr);
                map.insert(pr);
            }
        } else if (choice == 1) {
            Map = Map;
            cs540::Map<int, int> MapCopy(Map);
            check(map, MapCopy);
            assert(Map == MapCopy);
        } else if (choice == 2) {
            for (int j = 0; j < 1000; ++j) {
                int elem = rand() % MAX_ELEM_VAL;
                auto iter = map.find(elem);
                auto iter2 = Map.find(elem);
                if (iter != map.end()) {
                    map.erase(iter);
                    Map.erase(iter2);
                    break;
                }
            }
        } else if (choice == 3) {
            map.clear();
            Map.clear();
        }

    }
}

