# optional_utility

Header only, `boost::optional` 's utilities.

## Features

 * *Boost Range Adaptors*-like interfaces; *map*, *flat map* and *filter*
 * Adaptors are lazy evaluation
 * You can use member functions intead of lambda functions
 * Non-member functions, `value`, `value_or` and `value_or_eval`
 * Non-member function, `value_or_throw`. This throws exception you pass if `boost::optional` is `boost::none`;

## Install

Just add the `optional_utility.hpp` to your build tree.

## Dependencies (Requires)

 * C++11 or newer
   - You need to define `OPTIONAL_UTILITY_NO_CXX14_RETURN_TYPE_DEDUCTION` macro if you compile c++11 mode. e.g. `-std=c++11`
 * boost-1.56.0 or newer

## Tested Compilers

 * gcc
   - 4.9.0 (all of 11, 1y)
   - 4.9.1 (all of 11, 1y)
   - 4.9.2 (all of 11, 1y)
   - 5.1.0 (all of 11, 14, 1z)
   - 5.2.0 (all of 11, 14, 1z)
 * clang
   - 3.4 (all of 11, 1y)
   - 3.5 (all of 11, 14, 1z)
 * msvc
   - 14.0

gcc and clang was tested on [Wandbox](http://melpon.org/wandbox/)

## Usage Example

(In all of the examples, assume that threre are appropriate *using directive*s or `using namespace`s)

 * Join adaptors to another using `operator|`. `<optional_value> | <adaptor>(<function>) | ...`

 * map

```cpp

optional<int> op = 42;
optional<int> result = op
    | map([](int i) { return i * 2; }); // result is 84

```

 * flat_map

```cpp

optional<int> op = 42;
optional<int> result = op
    | flat_map([](int i) { return i % 2 ? make_optional(i) : none; }); // result is 42

```

 * filter

```cpp

optional<int> op = 42;
optional<int> result = op
    | map([](int i) { return i + 1; })
    | filter([](int i) { return i % 2; }); // result is none

```

 * You can use `to_optional` if you want to convert to `optional` explicitly

``` cpp

optional<int> op = 42;
optional<int> result;
result = op
    | map([](int i) { return -i; })
    | to_optional; // result is -42

```

 * It can not assign

```cpp

optional<int> op = 42;
optional<int> result;
result = op
    | map([](int i) { return -i; }); // error

```

 * You can use member function pointers

```cpp

optional<string> op = "hello"s;
optional<string::size_type> result = op
    | map(&string::length); // result is 5

```

 * You can pass arguments

```cpp

optional<string> op = "hello"s;
optional<std::string> result = op
    | map(&string::substr, 1, 3); // result is "ell"

```
## Customize

If you define `#define OPTIONAL_UTILITY_NAMESPACE op`, optional_utilitiy's namespace is `op`. (default is `optional_utility`)

## TODO

 * Enough tests
 * The End of adaptors; `value`, `value_or`, `value_or_eval` and `value_or_throw`