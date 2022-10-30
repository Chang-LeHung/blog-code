

#include <iostream>


void print(){}

template <typename T, typename... Types>
void print(const T& firstArg, const Types&... args) {
  std::cout << firstArg << "\t";
  print(args...);
}

int main() {

  print(1, 2, 3, 4, "hello world\n");
  return 0;
}