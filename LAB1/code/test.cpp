#include <iostream>

void foo() {
    std::cout << "recursive call" << std::endl;
    foo();
}

int main() {
    foo();
    return 0;
}