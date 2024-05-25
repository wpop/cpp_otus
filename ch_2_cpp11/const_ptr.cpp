#include <iostream>

int main()
{
    int a = 10;
    int b = 20;
    const int * ptr = &a;
    ptr = &b;
    //*ptr = 30; error
    std::cout << *ptr << std::endl;

    int * const ptr_2 = &a;
    // ptr_2 = &b; error
    *ptr_2 = 30;
    std::cout << *ptr_2 << std::endl;

    int x = 1;
    int y = 2;
    int &xref = x;
    xref = y;
    std::cout << "xref = " << xref << std::endl;
    xref = 5;
    std::cout << "xref = " << xref << std::endl;

    return 0;
}