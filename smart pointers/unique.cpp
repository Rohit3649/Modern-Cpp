#include <iostream>
using namespace std;

template<typename T>
class UniquePtr
{
private:
    T* data;
public:
    UniquePtr(T* data = nullptr) : data{data} {
        cout << "parameterised cons" << endl;
    }

    UniquePtr(const UniquePtr& obj) = delete;

    UniquePtr& operator=(const UniquePtr& obj) = delete;

    UniquePtr(UniquePtr&& obj) {
        cout << "move cons" << endl;
        data = obj.data;
        obj.data = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& obj) {
        cout << "move assignment" << endl;
        if (this != &obj) {
            data = obj.data;
            obj.data = nullptr;
        }
        return *this;
    }

    T getData() {
        return *data;
    }

    T* operator->() {
        return data;
    }

    ~UniquePtr() {
        delete data;
    }
};

int main()
{
    cout << "[main] start" << endl;
    {
        UniquePtr<int> p1 = UniquePtr(new int(20));
        UniquePtr<int> p2 = UniquePtr(new int(10));
        //UniquePtr<int> p3(p2); //error [operation not allowed]
        //UniquePtr<int> p4 = p2; //error [operation not allowed]
        UniquePtr<int> p5;
        //p5 = p2; //error [operation not allowed]

        UniquePtr<int> p6 = move(p1);

        UniquePtr<int> p7;
        p7 = move(p2);
    }
    cout << "[main] before return" << endl;
    return 0;
}