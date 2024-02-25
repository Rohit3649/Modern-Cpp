#include <iostream>
using namespace std;

#define SAFE_DELETE(p) if(p) {delete p; p = nullptr;}

template<typename T>
class SharedPtr
{
private:
    T* data;
    int* refCount;
    void incrementRefCount() {
        if (refCount) {
            (*refCount)++;
        }
    }
    void decrementRefCont() {
        if (refCount) {
            (*refCount)--;
            if (*refCount == 0) {
                cout << "free resource" << endl;
                SAFE_DELETE(refCount);
                SAFE_DELETE(data);
            }
        }
    }
public:
    SharedPtr(T* data = nullptr) : data{data}, refCount{new int(1)} {
        cout << "parameterised cons" << endl;
    }

    SharedPtr(const SharedPtr& obj) {
        cout << "copy cons" << endl;
        data = obj.data;
        refCount = obj.refCount;
        incrementRefCount();
    }

    SharedPtr& operator=(const SharedPtr& obj) {
        cout << "copy assignment" << endl;
        if (this != &obj) {
            decrementRefCont();
            data = obj.data;
            refCount = obj.refCount;
            incrementRefCount();
        }
        return *this;
    }

    SharedPtr(SharedPtr&& obj) {
        cout << "move cons" << endl;
        data = obj.data;
        refCount = obj.refCount;
        obj.data = nullptr;
        obj.refCount = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& obj) {
        cout << "move assignment" << endl;
        if (this != &obj) {
            decrementRefCont();
            data = obj.data;
            refCount = obj.refCount;
            obj.data = nullptr;
            obj.refCount = nullptr;
        }
        return *this;
    }

    int getRefCount() {
        return refCount ? *refCount : -1;
    }

    T getData() {
        return *data;
    }

    void reset(T* res) {
        decrementRefCont();
        data = res;
        refCount = new int(1);
    }

    T* operator->() {
        return data;
    }

    ~SharedPtr() {
        decrementRefCont();
    }
};

int main()
{
    cout << "[main] start" << endl;
    {
        SharedPtr<int> p1 = SharedPtr(new int(20));
        SharedPtr<int> p2 = SharedPtr(new int(10));
        SharedPtr<int> p3(p2);
        SharedPtr<int> p4 = p2;
        SharedPtr<int> p5;
        p5 = p2;
        cout << "P1 refcount : " << p1.getRefCount() << endl;
        cout << "P2 refcount : " << p2.getRefCount() << endl;
        cout << "P3 refcount : " << p3.getRefCount() << endl;
        cout << "P4 refcount : " << p4.getRefCount() << endl;
        cout << "P5 refcount : " << p5.getRefCount() << endl;

        SharedPtr<int> p6 = move(p1);
        cout << "P6 refcount : " << p6.getRefCount() << endl;

        SharedPtr<int> p7;
        p7 = move(p2);
        cout << "P7 refcount : " << p7.getRefCount() << endl;
    }
    cout << "[main] before return" << endl;
    return 0;
}