#include <iostream>
#include <iterator>
#include <initializer_list>
using namespace std;

template<typename T>
class MyVec
{
private:
    T* data;
    size_t size;
    size_t capacity;

public:
    // Iterator class with full random access iterator implementation
    class iterator
    {
    private:
        T* mPtr;

    public:
        // Constructor
        explicit iterator(T* ptr) : mPtr(ptr) {}

        // Dereference operators
        T& operator*() const { return *mPtr; }
        T* operator->() const { return mPtr; }
        T& operator[](std::ptrdiff_t n) const { return mPtr[n]; }

        // Increment/Decrement operators
        iterator& operator++() { ++mPtr; return *this; }
        iterator operator++(int) { iterator temp = *this; ++mPtr; return temp; }
        iterator& operator--() { --mPtr; return *this; }
        iterator operator--(int) { iterator temp = *this; --mPtr; return temp; }

        // Arithmetic operators
        iterator& operator+=(std::ptrdiff_t n) { mPtr += n; return *this; }
        iterator& operator-=(std::ptrdiff_t n) { mPtr -= n; return *this; }
        iterator operator+(std::ptrdiff_t n) const { return iterator(mPtr + n); }
        iterator operator-(std::ptrdiff_t n) const { return iterator(mPtr - n); }
        
        // Friend function for n + iterator
        friend iterator operator+(std::ptrdiff_t n, const iterator& it) {
            return iterator(it.mPtr + n);
        }

        // Distance between iterators
        std::ptrdiff_t operator-(const iterator& other) const {
            return mPtr - other.mPtr;
        }

        // Comparison operators
        bool operator==(const iterator& other) const { return mPtr == other.mPtr; }
        bool operator!=(const iterator& other) const { return mPtr != other.mPtr; }
        bool operator<(const iterator& other) const { return mPtr < other.mPtr; }
        bool operator<=(const iterator& other) const { return mPtr <= other.mPtr; }
        bool operator>(const iterator& other) const { return mPtr > other.mPtr; }
        bool operator>=(const iterator& other) const { return mPtr >= other.mPtr; }
    };

    // Const iterator (for const correctness)
    class const_iterator
    {
    private:
        const T* mPtr;

    public:
        explicit const_iterator(const T* ptr) : mPtr(ptr) {}
        
        // Allow conversion from iterator to const_iterator
        const_iterator(const iterator& it) : mPtr(&(*it)) {}

        const T& operator*() const { return *mPtr; }
        const T* operator->() const { return mPtr; }
        const T& operator[](std::ptrdiff_t n) const { return mPtr[n]; }

        const_iterator& operator++() { ++mPtr; return *this; }
        const_iterator operator++(int) { const_iterator temp = *this; ++mPtr; return temp; }
        const_iterator& operator--() { --mPtr; return *this; }
        const_iterator operator--(int) { const_iterator temp = *this; --mPtr; return temp; }

        const_iterator& operator+=(std::ptrdiff_t n) { mPtr += n; return *this; }
        const_iterator& operator-=(std::ptrdiff_t n) { mPtr -= n; return *this; }
        const_iterator operator+(std::ptrdiff_t n) const { return const_iterator(mPtr + n); }
        const_iterator operator-(std::ptrdiff_t n) const { return const_iterator(mPtr - n); }
        
        friend const_iterator operator+(std::ptrdiff_t n, const const_iterator& it) {
            return const_iterator(it.mPtr + n);
        }

        std::ptrdiff_t operator-(const const_iterator& other) const {
            return mPtr - other.mPtr;
        }

        bool operator==(const const_iterator& other) const { return mPtr == other.mPtr; }
        bool operator!=(const const_iterator& other) const { return mPtr != other.mPtr; }
        bool operator<(const const_iterator& other) const { return mPtr < other.mPtr; }
        bool operator<=(const const_iterator& other) const { return mPtr <= other.mPtr; }
        bool operator>(const const_iterator& other) const { return mPtr > other.mPtr; }
        bool operator>=(const const_iterator& other) const { return mPtr >= other.mPtr; }
    };

    // Iterator accessors
    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + size); }
    const_iterator begin() const { return const_iterator(data); }
    const_iterator end() const { return const_iterator(data + size); }
    const_iterator cbegin() const { return const_iterator(data); }
    const_iterator cend() const { return const_iterator(data + size); }

    // Constructors
    MyVec(size_t capacity = 1) : capacity(capacity), size(0) {
        cout << "default constructor" << endl;
        data = new T[capacity];
    }

    MyVec(std::initializer_list<T> il) : capacity(il.size()), size(il.size()) {
        cout << "initializer_list constructor" << endl;
        data = new T[capacity];
        std::copy(il.begin(), il.end(), data);
    }

    // Rule of 5 implementation
    MyVec(const MyVec& obj) : capacity(obj.capacity), size(obj.size) {
        cout << "copy constructor" << endl;
        data = new T[capacity];
        std::copy(obj.data, obj.data + size, data);
    }

    MyVec& operator=(const MyVec& obj) {
        cout << "copy assignment" << endl;
        if (this != &obj) {
            delete[] data;
            capacity = obj.capacity;
            size = obj.size;
            data = new T[capacity];
            std::copy(obj.data, obj.data + size, data);
        }
        return *this;
    }

    MyVec(MyVec&& obj) noexcept : data(obj.data), size(obj.size), capacity(obj.capacity) {
        cout << "move constructor" << endl;
        obj.data = nullptr;
        obj.size = 0;
        obj.capacity = 0;
    }

    // Fixed: Changed from operator== to operator=
    MyVec& operator=(MyVec&& obj) noexcept {
        cout << "move assignment" << endl;
        if (this != &obj) {
            delete[] data;
            data = obj.data;
            size = obj.size;
            capacity = obj.capacity;
            obj.data = nullptr;
            obj.size = 0;
            obj.capacity = 0;
        }
        return *this;
    }

    ~MyVec() {
        delete[] data;
    }

    // Accessors
    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }
    
    size_t getSize() const { return size; }
    size_t getCapacity() const { return capacity; }
    bool empty() const { return size == 0; }

    // Utility functions
    void printData() const {
        for(size_t i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    // Add element (basic implementation)
    void push_back(const T& value) {
        if (size >= capacity) {
            // Simple doubling strategy
            capacity *= 2;
            T* newData = new T[capacity];
            std::copy(data, data + size, newData);
            delete[] data;
            data = newData;
        }
        data[size++] = value;
    }
};

int main()
{
    cout << "=== Testing Basic Functionality ===" << endl;
    MyVec<int> v1;
    MyVec<int> v2{1, 2, 3, 4, 5};
    
    cout << "\n=== Testing Range-based for loop ===" << endl;
    for(const auto& ele : v2) {
        cout << ele << " ";
    }
    cout << endl;
    
    cout << "\n=== Testing STL algorithms ===" << endl;
    // Test with STL algorithms
    auto it = std::find(v2.begin(), v2.end(), 3);
    if (it != v2.end()) {
        cout << "Found: " << *it << " at position: " << (it - v2.begin()) << endl;
    }
    
    cout << "\n=== Testing iterator arithmetic ===" << endl;
    auto begin_it = v2.begin();
    auto third_element = begin_it + 2;
    cout << "Third element: " << *third_element << endl;
    cout << "Distance from begin to third: " << (third_element - begin_it) << endl;
    
    cout << "\n=== Testing const iterator ===" << endl;
    const MyVec<int> v3{10, 20, 30};
    for(auto it = v3.cbegin(); it != v3.cend(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    
    cout << "\n=== Testing move semantics ===" << endl;
    MyVec<int> v4 = std::move(v2);
    cout << "v4 size after move: " << v4.getSize() << endl;
    cout << "v2 size after move: " << v2.getSize() << endl;
    
    return 0;
}
