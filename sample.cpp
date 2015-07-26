// Customize
// You need to define `OPTIONAL_UTILITY_NO_CXX14_RETURN_TYPE_DEDUCTION` macro if you compile c++11 mode. e.g. `std=c++11`
// You define `USE_BOOST_LAMBDA` if you want to use Booost.Lambda in sample codes.
//#define OPTIONAL_UTILITY_NO_CXX14_RETURN_TYPE_DEDUCTION
//#define USE_BOOST_LAMBDA
#include "optional_utility.hpp"
#include <boost/optional/optional_io.hpp>
#if defined(USE_BOOST_LAMBDA)
#include <boost/lambda/lambda.hpp>
#endif
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
using boost::optional;

// returns joined strings if both strings has a value. otherwise none;
optional<std::string> full_name(optional<std::string> const& first_name, optional<std::string> const& last_name)
{
    using optional_utility::flat_map;
    using optional_utility::map;
    return last_name | flat_map([&first_name](std::string const& ln) {
        return first_name | map([&ln](std::string const& fn) {
            return ln + " " + fn;
        });
    });
}

optional<int> divide(int dividend, int divisor)
{
    if (divisor == 0) {
        return boost::none;
    }
    return dividend / divisor;
}

int main()
{
    // `map` and `flat_map`
    {
        // basic usage
        auto const first_name = boost::make_optional<std::string>("John");
        auto const last_name = boost::make_optional<std::string>("Smith");
        std::cout << full_name(first_name, last_name) << std::endl; // John Smith
        std::cout << full_name(first_name, boost::none) << std::endl; // --
        std::cout << full_name(boost::none, last_name) << std::endl; // --
        std::cout << full_name(boost::none, boost::none) << std::endl; // -- 
    }
    {
        // adaptors are lazy evaluation
        using optional_utility::map;
        auto const op = boost::make_optional<int>(42);
        int counter = 0;
        auto const temp = op
            | map([&counter](int i) { ++counter;  return i * 2; })
            | map([&counter](int i) { ++counter;  return i + 1; });
        // it is not evalutated yet
        std::cout << counter << std::endl; // 0
        // it is evaluated
        optional<int> op2 = temp;
        std::cout << counter << std::endl; // 2
        std::cout << op2 << std::endl; // 85
    }
    // there are 2 ways to explicitly convert to boost::optional
    {
        using boost::make_optional;
        using optional_utility::map;
        auto const temp = make_optional(42)
            | map([](int num) { return num * 2; });
        // 1. explicit type declaration
        {
            optional<int> const op = temp;
        }
        // 2. using to_optional
        {
            using optional_utility::to_optional;
            auto const op = temp
                | to_optional;
        }
    }
    // non-member functions
    {
        using optional_utility::value_or;
        auto const op = divide(42, std::random_device{}() % 2);
        std::cout << value_or(op, 0) << std::endl; // 42 or 0
    }
    {
        using optional_utility::value_or_throw;
        optional<int> const op = boost::none;
        try {
            // throw any exception if optional is none
            value_or_throw<std::runtime_error>(op, "none!");
        } catch (std::exception const& e) {
            std::cout << e.what() << std::endl; // none!
        }
    }
    // you can use member function pointers
    {
        using optional_utility::map;
        auto const  op = boost::make_optional<std::string>("hello");
        optional<std::string::size_type> op2 = op
            | map(&std::string::substr, 1, 3)
            | map(&std::string::length);
        std::cout << op2 << std::endl;
    }
#if defined(USE_BOOST_LAMBDA)
    {
        using optional_utility::map;
        using optional_utility::filter;
        using optional_utility::to_optional;
        using boost::lambda::_1;
        auto const op = boost::make_optional<std::string>("42")
            | map(_1 + _1)
            | map([](std::string const& s) { return std::stoi(s); })
            | filter(_1 % 2 == 0)
            | to_optional;
        std::cout << op << std::endl;
    }
#endif
}
