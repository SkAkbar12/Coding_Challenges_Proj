#include <iostream>
#include <string>

int main() {
    // 1. Substring
    std::string str = "Hello, World!";
    std::string sub = str.substr(7, 5);
    std::cout << "Substring: " << sub << "\n"; // "World"

    // 2. Searching
    size_t found = str.find("World");
    if (found != std::string::npos) {
        std::cout << "'World' found at position: " << found << "\n";
    }

    // 3. Comparison
    std::string str2 = "Hello, C++!";
    int result = str.compare(str2);
    if (result == 0) {
        std::cout << "Strings are equal\n";
    } else if (result < 0) {
        std::cout << "str comes before str2\n";
    } else {
        std::cout << "str comes after str2\n";
    }

    // 4. Modifiers
    str.insert(7, "Beautiful ");
    std::cout << "After insert: " << str << "\n";

    str.replace(7, 9, "Amazing");
    std::cout << "After replace: " << str << "\n";

    str.erase(7, 7);
    std::cout << "After erase: " << str << "\n";


    // Method 1: Using stringstream
std::vector<std::string> tokenize_stream(const std::string& str, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delim)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

// Method 2: Using find and substr
std::vector<std::string> tokenize_find(const std::string& str, char delim) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delim);
    
    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delim, start);
    }
    
    tokens.push_back(str.substr(start));
    return tokens;
}

// Method 3: Using STL algorithms
std::vector<std::string> tokenize_stl(const std::string& str, char delim) {
    std::vector<std::string> tokens;
    auto start = str.begin();
    auto end = std::find(start, str.end(), delim);
    
    while (end != str.end()) {
        tokens.emplace_back(start, end);
        start = end + 1;
        end = std::find(start, str.end(), delim);
    }
    
    tokens.emplace_back(start, end);
    return tokens;
}


    std::string text = "apple,orange,banana,grape,melon";
    
    auto tokens1 = tokenize_stream(text, ',');
    auto tokens2 = tokenize_find(text, ',');
    auto tokens3 = tokenize_stl(text, ',');


    return 0;
}