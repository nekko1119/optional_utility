# optional_utility

Header only, `boost::optional` 's utilities.

## Features

 * *map* and *flat map*. These interfaces are like `boost::range::adaptor`
 * Non-member functions, `value`, `value_or` and `value_or_eval`
 * Non-member function, `value_or_throw`. This throws whatever exception if `boost::optional` is `boost::none`;

## Install

Just through the path to your build tree.

## Dependencies (Requires)

 * C++11 or newer
 * boost-1.56.0 or newer

## Tested Compilers

 * gcc 4.7.3 4.8.1 4.8.2 4.9.0 4.9.1 4.9.2
 * clang 3.1 3.2 3.3 3.4 3.5
 * msvc 12.0 12.CTP 14.0PR

## Usage Example

```cpp

#include "optional_utility.hpp"
#include <boost/optional/optional_io.hpp>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
using boost::optional;

// returns joined strings if both strings has a value. otherwise none;
optional<std::string> full_name(optional<std::string> const& first_name, optional<std::string> const& last_name)
{
    using optional_utility::flat_mapped;
    using optional_utility::mapped;
    return last_name | flat_mapped([&first_name](std::string const& ln) {
        return first_name | mapped([&ln](std::string const& fn) {
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
    {
        // example of `mapped` and `flat_mapped`
        auto const first_name = boost::make_optional<std::string>("John");
        auto const last_name = boost::make_optional<std::string>("Smith");
        std::cout << full_name(first_name, last_name) << std::endl; // John Smith
        std::cout << full_name(first_name, boost::none) << std::endl; // --
        std::cout << full_name(boost::none, last_name) << std::endl; // --
        std::cout << full_name(boost::none, boost::none) << std::endl; // -- 
    }
    {
        // gets held value or default value
        using optional_utility::value_or;
        auto const op = divide(42, std::random_device{}() % 2);
        std::cout << value_or(op, 0) << std::endl; // 42 or 0
    }
    {
        using optional_utility::value_or_throw;
        optional<int> const op = boost::none;
        try {
            // you can throw whatever exception if optional is none
            value_or_throw<std::runtime_error>(op, "none!");
        } catch (std::exception const& e) {
            std::cout << e.what() << std::endl;
        }
    }
}

```

## TODO

 * `filtered`
 * lazy evaluation
 * support member function pointer