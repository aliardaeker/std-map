#include <map>
#include "Map.hpp"
#include <random>
#include <cassert>
#include <numeric>
#include <algorithm>

#define OPTS 3
#define MAX_ITER 1000
#define MAX_ELEM_COUNT 1000
#define MAX_ELEM_VAL 10000

struct AddValues {
    long operator()(int value, const std::pair<int, int> &pair) const {
        return value + pair.second;
    }
};

void checkIteration(std::map<int, int> &stdmap, cs540::Map<int, int> &cs540map) {
    // Find sum of elements from begin to end
    auto val1 = std::accumulate(stdmap.begin(), stdmap.end(), 0, AddValues());
    auto val2 = std::accumulate(cs540map.begin(), cs540map.end(), 0, AddValues());
    assert(val1 == val2);

    // Reverse sum should be same too.
    auto val3 = std::accumulate(cs540map.rbegin(), cs540map.rend(), 0, AddValues());
    assert(val1 == val3);

}

void insertElems(std::map<int, int> &stdmap, cs540::Map<int, int> &cs540map) {

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, MAX_ELEM_COUNT);
    int randnum = dist6(rng);

    for (int i = 0; i < randnum; ++i) {

        // Select one of three moethod to insert 
        // elements in map
        int choice = rand() % 3;

        if (choice == 0) {
            int elem = rand() % MAX_ELEM_VAL;
            std::pair<int, int> pr = std::make_pair(elem, elem);
            cs540map.insert(pr);
            stdmap.insert(pr);
        } else if (choice == 1) {
            int elem = rand() % MAX_ELEM_VAL;
            cs540map[elem] = elem;
            stdmap[elem] = elem;
        } else {
        }
    }
}

void eraseElem(std::map<int, int> &stdmap, cs540::Map<int, int> &cs540map) {
    for (int i = 0; i < 500; i++) {
        int elem = rand() % MAX_ELEM_VAL;
        auto iter1 = stdmap.find(elem);
        if (iter1 == stdmap.end()) {
            continue;
        }
        stdmap.erase(iter1);
        auto iter2 = cs540map.find(elem);
        cs540map.erase(iter2);
    }
}

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
            int newelem = rand() % MAX_ELEM_VAL;
            stdmap[elem] = newelem;
            cs540map[elem] = newelem;
        }


    }

}

int main() {

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, OPTS);

    std::map<int, int> stdmap;
    cs540::Map<int, int> cs540map;

    if (stdmap.size() == 0) {
        assert(cs540map.empty());
        insertElems(stdmap, cs540map);

    }

    for (int i = 0; i < MAX_ITER; ++i) {

        int randn = dist6(rng);
        // Check if map are infact same after every 10 iterations
        if (i % 10 == 0) {
            check(stdmap, cs540map);
        }

        if (randn == 0) {
            //std::cout << stdmap.size() << '\n';
            insertElems(stdmap, cs540map);
        } else if (randn == 1) { //erase element
            eraseElem(stdmap, cs540map);
        } else if (randn == 2) { // reverse iteration and forward iteration
            checkIteration(stdmap, cs540map);
        } else if (randn == 3) {
            stdmap.clear();
            cs540map.clear();
        }

    }

    return 0;
}


