#include "Integer.cpp"
#include "MyString.cpp"

class Employee
{
    MyString m_name;
    MyInteger m_age;
public:
    Employee() : m_name(""), m_age(0) {
        cout << "[Employee] default constructor" << endl;
    }
    #if 0
    explicit Employee(const MyString& name, const MyInteger& age) : m_name(name), m_age(age) {
        cout << "[Employee] parameterised constructor [lvalue][lvalue]" << endl;
    }
    Employee(MyString&& name, MyInteger&& age) noexcept : m_name(move(name)), m_age(move(age)) {
        cout << "[Employee] parameterised constructor [rvalue][rvalue]" << endl;
    }
    Employee(const MyString& name, MyInteger &&age): m_name(name), m_age(move(age)) {
        cout << "[Employee] parameterised constructor [lvalue][rvalue]" << endl;
    }
    Employee(MyString&& name, const MyInteger &age): m_name(move(name)), m_age(age) {
        cout << "[Employee] parameterised constructor [rvalue][lvalue]" << endl;
    }
    #endif

    template<typename T1, typename T2>
    Employee(T1&& ms, T2&& mi) : m_name{forward<T1>(ms)}, m_age{forward<T2>(mi)} {
        cout << "[Employee] template constructor" << endl;
    }

    ~Employee() {
        cout << "[Employee] destructor" << endl;
    }
};

int main()
{
    MyString ms("rohit");
    MyInteger mi(10);
    //Employee emp{ms, mi};
    Employee emp6{MyString("rk"), MyInteger(100)};

    /*
    [MyString] parameterised constructor
    [MyInteger] Parameterized constructor
    [MyString] parameterised constructor
    [MyInteger] Parameterized constructor
    [MyString] move constructor
    [MyInteger] Move constructor
    [Employee] parameterised constructor [rvalue][rvalue]
    [Employee] destructor
    */
    //Employee emp1{"rohit", 10};

    /*
    [MyString] parameterised constructor
    [MyInteger] Parameterized constructor
    [MyInteger] Parameterized constructor
    [MyString] copy constructor
    [MyInteger] Move constructor
    [Employee] parameterised constructor [lvalue][rvalue]
    [Employee] destructor
    */
    //Employee emp2(ms, 10);
    //Employee em5(ms, MyInteger(10));

    /*
    [MyString] parameterised constructor
    [MyInteger] Parameterized constructor
    [MyString] parameterised constructor
    [MyString] move constructor
    [MyInteger] Copy constructor
    [Employee] parameterised constructor [rvalue][lvalue]
    [Employee] destructor
    */
    //Employee emp3("rohit", mi);

    /*
    [MyString] parameterised constructor
    [MyInteger] Parameterized constructor
    [MyString] copy constructor
    [MyInteger] Copy constructor
    [Employee] parameterised constructor [lvalue][lvalue]
    [Employee] destructor
    */
    //Employee emp4(ms, mi);

    return 0;
}