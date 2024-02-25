#include <iostream>
#include <type_traits>

template<typename T>
struct is_pointer {
    static constexpr bool value = false;
};

template<typename T>
void print1(T t) {
    if constexpr(is_pointer<T>::value) {
        std::cout << "pointer type : " << *t << std::endl;
    }
    else {
        std::cout << t << std::endl;
    }
}

template<typename A, typename B, typename C>
void print(A a, B b, C c){
    print1(a);
    print1(b);
    print1(c);
    std::cout << "********" << std::endl;
}

int main()
{
    std::string hello = "Hello world";
    print(1,2,3);
    print(hello, 2, 2.3);
    print(&hello, hello, 2.3);
    return 0;
}