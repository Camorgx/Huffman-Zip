#ifndef Queue_hpp
#define Queue_hpp

#include <stdexcept>
#include <string>

class QueueException : public std::runtime_error {
public:
    explicit QueueException(const std::string& information) : runtime_error(information) {}
    const char* what() const noexcept override { return runtime_error::what(); }
};

template<typename DataType>
//data[head + 1] points to the first element in the queue, 
//data[tail - 1] points to the last element in the queue.
class Queue {
    DataType* data = new DataType[5];
    unsigned head = 0, tail = 1;
    unsigned size = 5;
public:
    Queue() = default;
    Queue(const Queue& b) {
        data = new DataType[size = b.size];
        head = b.head; tail = b.tail;
        memcpy(data, b.data, size * sizeof(DataType));
    }
    Queue(Queue&& b) noexcept = default;
    Queue& operator=(const Queue& b) noexcept {
        if (this == &b) return *this;
        delete[] data;
        data = new DataType[size = b.size];
        head = b.head; tail = b.tail;
        memcpy(data, b.data, size * sizeof(DataType));
        return *this;
    }
    Queue& operator=(Queue&& b) noexcept = default;
    ~Queue() { delete[] data; }
    
    DataType& front() {
        if (is_empty()) throw QueueException("The Queue has already been empty");
        else if (head == size - 1) return data[0];
        else return data[head + 1];
    }
    DataType& back() {
        if (is_empty()) throw QueueException("The Queue has already been empty");
        else if (tail == 0) return data[size - 1];
        else return data[tail - 1];
    }
    void push(const DataType& x) {
        if (tail == size - 1) {
            if (head == 0) {
                try {
                    auto* tmp = new DataType[2 * size];
                    memcpy(tmp, data, size * sizeof(DataType));
                    delete[] data;
                    data = tmp;
                    data[tail++] = x;
                    size *= 2;
                }
                catch (std::bad_alloc&) {
                    throw QueueException("Failed to expand the Queue!");
                }
            }
            else {
                data[tail] = x;
                tail = 0;
            }
        }
        else if (tail == head) {
            try {
                auto* tmp = new DataType[2 * size];
                memcpy(tmp, data + head, (size - head) * sizeof(DataType));
                memcpy(tmp + size - head, data, tail * sizeof(DataType));
                delete[] data;
                tail = size; head = 0;
                size *= 2; data = tmp;
                data[tail++] = x;
            }
            catch (const std::exception&) {
                throw QueueException("Failed to expand the Queue!");
            }
        }
        else data[tail++] = x;
    }
    void pop() {
        if (is_empty()) throw QueueException("The Queue has already been empty");
        else if (head == size - 1) head = 0;
        else ++head;
    }
    bool is_full() const {
        return head == tail || (head == 0 && tail == size - 1);
    }
    bool is_empty() const {
        return tail == head + 1 || (tail == 0 && head == size - 1);
    }
    size_t length() {
        if (tail > head) return tail - head - 1;
        else return tail + size - head - 1;
    }
    DataType* begin() {
        if (is_empty()) throw QueueException("The Queue has already been empty");
        else if (head == size - 1) return data;
        else return data + head + 1;
    }
    DataType* end() {
        if (is_empty()) throw QueueException("The Queue has already been empty");
        else if (tail == 0) return data + size;
        else return data + tail;
    }
};

#endif /* Queue_hpp */
