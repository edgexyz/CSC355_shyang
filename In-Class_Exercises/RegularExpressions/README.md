# Regular Expression Exercise

In this exercise, you will complete three regular expression patterns to check if the patterns accept the input strings.

To use regular expressions in C++, we need to import `regex` library: `#include <regex>`.

To define a regular expression in our C++ program, we need to declare a constant variable type regex, e.g., `const std::regex pattern("[0-9]+");`
This example will declare a constant variable named `pattern` for a regular expression recognizing one or more digits.

`std::regex_match(string, regex)` receives two arguments: (1) input string to test and (2) regex constant variable. The function will return a boolean result, i.e., true if the defined regular expression recognizes the input string; otherwise, false.
E.g., `std::regex_match("1234", pattern)` will return true as "1234" is recognized by the regular expression `[0-9]+`.

Due to how C++ handles [escape sequnces](https://en.wikipedia.org/wiki/Escape_sequence), e.g., `\n`, `\t`, `\s`, etc., we need to add extra backslash (`\`) in some regular expressions. For example,  `\\s`, not just `\s` for whitespace.

There are three functions: "isValidEmail", "isValidPhoneNumber", and "isValidIntDeclaration".

## isValidEmail
isValidEmail function checks if the input string has a valid email syntax. E.g., username@domain_name.tld, user.name@domain_name.co.uk. A username can have letters, numbers, underscore, period/dot, and hyphen. A domain_name can have letters, numbers, and hyphen. A top-level domain (tld) can have letters, numbers, hyphen, and period.

## isValidPhoneNumber
isValidPhoneNumber function checks if the input string has a valid phone number syntax. E.g., (123) 456 7890. The first three numbers must be between the parentheses; there can be zero or one space, three numbers, zero or one space or period or hyphen, and four numbers.

## isValidIntDeclaration
isValidIntDeclaration checks if the C/C++ integer declaration syntax is valid. E.g., int x = 10; int x;. There must be one or more whitespaces after int, e.g., intx; is not valid while int     x; is valid. There can be initialization or not.

There are test cases for each function in the provided starter code.

## How to Run
```
$g++ -o regex regex.cpp
$./regex
```

## Useful Resources:
More on [C++ Regular Expressions Library](https://en.cppreference.com/w/cpp/regex).

A useful online tool to test your regular expression: [regex101](https://regex101.com/).
