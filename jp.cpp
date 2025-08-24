#include <iostream>
#include <cctype>
#include <fstream>
#include <string>

using namespace std;

class JSONValidator{
private:
    string content;
    size_t pos;
    size_t end;

    void whitespace(){
        while(pos < end && isspace(content[pos])) {
            ++pos;
        }
    }
    bool parseValue();
    bool parseObject();
    bool parseArray();
    bool parseString();
    bool parseNumber();
    bool parseKeyword(const string& keyword);

    

public:
    
    JSONValidator(const string& json):content(json),pos(0),end(json.size()-1){}
    bool isvalidjson(){
        whitespace();
        return parseValue() && pos==end;
    }
};

bool JSONValidator::parseValue() {
    if(pos>=end) return false;
    whitespace();

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

bool JSONValidator::parseObject(){
    
    
    if(content[pos]!='{') return false;
    pos++;
    whitespace();
    if(content[pos]=='}'){
        pos++;
        return true;
    }

    while(true){
        whitespace();
        if(!parseString()){
            return false;
        }
        whitespace();
        if(!content[pos]==':') return false;
        pos++;
    
        if(!parseValue) return false;
        
        whitespace();
        if(content[pos]=='}') {
            pos++;
            return true;
        }

        return false;
    }
}

bool JSONValidator::parseArray(){
    if(content[pos]!='[') return false;
    pos++;
    whitespace();
     if (content[pos] == ']') {
        pos++;
        return true;
    }
    while(true){
        if(!parseValue()) return false;
        pos++;
        whitespace();
        if (content[pos] == ']') {
        pos++;
        return true;
    }
    if(content[pos]!=']') return false;

    pos++;
    }
}

bool JSONValidator::parseString(){
    if(content[pos]!='"') return false;
    pos++;
    whitespace();
    if(content[pos]=='"') {
        pos++;
        return true;
    }
     while (pos < end && content[pos] != '"') {
            if (content[pos] == '\\') { // handle escape sequences
                pos++;
                if (pos >= end) return false;
                // Skip escaped character
                pos++;
            } else {
                pos++;
            }
        }

        if (pos >= end || content[pos] != '"') return false;
        pos++;
        return true;
    }


    
    bool JSONValidator::parseNumber() {
        size_t start = pos;

        // Optional minus sign
        if (content[pos] == '-') pos++;

        // Integer part
        if (pos <= end && content[pos] == '0') {
            pos++;
        } else if (pos <= end && isdigit(content[pos])) {
            while (pos <= end && isdigit(content[pos])) pos++;
        } else {
            return false;
        }

        // Fractional part
        if (pos <= end && content[pos] == '.') {
            pos++;
            if (pos > end || !isdigit(content[pos])) return false;
            while (pos <= end && isdigit(content[pos])) pos++;
        }

        // Exponent part
        if (pos <= end && (content[pos] == 'e' || content[pos] == 'E')) {
            pos++;
            if (pos <= end && (content[pos] == '+' || content[pos] == '-')) pos++;
            if (pos > end || !isdigit(content[pos])) return false;
            while (pos <= end && isdigit(content[pos])) pos++;
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

int main(int argc, char* argv[]){
    istream* in=nullptr;
    string json;
    ifstream file;

if(argc==1) {
    in=&cin;
}
else{
    file.open(argv[1]);
    if(!file.is_open()){
        cerr<<"file not exist enter data : "<<endl;
        in=&cin;
    }else{
    in = &file;
    }

}

json.assign(istreambuf_iterator<char>(*in), istreambuf_iterator<char>());
if(json.size()==0) return false;
JSONValidator validate(json);
bool isjson=validate.isvalidjson();

cout << "File: " << argv[1] << "\n";
cout << "Size: " << json.size() << " bytes\n";
cout << "JSON Valid: " << (isjson ? "YES" : "NO") << "\n";
 if (!isjson) {
        cout << "\nFile content:\n" << json << "\n";
    }

    return isjson ? 0 : 1;
}