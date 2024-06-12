#include <iostream>
using namespace std;

class Base
{
public:
    Base() {
        cout << "default const" << endl;
    }

    Base(const Base& obj) {
        cout << "copy cons" << endl;
    }
    
    Base(const Base&& obj) {
        cout << "move cons" << endl;
    }

    Base& operator=(const Base& obj) {
        cout << "copy assignment" << endl;
        return *this;
    }

    Base& operator=(Base&& obj) {
        cout << "move assignment" << endl;
        return *this;
    }

    //Returns a pointer to the current instance of the Base class.
    Base* BaseA() {
        return this;
    }
#if 0
    Base BaseB() {
        return this;
    }
    Base& BaseC() {
        return this;
    }
    /***********************************************/
    Base* BaseD() {
        return *this;
    }
#endif
    //returns a copy of the current instance of the Base class
    Base BaseE() {
        return *this;
    }
    //returns a reference to the current instance of the Base class. 
    Base& BaseF() {
        return *this;
    }
#if 0
    /***********************************************/
    Base* BaseG() {
        return &this;
    }
    Base BaseH() {
        return &this;
    }
    Base& BaseI() {
        return &this;
    }
#endif
};

int main()
{
    Base b;
    Base* b1 = b.BaseA(); //no output
    
    //Base* b2 = b.BaseE(); // comilation error
    //Base& b2 = b.BaseE(); //compilation error
    //Base b2 = b.BaseE(); //copy cons


    //Base b3 = b.BaseF(); //copy cons
    //Base& b3 = b.BaseF(); // no output
    return 0;
}