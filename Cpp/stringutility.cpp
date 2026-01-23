#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <regex>

class StringUtils {
public:
    // Trim whitespace from left
    static std::string ltrim(const std::string &s) {
        auto it = std::find_if(s.begin(), s.end(), [](char c) {
            return !std::isspace(c);
        });
        return std::string(it, s.end());
    }

    // Trim whitespace from right
    static std::string rtrim(const std::string &s) {
        auto it = std::find_if(s.rbegin(), s.rend(), [](char c) {
            return !std::isspace(c);
        });
        return std::string(s.begin(), it.base());
    }

    // Trim whitespace from both ends
    static std::string trim(const std::string &s) {
        return rtrim(ltrim(s));
    }

    // Convert to lowercase
    static std::string toLower(const std::string &s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    // Convert to uppercase
    static std::string toUpper(const std::string &s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    // Replace all occurrences
    static void replaceAll(std::string &s, const std::string &from, const std::string &to) {
        size_t pos = 0;
        while ((pos = s.find(from, pos)) != std::string::npos) {
            s.replace(pos, from.length(), to);
            pos += to.length();
        }
    }

    // Check if string starts with prefix
    static bool startsWith(const std::string &s, const std::string &prefix) {
        return s.rfind(prefix, 0) == 0;
    }

    // Check if string ends with suffix
    static bool endsWith(const std::string &s, const std::string &suffix) {
        if (suffix.size() > s.size()) return false;
        return std::equal(suffix.rbegin(), suffix.rend(), s.rbegin());
    }

    // Split string by delimiter
    static std::vector<std::string> split(const std::string &s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Join strings with delimiter
    static std::string join(const std::vector<std::string> &strings, const std::string &delimiter) {
        std::ostringstream oss;
        for (size_t i = 0; i < strings.size(); ++i) {
            if (i != 0) oss << delimiter;
            oss << strings[i];
        }
        return oss.str();
    }
};

int main() {
    std::string test = "   Hello, World!   ";
    std::cout << "Original: '" << test << "'\n";
    std::cout << "Trimmed: '" << StringUtils::trim(test) << "'\n";
    std::cout << "Lower: '" << StringUtils::toLower(test) << "'\n";
    std::cout << "Upper: '" << StringUtils::toUpper(test) << "'\n";

    std::string text = "I like apples and apples are great";
    StringUtils::replaceAll(text, "apples", "oranges");
    std::cout << "Replaced: " << text << "\n";

    std::cout << "Starts with 'I': " << std::boolalpha 
              << StringUtils::startsWith(text, "I") << "\n";
    std::cout << "Ends with 'great': " << StringUtils::endsWith(text, "great") << "\n";

    std::vector<std::string> words = StringUtils::split(text, ' ');
    std::cout << "Split into " << words.size() << " words\n";
    std::cout << "Joined with '-': " << StringUtils::join(words, "-") << "\n";

    return 0;
}