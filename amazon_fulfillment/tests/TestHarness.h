#pragma once

// A tiny, dependency-free unit test harness.
//
//   TEST_CASE(name) { ... }          registers a test
//   EXPECT_EQ(actual, expected)      fails the current test if values differ
//   EXPECT_TRUE(condition)           fails the current test if condition is false
//   EXPECT_THROWS(statement, Type)   fails unless statement throws Type
//
// The first failing expectation stops the current test.

#include <functional>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace testing_lite {

struct TestCase {
    std::string name;
    std::function<void()> body;
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

struct Registrar {
    Registrar(const char* name, std::function<void()> body) {
        registry().push_back({name, std::move(body)});
    }
};

struct AssertionFailure {
    std::string message;
};

template <typename T>
std::string show(const T& value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

inline std::string show(bool value) { return value ? "true" : "false"; }

inline std::string show(std::nullopt_t) { return "nullopt"; }

template <typename T>
std::string show(const std::optional<T>& value) {
    return value ? show(*value) : std::string("nullopt");
}

template <typename T>
std::string show(const std::vector<T>& values) {
    std::string out = "[";
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) out += ", ";
        out += show(values[i]);
    }
    return out + "]";
}

[[noreturn]] inline void fail(const char* file, int line, const std::string& what) {
    std::ostringstream os;
    os << file << ":" << line << ": " << what;
    throw AssertionFailure{os.str()};
}

}  // namespace testing_lite

#define TEST_CASE(name)                                                        \
    static void name();                                                        \
    static ::testing_lite::Registrar name##_registrar(#name, name);            \
    static void name()

#define EXPECT_EQ(actual, expected)                                            \
    do {                                                                       \
        const auto& tl_actual_ = (actual);                                     \
        const auto& tl_expected_ = (expected);                                 \
        if (!(tl_actual_ == tl_expected_)) {                                   \
            ::testing_lite::fail(__FILE__, __LINE__,                           \
                "EXPECT_EQ(" #actual ", " #expected ")\n"                      \
                "        actual:   " + ::testing_lite::show(tl_actual_) + "\n" \
                "        expected: " + ::testing_lite::show(tl_expected_));    \
        }                                                                      \
    } while (0)

#define EXPECT_TRUE(condition)                                                 \
    do {                                                                       \
        if (!(condition)) {                                                    \
            ::testing_lite::fail(__FILE__, __LINE__,                           \
                "EXPECT_TRUE(" #condition ") was false");                      \
        }                                                                      \
    } while (0)

#define EXPECT_THROWS(statement, ExceptionType)                                \
    do {                                                                       \
        bool tl_threw_ = false;                                                \
        try {                                                                  \
            statement;                                                         \
        } catch (const ExceptionType&) {                                       \
            tl_threw_ = true;                                                  \
        } catch (...) {                                                        \
            ::testing_lite::fail(__FILE__, __LINE__,                           \
                "EXPECT_THROWS(" #statement ", " #ExceptionType                \
                ") threw a different exception type");                         \
        }                                                                      \
        if (!tl_threw_) {                                                      \
            ::testing_lite::fail(__FILE__, __LINE__,                           \
                "EXPECT_THROWS(" #statement ", " #ExceptionType                \
                ") did not throw");                                            \
        }                                                                      \
    } while (0)
