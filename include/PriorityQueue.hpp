#ifndef HUFFMAN_ZIP_PRIORITY_QUEUE_HPP
#define HUFFMAN_ZIP_PRIORITY_QUEUE_HPP

#include "Vector.hpp"

template<typename DataType>
// Big-root heap.
class PriorityQueue {
    Vector<DataType> dat = {DataType()};

    void swim(unsigned pos) {
        while (pos > 1 && dat[pos] > dat[pos >> 1]) {
            std::swap(dat[pos], dat[pos >> 1]);
            pos >>= 1;
        }
    }
    void sink(unsigned pos) {
        while (2 * pos <= size()) {
            auto target = (2 * pos < size() &&
                    dat[2 * pos] < dat[2 * pos + 1]) ? 2 * pos + 1 : 2 * pos;
            if (dat[pos] < dat[target]) {
                std::swap(dat[pos], dat[target]);
                pos = target;
            }
            else break;
        }
    }
public:
    PriorityQueue() = default;
    PriorityQueue(const DataType* beg, const DataType* en) {
        for (auto i = beg; i != en; ++i) push(*i);
    }
    PriorityQueue(const Vector<DataType>& data) {
        for (const auto& i : data) push(i);
    }

    DataType& top() const { return dat[1]; }
    void pop() {
        if (empty()) throw VectorError("Heap already empty.");
        std::swap(dat[1], dat[dat.size() - 1]);
        dat.pop_back();
        sink(1);
    }
    void push(const DataType& x) {
        dat.push_back(x);
        swim(dat.size() - 1);
    }
    size_t size() const { return dat.size() - 1; }
    bool empty() const { return dat.size() == 1; }
};

template<typename DataType>
// Big-root heap. DataType must be pointer.
class PriorityQueue_Pointers {
    Vector<DataType> dat = {DataType()};

    void swim(unsigned pos) {
        while (pos > 1 && *(dat[pos]) > *(dat[pos >> 1])) {
            std::swap(dat[pos], dat[pos >> 1]);
            pos >>= 1;
        }
    }
    void sink(unsigned pos) {
        while (2 * pos <= size()) {
            auto target = (2 * pos < size() &&
                           *(dat[2 * pos]) < *(dat[2 * pos + 1])) ? 2 * pos + 1 : 2 * pos;
            if (*(dat[pos]) < *(dat[target])) {
                std::swap(dat[pos], dat[target]);
                pos = target;
            }
            else break;
        }
    }
public:
    PriorityQueue_Pointers() = default;
    PriorityQueue_Pointers(const DataType* beg, const DataType* en) {
        for (auto i = beg; i != en; ++i) push(*i);
    }
    PriorityQueue_Pointers(const Vector<DataType>& data) {
        for (const auto& i : data) push(i);
    }

    DataType& top() const { return dat[1]; }
    void pop() {
        if (empty()) throw VectorError("Heap already empty.");
        std::swap(dat[1], dat[dat.size() - 1]);
        dat.pop_back();
        sink(1);
    }
    void push(const DataType& x) {
        dat.push_back(x);
        swim(dat.size() - 1);
    }
    auto begin() const { return dat.begin() + 1; }
    auto end() const { return dat.end(); }
    size_t size() const { return dat.size() - 1; }
    bool empty() const { return dat.size() == 1; }
};

#endif //HUFFMAN_ZIP_PRIORITY_QUEUE_HPP
