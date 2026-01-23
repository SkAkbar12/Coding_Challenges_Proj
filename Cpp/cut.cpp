#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Usage: " << argv[0] 
             << " -c<col_index> -r<row_limit> -d<delimiter> <filename>\n";
        return 1;
    }

    int colIndex = -1;
    int rowLimit = -1;
    char delimiter = ' ';  // default delimiter = space
    string filename;

    // Parse args
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg.rfind("-c", 0) == 0) {
            colIndex = stoi(arg.substr(2)); // e.g. -c1 → 1
        } else if (arg.rfind("-r", 0) == 0) {
            rowLimit = stoi(arg.substr(2)); // e.g. -r5 → 5
        } else if (arg.rfind("-d", 0) == 0) {
            if (arg.size() > 2) delimiter = arg[2]; 
            else delimiter = ' '; // -d with nothing = space
        } else {
            filename = arg; // assume last argument is file
        }
    }

    if (colIndex < 1 || rowLimit < 1 || filename.empty()) {
        cerr << "Error: Missing required options\n";
        return 1;
    }

    ifstream in(filename);
    if (!in) {
        cerr << "Can't open file: " << filename << "\n";
        return 1;
    }

    string line;
    int rowCount = 0;
    while (getline(in, line) && rowCount < rowLimit) {
        istringstream iss(line);
        string word;
        int col = 0;

        while (getline(iss, word, delimiter)) {
            if (col == colIndex - 1) {
                cout << word << "\n";
                break;
            }
            col++;
        }
        rowCount++;
    }

    return 0;
}
