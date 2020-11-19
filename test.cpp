#include "simplesignal.h"
#include <iostream>

using namespace std;

class A {
public:
    void hello(int a, double l) {
        cout << "hello world!" << endl;
        cout << "integer: " << a << endl;
        cout << "double: " << l << endl;
    }
};

class Mysig {
public:
    void emit(int a, double l) {
        mysig(a, l);
    }
public:
    Signal<int, double> mysig;
};

int main() {
    Mysig *s1 = new Mysig;
    A *a1 = new A;
    CONNECT(s1, mysig, a1, &A::hello);

    s1->emit(2, 4.5e3);
    return 0;
}