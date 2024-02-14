#if 0
#include <iostream>
using namespace std;

class MyString
{
private:
    char* m_name;
    int m_len;
public:
    MyString() {
        cout << "[MyString] defualt cons" << endl;
        m_name = new char[]{""};
        m_len = 0;
    }
    MyString(char *name) {
        cout << "[MyString] parameterised cons" << endl;
        m_len = strlen(name) + 1;
        m_name = new char[m_len]{""};
        strcpy(m_name, name);
    }
    MyString(const MyString& obj) {
        cout << "[MyString] copy cons" << endl;
        m_len = obj.m_len;
        m_name = new char[m_len];
        strcpy(m_name, obj.m_name);
    }
    MyString(MyString&& obj) {
        cout << "[MyString] move cons" << endl;
        m_len = obj.m_len;
        m_name = obj.m_name;
        obj.m_len = 0;
        obj.m_name = nullptr;
    }
    MyString& MyString::operator = (const MyString& obj) {
        cout << "[MyString] copy assignment" << endl;
        if (this != &obj) {
            delete m_name;
            m_len = obj.m_len;
            m_name = new char[m_len];
            strcpy(m_name, obj.m_name);
        }
        return *this;
    }
    MyString& MyString::operator=(MyString&& obj) {
        cout << "[MyString] move assignment" << endl;
        if (this != &obj) {
            delete m_name;
            m_len = obj.m_len;
            m_name = obj.m_name;
            obj.m_len = 0;
            obj.m_name = nullptr;
        }
    }
};
#endif


#include <iostream>
#include <cstring> // Include for strlen, strcpy

using namespace std;

class MyString {
private:
    char* m_name;
    int m_len;

public:
    MyString() {
        cout << "[MyString] default constructor" << endl;
        m_name = nullptr; // Initialize to nullptr for safety
        m_len = 0;
    }

    MyString(const char *name) {
        cout << "[MyString] parameterised constructor" << endl;
        m_len = strlen(name) + 1;
        m_name = new char[m_len]; // Remove unnecessary initializer list
        strcpy(m_name, name);
    }

    MyString(const MyString& obj) {
        cout << "[MyString] copy constructor" << endl;
        m_len = obj.m_len;
        m_name = new char[m_len];
        strcpy(m_name, obj.m_name);
    }

    MyString(MyString&& obj) noexcept { // Add noexcept for move operations
        cout << "[MyString] move constructor" << endl;
        m_len = obj.m_len;
        m_name = obj.m_name;
        obj.m_len = 0;
        obj.m_name = nullptr;
    }

    ~MyString() { // Add destructor to free memory
    cout << "[MyString] desc" << endl;
        delete[] m_name;
    }

    MyString& operator=(const MyString& obj) {
        cout << "[MyString] copy assignment" << endl;
        if (this != &obj) {
            delete[] m_name; // Use delete[] for arrays
            m_len = obj.m_len;
            m_name = new char[m_len];
            strcpy(m_name, obj.m_name);
        }
        return *this;
    }

    MyString& operator=(MyString&& obj) noexcept { // Add noexcept for move assignment
        cout << "[MyString] move assignment" << endl;
        if (this != &obj) {
            delete[] m_name; // Use delete[] for arrays
            m_name = obj.m_name;
            m_len = obj.m_len;
            obj.m_name = nullptr;
            obj.m_len = 0;
        }
        return *this;
    }
};