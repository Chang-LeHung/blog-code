
#include <iostream>

using namespace std;


class A{

  public:
    virtual void start() {
      cout << "a\n";
    }
};

class B:public A {

  public:
    void start() {
      cout << "b\n";
    }
};

int main() {

  A *a = new A;
  B* b = (B*) a;
  a->start();
  return 0;
}