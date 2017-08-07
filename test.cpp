//#define OPTIONAL_UTILITY_NO_CXX14_RETURN_TYPE_DEDUCTION
#include "optional_utility.hpp"
#include <cassert>
#include <iostream>
#include <string>

namespace
{
    struct _
    {
        template <typename F>
        _(F f)
        {
            try {
                f();
            } catch (std::exception const& e) {
                std::cerr << e.what() << std::endl;
                assert(false && "unexpected exception");
            } catch (...) {
                assert(false && "unexpected exception");
            }
        }
    };
}

#define TEST(name) _ name##__LINE__ = []

TEST(value)
{
    using optional_utility::value;
    {
        boost::optional<int> op =  42;
        assert(value(op) == 42);
    }
    {
        boost::optional<int> op = boost::none;
        try {
            value(op);
            assert(false && "not thrown");
        } catch (boost::bad_optional_access const&) {
        }
    }
};

TEST(value_or)
{
    using optional_utility::value_or;
    {
        boost::optional<int> op = 42;
        assert(value_or(op, 84) == 42);
    }
    {
        boost::optional<int> op = boost::none;
        assert(value_or(op, 84) == 84);
    }
};

TEST(map)
{
    using optional_utility::map;
    {
        boost::optional<int> op = 42;
        boost::optional<int> op2 = op
            | map([](int i) { return i * 2; });
        assert(op2.get() == 84);
    }
    {
        boost::optional<int> op = 42;
        boost::optional<int> op2 = op
            | map([](int i) { return i * 2; })
            | map([](int i) { return i + 1; });
        assert(op2.get() == 85);
    }
    {
        boost::optional<int> op = 42;
        boost::optional<std::string> op2 = op
            | map([](int i) { return std::to_string(i); });
        assert(op2.get() == "42");
    }
};

TEST(to_optional)
{
    using optional_utility::map;
    using optional_utility::to_optional;
    {
        boost::optional<int> op = 42;
        auto op2 = op
            | map([](int i) { return i * 2; })
            | to_optional;
        static_assert(std::is_same<decltype(op2), boost::optional<int>>::value, "");
        assert(op2 == 84);
    }
    {
        boost::optional<int> op = 42;
        auto temp = op
            | map([](int i) { return i * 2; });
        static_assert(!std::is_same<decltype(temp), boost::optional<int>>::value, "");
        auto op2 = temp
            | to_optional;
        assert(op2 == 84);
    }
};

TEST(member_function)
{
    using optional_utility::map;
    using namespace std::string_literals;
    {
        boost::optional<std::string> op = "hello"s;
        boost::optional<std::string::size_type> op2 = op
            | map(&std::string::length);
        assert(op2.get() == 5);
    }
    {
        boost::optional<std::string> op = "hello"s;
        boost::optional<std::string> op2 = op
            | map(&std::string::substr, 1, 3);
        assert(op2.get() == "ell");
    }
};

#include <iostream>

int main() {
    std::cout << "tests finished" << std::endl;
}
