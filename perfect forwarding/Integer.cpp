#if 0
#include <iostream>
using namespace std;

class MyInteger
{
private:
    int *m_x;
public:
    MyInteger() {
        cout << "[MyInteger] default cons" << endl;
        m_x = new int(0);
    }
    MyInteger(int a) {
        cout << "[MyInteger] parameterised cons" << endl;
        m_x = new int(a);
    }
    MyInteger(MyInteger &a) {
        cout << "[MyInteger] copy cons" << endl;
        m_x = new int(*a.m_x);
    }
    MyInteger(MyInteger &&a) {
        cout << "[MyInteger] move cons" << endl;
        m_x = a.m_x;
        a.m_x = nullptr;
    }
    MyInteger& MyInteger::operator=(const MyInteger& a) {
        cout << "[MyInteger] copy asignment" << endl;
        if (this != &a) {
            delete m_x;
            m_x = new int(*a.m_x);
        }
        return *this;
    }
    MyInteger& MyInteger::operator = (MyInteger &&a) {
        cout << "[MyInteger] move assignment" << endl;
        if (this != &a) {
            delete m_x;
            m_x = a.m_x;
            a.m_x = nullptr;
        }
        return *this;
    }
    ~MyInteger() {
        if (m_x) {
            delete m_x;
            m_x = nullptr;
        }
    }
};
#endif
#include <iostream>
#include <memory>
using namespace std;

class MyInteger {
private:
    std::unique_ptr<int> m_x;

public:
    // Default constructor:
    MyInteger() : m_x(std::make_unique<int>(0)) {
        // Optional: Log constructor call
        cout << "[MyInteger] default constructor" << endl;
    }

    // Parameterized constructor:
    MyInteger(int a) : m_x(std::make_unique<int>(a)) {
        // Optional: Log constructor call
        cout << "[MyInteger] Parameterized constructor" << endl;
    }

    // Copy constructor:
    MyInteger(const MyInteger& other) : m_x(std::make_unique<int>(*other.m_x)) {
        // Optional: Log constructor call
        cout << "[MyInteger] Copy constructor" << endl;
    }

    // Move constructor:
    MyInteger(MyInteger&& other) noexcept : m_x(std::move(other.m_x)) {
        other.m_x = nullptr;
        // Optional: Log constructor call
        cout << "[MyInteger] Move constructor" << endl;
    }

    // Copy assignment operator:
    MyInteger& operator=(const MyInteger& other) {
        cout << "[MyInteger] Copy assignment" << endl;
        if (this != &other) {
            m_x = std::make_unique<int>(*other.m_x);
        }
        return *this;
    }

    // Move assignment operator:
    MyInteger& operator=(MyInteger&& other) noexcept {
        cout << "[MyInteger] Move assignment" << endl;
        if (this != &other) {
            m_x = std::move(other.m_x);
            other.m_x = nullptr;
        }
        return *this;
    }

    // Destructor (automatically handled by unique_ptr):
    ~MyInteger() = default;

    // Accessor method:
    int getValue() const {
        return *m_x;
    }

    // Setter method:
    void setValue(int value) {
        *m_x = value;
    }

    // Additional methods as needed
};