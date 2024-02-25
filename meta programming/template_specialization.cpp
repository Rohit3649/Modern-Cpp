#include <iostream>
using namespace std;

/*  Template Definition  */
template<typename KEY, typename VALUE>
struct QMap {
    QMap() {
        cout << "QMap(KEY,VALUE)" << endl;
    }
};

/*  Partial Specialization  */
template<typename VALUE>
struct  QMap<int, VALUE>
{
    QMap() {
        cout << "QMap(int,Value)" << endl;
    }
};

/*  Full Specialization  */
template<>
struct QMap<int,int>
{
    QMap() {
        cout << "QMap(int,int)" << endl;
    }
};



int main()
{
    auto _default = QMap<float, int>();
    auto _partial = QMap<int, bool>();
    auto _full = QMap<int, int>();
    return 0;
}