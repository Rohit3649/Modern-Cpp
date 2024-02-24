#include <iostream>
#include <memory>
using namespace std;

using Type = int;

template<typename T>
class MyWeakPtr
{
private:
    shared_ptr<T> sp;
    bool expired;
public:
    MyWeakPtr(const shared_ptr<T>& p) : sp{p}, expired{false} {
        cout << "copy cons" << endl;
    }

    bool hasExpired() const {
        return expired;
    }

    shared_ptr<T> lock() {
        if (!expired && sp.use_count() > 1) {
            return sp;
        }
        expired = true;
        return nullptr;
    }
};

int main()
{
    shared_ptr<Type> sp = make_shared<Type>(10);
    MyWeakPtr wp(sp);

    if (!wp.hasExpired()) {
        shared_ptr<Type> tmp = wp.lock();
        if (tmp) {
            cout << "Value : " << *tmp << endl;
        }
        else {
            cout << "does not exist anymore" << endl;
        }
    }

    sp = nullptr;

    if (!wp.hasExpired()) {
        shared_ptr<Type> tmp = wp.lock();
        if (tmp) {
            cout << "Value : " << *tmp << endl;
        }
        else {
            cout << "does not exist anymore" << endl;
        }
    }
    return 0;
}