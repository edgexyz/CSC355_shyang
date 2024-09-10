#include <iostream>
#include <regex>

// Due to how C++ (and many other languages) handle escape sequences,
// we need to add an extra backslash (\) in some cases. For example,
// "\\s" not just "\s" for whitespace.
bool isValidEmail(const std::string& email) {
    const std::regex pattern("([A-Za-z_]|[0-9])+((-|\.)?([A-Za-z_]|[0-9])+)+@([A-Za-z_]|[0-9])+(\.([A-Za-z_]|[0-9])+)+");

    return std::regex_match(email, pattern);
}

bool isValidPhoneNumber(const std::string& phoneNumber) {
    const std::regex pattern("\([0-9]{3}\)([0-9]{3}\.|(\\s|-)?[0-9]{3}(\\s|-)?)[0-9]{4}");

    return std::regex_match(phoneNumber, pattern);
}

bool isValidIntDeclaration(const std::string& declaration) {
    const std::regex pattern("(int)\\s+[A-Za-z_]((\\s)*=(\\s)*[0-9]+)?;");

    return std::regex_match(declaration, pattern);
}

void testRegex4Email() {

    std::string tests[10] = {
        "username@domain.com",              // Valid.
        "first.lastname@domain.com",        // Valid.
        "123first.lastname@domain.com",     // Valid.
        "first.lastname123@domain.com",     // Valid.
        "f1rst-lastname.0@domain.co.uk",    // Valid.
        "username!@domain.com",             // Not valid.
        "user$name@domain.com",             // Not valid.
        "username@domaincom",               // Not valid.
        "username@domain,com",              // Not valid.
        "(user)name@domain.com"             // Not valid.
    };

    for (int i = 0; i < 10 ; i++) {
        if (isValidEmail(tests[i])) {
            std::cout << "\"" << tests[i] << "\" is a valid email address." << std::endl;
        } else {
            std::cout << "\"" << tests[i] << "\" is not a valid email address." << std::endl;
        }
    }
}

void testRegex4PhoneNumber() {

    std::string tests[10] = {
        "(123) 456 7890",       // Valid.
        "(123)456 7890",        // Valid.
        "(123) 456-7890",       // Valid.
        "(123)456.7890",        // Valid.
        "(I23)456 7890",        // Not valid.
        "123) 456-7890",        // Not valid.
        "123 456.7890",         // Not valid.
        "(123) 456.7890.",      // Not valid.
        "(123).456-7890",       // Not valid.
        "(1233) 4566-8900"      // Not valid.
    };

    for (int i = 0; i < 10 ; i++) {
        if (isValidPhoneNumber(tests[i])) {
            std::cout << "\"" << tests[i] << "\" is a valid phone number." << std::endl;
        } else {
            std::cout << "\"" << tests[i] << "\" is not a valid phone number" << std::endl;
        }
    }
}

void testRegex4IntDeclaration() {

    std::string tests[5] = {
        "int x;",                   // Valid.
        "int x = 10;",              // Valid.
        "int x =;",                 // Not valid.
        "int x           =   10;",  // Valid.
        "int x = 10"                // Not valid.
    };

    for (int i = 0; i < 5 ; i++) {
        if (isValidIntDeclaration(tests[i])) {
            std::cout << "\"" << tests[i] << "\" is a valid declaration." << std::endl;
        } else {
            std::cout << "\"" << tests[i] << "\" is not a valid delcaration" << std::endl;
        }
    }
}

int main() {

    testRegex4Email();

    testRegex4PhoneNumber();

    testRegex4IntDeclaration();

    return 0;
}
