# optional_utility

Header only, `boost::optional` 's utilities.

## Features

 * *map*, *flat map* and *filter*. these interfaces, such as *Boost Range Adaptors*
 * Adaptors are lazy evaluation
 * You can use member functions intead of lambda functions
 * Non-member functions, `value`, `value_or` and `value_or_eval`
 * Non-member function, `value_or_throw`. This throws whatever exception if `boost::optional` is `boost::none`;

## Install

Just through the path to your build tree.

## Dependencies (Requires)

 * C++14 or newer
 * boost-1.56.0 or newer

## Tested Compilers

 * gcc
   - 4.9.0
   - 4.9.1
   - 4.9.2
 * clang
   - 3.4
   - 3.5
 * msvc
   - 14.0PR

## Usage Example

(In all of the examples, assume that threre are appropriate *using directive*s or `using namespace`s)

 * Join adaptors to another using `operator|`. These interfaces, such as `<optional_value> | <adaptor>(<function>) | ...`
 * The result is 84

```cpp

optional<int> op = 42;
optional<int> result = op
    | mapped([](int i) { return i * 2; });

```

 * The result is 42

```cpp

optional<int> op = 42;
optional<int> result = op
    | flat_mapped([](int i) { return i % 2 ? make_optional(i) : none; });

```

 * The result is none

```cpp

optional<int> op = 42;
optional<int> result = op
    | mapped([](int i) { return i + 1; })
    | filtered([](int i) { return i % 2; });

```

 * You can use `to_optional` if you want to convert to `optional` explicitly
 * The result is -42

``` cpp

optional<int> op = 42;
optional<int> result;
result = op
    | mapped([](int i) { return -i; })
    | to_optional();

```

 * This is error

```cpp

optional<int> op = 42;
optional<int> result;
result = op
    | mapped([](int i) { return -i; }); // error

```

 * You can use member function pointer
 * The result is 5

```cpp

optional<string> op = "hello"s;
optional<string::size_type> result = op
    | mapped(&string::length);

```

 * The result is "ell"

```cpp

optional<string> op = "hello"s;
optional<std::string> result = op
    | mapped(&string::substring, 1, 3);

```