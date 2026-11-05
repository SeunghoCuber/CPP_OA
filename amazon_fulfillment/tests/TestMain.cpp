// Test runner entry point.
//
// Usage:
//   ./build/run_tests            run every test
//   ./build/run_tests T05        run only tests whose name contains "T05"

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

#include "TestHarness.h"

int main(int argc, char** argv) {
    const std::string filter = argc > 1 ? argv[1] : "";

    auto tests = testing_lite::registry();
    std::sort(tests.begin(), tests.end(),
              [](const auto& a, const auto& b) { return a.name < b.name; });

    int ran = 0;
    int passed = 0;
    for (const auto& test : tests) {
        if (!filter.empty() && test.name.find(filter) == std::string::npos) {
            continue;
        }
        ++ran;
        std::cout << "[ RUN  ] " << test.name << std::endl;
        try {
            test.body();
            ++passed;
            std::cout << "[ PASS ] " << test.name << std::endl;
        } catch (const testing_lite::AssertionFailure& failure) {
            std::cout << "    " << failure.message << std::endl;
            std::cout << "[ FAIL ] " << test.name << std::endl;
        } catch (const std::exception& ex) {
            std::cout << "    unexpected exception: " << ex.what() << std::endl;
            std::cout << "[ FAIL ] " << test.name << std::endl;
        } catch (...) {
            std::cout << "    unexpected non-standard exception" << std::endl;
            std::cout << "[ FAIL ] " << test.name << std::endl;
        }
    }

    std::cout << "\n" << passed << "/" << ran << " tests passed" << std::endl;
    if (ran == 0) {
        std::cout << "No tests matched filter \"" << filter << "\"" << std::endl;
        return 1;
    }
    return passed == ran ? 0 : 1;
}
