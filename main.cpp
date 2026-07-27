#include <iostream>
//#include <cmath>
#include <cstdlib>
#include "TutorialConfig.h"
#ifdef USE_MYMATH
#include "MathFunctions.h"
#endif
#include "rebel_tools.h"
#include "rebel_test.h"

// --- The following lines break the C++11 standard (they require C++14 or newer) ---

// C++14: generic lambda (auto parameters not allowed in C++11)
auto add = [](auto a, auto b) { return a + b; };

// C++14: return type deduction for a normal function (not allowed in C++11)
auto square(int x) {
    return x * x;
}

int main(int argc, char* argv[]) {
    std::cout << "Hello, CMake!" << std::endl;

    print_os_info();

    print_test_info();

    // C++14: digit separators and binary literals (not allowed in C++11)
    int million = 1'000'000;
    int flags = 0b1010'1010;
    std::cout << add(million, flags) << " " << square(3) << std::endl;

    if ( argc < 2 ) {
        std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "." << Tutorial_VERSION_MINOR << std::endl;
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
        return 1;
    }

    const double inputValue = std::atof(argv[1]);
#ifdef USE_MYMATH
    const double outputValue = mysql(inputValue);
#else
    const double outputValue = std::sqrt(inputValue);
#endif
    std::cout << "The square root of " << inputValue << " is " << outputValue << std::endl;

    return 0;
}
