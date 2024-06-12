#include <iostream>
using namespace std;

template<typename T>
class MyVec
{
private:
    T* data;
    int size;
    int capacity;
private:
    struct iterator
    {
        //signed interger substracting 2 pointers (Bytes we need to jump from one pointer to other pointer)
        using Distance = ptrdiff_t;
        using Category = forward_iterator_tag;
        using Pointer = T*;
        using Reference = T&;
        using value_type = T;

        iterator(Pointer ptr) : mPtr{ptr} {}

        Pointer operator->() {
            return mPtr;
        }

        Reference operator*() {
            return *mPtr;
        }

        friend bool operator==(const iterator& t1, const iterator& t2) {
            return t1.mPtr == t2.mPtr;
        }

        //pre increment
        iterator& operator++() {
            (*mPtr)++;
            return *this;
        }

        // post increment
        iterator operator++(T) {
            iterator ret = *this;
            (*mPtr)++;
            return ret;
        }


        Pointer mPtr;
    };
public:

    iterator begin() {
        return iterator(&data[0]);
    }

    iterator end() {
        cout << "size : " << size << endl;
        return iterator(&data[size]);
    }

    MyVec(int capacity = 1) : capacity{capacity} {
        cout << "default cons" << endl;
        data = new T[capacity];
        size = 0;
    }

    MyVec(initializer_list<int> il) {
        cout << "initializer_list" << endl;
        int start = 0;
        size = il.size();
        capacity = size;
        for(auto& ele : il) {
            data[start++] = ele;
        }
    }

    void printData() const {
        for(int i = 0 ; i < size; i++) {
            cout << data[i] << endl;
        }
    }

    MyVec(const MyVec& obj) {
        cout << "copy cons" << endl;
        capacity = obj.capacity;
        size = obj.size;
        data = new T[capacity];
        for(int i = 0 ; i < size; i++) {
            data[i] = obj.data[i];
        }
    }

    MyVec& operator=(const MyVec& obj) {
        cout << "copy assignment" << endl;
        if (this != &obj) {
            delete[] data;
            capacity = obj.capacity;
            size = obj.size;
            data = new T[capacity];
            for(int i = 0 ; i < size; i++) {
                data[i] = obj.data[i];
            }
        }
        return *this;
    }

    MyVec(MyVec&& obj) noexcept {
        cout << "move cons" << endl;
        size = obj.size;
        capacity = obj.capacity;
        data = obj.data;
        obj.data = nullptr;
        obj.size = 0;
        obj.capacity = 0;
    }

    MyVec& operator==(MyVec&& obj) noexcept {
        cout << "move assignment" << endl;
        if (this != &obj) {
            delete[] data;
            size = obj.size;
            capacity = obj.capacity;
            data = obj.data;
            obj.data = nullptr;
            obj.size = 0;
            obj.capacity = 0;
        }
        return *this;
    }

    T operator[](int index) {
        return data[index];
    }
    
};

int main()
{
    MyVec<int> v1;
    MyVec<int> v2{1,2,3,4,5};
    //v2.printData();
    MyVec<int> v3 = v2;
    //v3.printData();
    v1 = v3;
    MyVec<int> v4 = move(v1);
    MyVec<int> v5{1,2,3,4,5};
    v4 = move(v5);
    cout << v4[2] << endl;
    for(auto& ele : v4) {
        cout << ele << endl;
    }
    return 0;
}
