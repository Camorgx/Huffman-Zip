#ifndef Vector_hpp
#define Vector_hpp

#include <stdexcept>
#include <initializer_list>

class VectorError : public std::runtime_error {
public:
    explicit VectorError(const char* info) : runtime_error(info) {}
    explicit VectorError(const std::string& information) : runtime_error(information) {}
    const char* what() const noexcept override { return runtime_error::what(); }
};

template<typename DataType>
class Vector {
    DataType* dat = new DataType[8];
    //Actual size of the space allocated.
    size_t actual_size = 8;
    //Size of the dynamic array.
    size_t array_size = 0;
public:
    Vector() = default;
    Vector(DataType* data, unsigned size) {
        actual_size = array_size = size;
        dat = new DataType[actual_size];
        for (unsigned i = 0; i < array_size; ++i)
            dat[i] = data[i];
    }
    Vector(Vector<DataType>&& b) noexcept {
        dat = b.dat; 
        actual_size = b.actual_size;
        array_size = b.array_size;
        b.array_size = b.actual_size = 0;
        b.dat = nullptr;
    }
    Vector(const Vector<DataType>& b) { *this = b; }
    Vector(std::initializer_list<DataType> initializerList) {
        for (auto&& i : initializerList) push_back(i);
    }
    ~Vector() { if (dat) delete[] dat; }

    Vector<DataType>& operator=(Vector<DataType>&& b) noexcept {
        dat = b.dat; 
        actual_size = b.actual_size;
        array_size = b.array_size;
        b.dat = nullptr;
        b.array_size = b.actual_size = 0;
        return *this;
    }
    Vector<DataType>& operator=(const Vector<DataType>& b) {
        if (this == &b) return *this;
        dat = new DataType[actual_size = b.actual_size];
        array_size = b.array_size;
        for (unsigned i = 0; i < array_size; ++i) dat[i] = b[i];
        return *this;
    }
    bool operator==(const Vector<DataType>& b) const {
        if (dat == b.dat) return true;
        if (array_size != b.array_size) return false;
        for (unsigned i = 0; i < array_size; ++i)
            if (dat[i] != b.dat[i]) return false;
        return true;
    }
    DataType& operator[](const int id) const {
        if (id >= 0 && unsigned(id) < array_size) return dat[id];
        else throw VectorError("Array out of bounds.");
    }

    size_t size() const { return array_size; }
    bool empty() const { return array_size == 0; }
    DataType* begin() const { return dat; }
    DataType* end() const { return dat + array_size; }
    void clear() { array_size = 0; }
    void push_back(const DataType& x) {
        if (actual_size == 0) {
            actual_size = 8;
            dat = new DataType[8];
        }
        if (array_size >= actual_size) {
            try {
                auto* tmp = new DataType[actual_size * 2];
                for (unsigned i = 0; i < actual_size; ++i)
                    tmp[i] = std::move(dat[i]);
                delete[] dat;
                dat = tmp;
                actual_size *= 2;
            }
            catch (std::runtime_error&) {
                throw VectorError("Failed to allocate memory.");
            }
        }
        dat[array_size++] = x;
    }
    void push_back(DataType&& x) {
        if (actual_size == 0) {
            actual_size = 8;
            dat = new DataType[8];
        }
        if (array_size >= actual_size) {
            try {
                auto* tmp = new DataType[actual_size * 2];
                for (unsigned i = 0; i < actual_size; ++i)
                    tmp[i] = std::move(dat[i]);
                actual_size *= 2;
                delete[] dat;
                dat = tmp;
            }
            catch (std::runtime_error&) {
                throw VectorError("Failed to allocate memory.");
            }
        }
        dat[array_size++] = x;
    }
    void pop_back() {
        if (array_size) --array_size;
        else throw VectorError("Array already empty.");
    }
    const DataType* c_array() const { return dat; }
};

#endif // !Vector_hpp
