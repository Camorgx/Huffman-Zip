#ifndef TEST_HPP
#define TEST_HPP

#include "PriorityQueue.hpp"
#include <iostream>

void TestPriorityQueue() {
    PriorityQueue<int> pq;
    std::cout << pq.size() << std::endl;
    pq.push(1); pq.push(10); pq.push(12); pq.push(6);
    pq.push(3); pq.push(5); pq.push(8); pq.push(2);
    pq.push(4); pq.push(7); pq.push(11); pq.push(9);
    std::cout << pq.size() << std::endl;
    for (const auto& i : pq) std::cout << i << ' ';
    std::cout << std::endl;
    int tp;
    while (!pq.empty()) {
        tp = pq.top();
        std::cout << tp << ' ';
        pq.pop();
    }
    std::cout << std::endl;
}

#include "HashMap.hpp"

void TestHashMap() {
    HashMap hashMap;
    hashMap[1] = 2; hashMap[12] = 3; hashMap[13] = 4;
    for (const auto& item : hashMap)
        std::cout << item.key << ' ' << item.value << std::endl;
}

#endif //!TEST_HPP