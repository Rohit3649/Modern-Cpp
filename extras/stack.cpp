#include <iostream>
using namespace std;


template<typename T, int size>
class MyStack
{
private:
    T m_buffer[size];
    int m_top{-1};
public:
    MyStack() = default;
    MyStack(const MyStack& obj) {
        m_top = obj.m_top;
        for(int i = 0 ; i <= m_top; i++) {
            m_buffer[i] = obj.m_buffer[i];
        }
    }
    void pop();
    const T& top() {
        return m_buffer[m_top];
    }
    void push(const T& ele) {
        m_buffer[++m_top] = ele;
    }
    bool isEmpty() {
        return m_top == -1;
    }
    static MyStack Create();
};

template<typename T, int size>
MyStack<T,size> MyStack<T,size>::Create() {
    return MyStack<T,size>();
}

template<typename T, int size>
void MyStack<T,size>::pop() {
    if (m_top >= 0)
        --m_top;
}

int main()
{
    MyStack<int, 10> ms = MyStack<int,10>::Create();
    ms.push(10);
    ms.push(20);
    cout << "is empty : " << ms.isEmpty() << endl;
    cout << "Top Ele : " << ms.top() << endl;
    ms.pop();
    ms.pop();
    cout << "Top Ele : " << ms.top() << endl;
    cout << "is empty : " << ms.isEmpty() << endl;
    auto ms2(ms);
    return 0;
}