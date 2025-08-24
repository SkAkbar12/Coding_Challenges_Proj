#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <regex>

// Function to split a string by delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);
    
    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + 1;
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));
    return tokens;
}

// Function to join a container of strings
template<typename T>
std::string join(const T& container, const std::string& delimiter) {
    std::string result;
    for (auto it = container.begin(); it != container.end(); ++it) {
        if (it != container.begin()) result += delimiter;
        result += *it;
    }
    return result;
}

int main() {
    // 1. Iterators and STL algorithms
    std::string str = "Hello, World!";
    std::reverse(str.begin(), str.end());
    std::cout << "Reversed: " << str << "\n";

    // 2. Numeric conversions
    std::string numStr = "123.45";
    double num = std::stod(numStr);
    std::cout << "As double: " << num * 2 << "\n";

    // 3. String stream
    std::stringstream ss;
    ss << "The answer is " << 42 << "!";
    std::cout << "String stream: " << ss.str() << "\n";

    // 4. Regular expressions
    std::regex date_regex(R"((\d{2})/(\d{2})/(\d{4}))");
    std::string date = "Today is 07/21/2023";
    std::smatch match;
    if (std::regex_search(date, match, date_regex)) {
        std::cout << "Date found: " << match[0] << "\n";
        std::cout << "Day: " << match[1] << ", Month: " << match[2] 
                  << ", Year: " << match[3] << "\n";
    }

    // 5. Split and join
    std::string csv = "apple,orange,banana";
    std::vector<std::string> fruits = split(csv, ',');
    std::cout << "Split fruits:\n";
    for (const auto& fruit : fruits) {
        std::cout << "- " << fruit << "\n";
    }

    std::string joined = join(fruits, " | ");
    std::cout << "Joined fruits: " << joined << "\n";

    // 6. String view (C++17)
    #if __cplusplus >= 201703L
    std::string_view sv = "String view example";
    std::cout << "String view substring: " << sv.substr(7, 4) << "\n";
    #endif

    return 0;
}