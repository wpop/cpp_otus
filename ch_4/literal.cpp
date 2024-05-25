#include "pretty.h"

#include <iostream>
#include <chrono>
#include <string>


struct kg {
    unsigned long long value;
    unsigned long long get() const { return value; }
};

struct m_sec_2 {
    unsigned long long value;
    unsigned long long get() const { return value; }
};

kg operator""_kg(unsigned long long value) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return kg{value};
}

m_sec_2 operator""_m_sec_2(unsigned long long value) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return m_sec_2{value};
}

void example1() 
{
    std::cout << "\nexample1" << std::endl;
    // auto a = 42;
    auto kg = 42_kg;
    std::cout << kg.get() << std::endl;
}

void example2()
{
    std::cout << "\nexample2" << std::endl;
    auto kg = 83_kg;
    auto g = 9_m_sec_2;
    std::cout << g.get() << std::endl;
    // auto F = kg * g;
}


std::string operator""_english(const char *, size_t) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return "sorok dva";
}

void example3()
{
    std::cout << "\nexample3" << std::endl;

    auto c = "42"_english;
    std::cout << c << std::endl << std::endl;
}

// void example4() {
//     std::cout << "\nexample4" << std::endl;

//     using namespace std::chrono_literals;
//         // std::chrono::seconds
//     auto seconds = 123s;
//     auto count = seconds.count();
//     123s .count();
// }

// -------------------- MAIN ---------------------
int main()
{
    int value2 = 0xff;
    std::cout << "Hexa 0xff: " << value2 << std::endl;

    int value3 = 0b1111;
    std::cout << "Binary 0b1111: " << value3 << std::endl;

    int value4 = 0137;
    std::cout << "Octa 0137: " << value4 << std::endl;

    // ------------------- literals ---------------
    example1();
    example2();
    example3();

    return 0;
}