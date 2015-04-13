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

TEST(mapped)
{
    using optional_utility::mapped;
    {
        boost::optional<int> op = 42;
        boost::optional<int> op2 = op
            | mapped([](int i) { return i * 2; });
        assert(op2.get() == 84);
    }
    {
        boost::optional<int> op = 42;
        boost::optional<int> op2 = op
            | mapped([](int i) { return i * 2; })
            | mapped([](int i) { return i + 1; });
        assert(op2.get() == 85);
    }
    {
        boost::optional<int> op = 42;
        boost::optional<std::string> op2 = op
            | mapped([](int i) { return std::to_string(i); });
        assert(op2.get() == "42");
    }
};

TEST(member_function)
{
    using optional_utility::mapped;
    using namespace std::string_literals;
    {
        boost::optional<std::string> op = "hello"s;
        boost::optional<std::string::size_type> op2 = op
            | mapped(&std::string::length);
        assert(op2.get() == 5);
    }
    {
        boost::optional<std::string> op = "hello"s;
        boost::optional<std::string> op2 = op
            | mapped(&std::string::substr, 1, 3);
        assert(op2.get() == "ell");
    }
};
