#ifndef TEST_HPP
#define TEST_HPP

#include "Priority_Queue.hpp"

void TestPriorityQueue() {
    Priority_Queue<int> pq;
    cout << pq.size() << endl;
    pq.push(1); pq.push(10); pq.push(12); pq.push(6);
    pq.push(3); pq.push(5); pq.push(8); pq.push(2);
    pq.push(4); pq.push(7); pq.push(11); pq.push(9);
    cout << pq.size() << endl;
    for (const auto& i : pq) cout << i << ' ';
    cout << endl;
    int tp;
    while (!pq.empty()) {
        tp = pq.top();
        cout << tp << ' ';
        pq.pop();
    }
    cout << endl;
}

#endif //!TEST_HPP