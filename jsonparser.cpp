#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

class JSONValidator {
private:
    string content;
    size_t pos;
    size_t length;

    void skipWhitespace() {
        while (pos < length && isspace(content[pos])) {
            pos++;
        }
    }

    bool parseValue();
    bool parseObject();
    bool parseArray();
    bool parseString();
    bool parseNumber();
    bool parseKeyword(const string& keyword);

public:
    JSONValidator(const string& json) : content(json), pos(0), length(json.length()) {}

    bool isValidJSON() {
        skipWhitespace();
        return parseValue() && pos == length;
    }
};

bool JSONValidator::parseValue() {
    skipWhitespace();
    if (pos >= length) return false;

    char c = content[pos];
    if (c == '{') {
        return parseObject();
    } else if (c == '[') {
        return parseArray();
    } else if (c == '"') {
        return parseString();
    } else if (c == '-' || isdigit(c)) {
        return parseNumber();
    } else if (c == 't') {
        return parseKeyword("true");
    } else if (c == 'f') {
        return parseKeyword("false");
    } else if (c == 'n') {
        return parseKeyword("null");
    }
    return false;
}

bool JSONValidator::parseObject() {
    if (content[pos] != '{') return false;
    pos++;

    skipWhitespace();
    if (content[pos] == '}') {
        pos++;
        return true;
    }

    while (true) {
        skipWhitespace();
        if (!parseString()) return false; // key must be a string

        skipWhitespace();
        if (content[pos] != ':') return false; // expect colon
        pos++;

        if (!parseValue()) return false; // parse value

        skipWhitespace();
        if (content[pos] == '}') {
            pos++;
            return true;
        }
        if (content[pos] != ',') return false; // expect comma or closing brace
        pos++;
    }
}

bool JSONValidator::parseArray() {
    if (content[pos] != '[') return false;
    pos++;

    skipWhitespace();
    if (content[pos] == ']') {
        pos++;
        return true;
    }

    while (true) {
        if (!parseValue()) return false;

        skipWhitespace();
        if (content[pos] == ']') {
            pos++;
            return true;
        }
        if (content[pos] != ',') return false;
        pos++;
    }
}

    bool JSONValidator::parseString() {
        if (content[pos] != '"') return false;
        pos++;

        while (pos < length && content[pos] != '"') {
            if (content[pos] == '\\') { // handle escape sequences
                pos++;
                if (pos >= length) return false;
                // Skip escaped character
                pos++;
            } else {
                pos++;
            }
        }

        if (pos >= length || content[pos] != '"') return false;
        pos++;
        return true;
    }

    bool JSONValidator::parseNumber() {
        size_t start = pos;

        // Optional minus sign
        if (content[pos] == '-') pos++;

        // Integer part
        if (pos < length && content[pos] == '0') {
            pos++;
        } else if (pos < length && isdigit(content[pos])) {
            while (pos < length && isdigit(content[pos])) pos++;
        } else {
            return false;
        }

        // Fractional part
        if (pos < length && content[pos] == '.') {
            pos++;
            if (pos >= length || !isdigit(content[pos])) return false;
            while (pos < length && isdigit(content[pos])) pos++;
        }

        // Exponent part
        if (pos < length && (content[pos] == 'e' || content[pos] == 'E')) {
            pos++;
            if (pos < length && (content[pos] == '+' || content[pos] == '-')) pos++;
            if (pos >= length || !isdigit(content[pos])) return false;
            while (pos < length && isdigit(content[pos])) pos++;
        }

        return pos > start; // At least one digit was read
    }

bool JSONValidator::parseKeyword(const string& keyword) {
    if (content.compare(pos, keyword.length(), keyword) == 0) {
        pos += keyword.length();
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename.json>\n";
        return 1;
    }

    string filename = argv[1];
    
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Failed to open file '" << filename << "'\n";
        return 1;
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    if (content.empty()) {
        cerr << "Error: File is empty!\n";
        return 1;
    }

    JSONValidator validator(content);
    bool isValid = validator.isValidJSON();

    cout << "File: " << filename << "\n";
    cout << "Size: " << content.size() << " bytes\n";
    cout << "JSON Valid: " << (isValid ? "YES" : "NO") << "\n";

    if (!isValid) {
        cout << "\nFile content:\n" << content << "\n";
    }

    return isValid ? 0 : 1;
}