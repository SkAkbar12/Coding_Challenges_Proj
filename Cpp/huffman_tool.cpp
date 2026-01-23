#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

// Node structure for Huffman Tree
struct Node {
    char ch;
    int freq;
    Node *left, *right;
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparator for min-heap
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// Generate Huffman codes recursively
void generateCodes(Node* root, string code, unordered_map<char, string> &huffmanCode) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = code;
    }
    generateCodes(root->left, code + "0", huffmanCode);
    generateCodes(root->right, code + "1", huffmanCode);
}

// Build Huffman Tree and return encoded string
string encode(const string &text, unordered_map<char, string> &huffmanCode, Node* &root) {
    // Frequency map
    unordered_map<char, int> freq;
    for (char ch : text) freq[ch]++;

    // Min-heap
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Build tree
    while (pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        Node *newNode = new Node('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }

    root = pq.top();

    // Generate codes
    generateCodes(root, "", huffmanCode);

    // Encode text
    string encodedStr = "";
    for (char ch : text) {
        encodedStr += huffmanCode[ch];
    }

    return encodedStr;
}

// Decode encoded string using Huffman Tree
string decode(const string &encodedStr, Node* root) {
    string decoded = "";
    Node* current = root;
    for (char bit : encodedStr) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        
        if (!current->left && !current->right) {
            decoded += current->ch;
            current = root;
        }
    }
    return decoded;
}

// Write encoded data to file
void writeEncodedFile(const string &filename, const string &encodedStr, const unordered_map<char, string> &huffmanCode) {
    ofstream out(filename + ".huff", ios::binary);
    if (!out) {
        cerr << "Error: Could not open file for writing.\n";
        return;
    }

    // Write frequency table (character and frequency)
    out << huffmanCode.size() << "\n";
    for (auto &pair : huffmanCode) {
        out << (int)pair.first << " " << pair.second << "\n";
    }

    // Write encoded string length and the string itself
    out << encodedStr.length() << "\n";
    out << encodedStr;
    out.close();
    cout << "File compressed and saved as: " << filename << ".huff\n";
}

// Rebuild Huffman Tree from frequency table
Node* rebuildTree(const unordered_map<char, string> &huffmanCode) {
    Node* root = new Node('\0', 0);
    
    for (auto &pair : huffmanCode) {
        Node* current = root;
        for (char bit : pair.second) {
            if (bit == '0') {
                if (!current->left) {
                    current->left = new Node('\0', 0);
                }
                current = current->left;
            } else {
                if (!current->right) {
                    current->right = new Node('\0', 0);
                }
                current = current->right;
            }
        }
        current->ch = pair.first;
    }
    
    return root;
}

// Read encoded data and decode it
void readAndDecodeFile(const string &filename) {
    ifstream in(filename, ios::binary);
    if (!in) {
        cerr << "Error: Could not open file for reading.\n";
        return;
    }

    // Read frequency table size
    int n;
    in >> n;
    in.ignore(); // Skip newline

    unordered_map<char, string> huffmanCode;
    for (int i = 0; i < n; i++) {
        int charCode;
        string code;
        in >> charCode >> code;
        huffmanCode[(char)charCode] = code;
        in.ignore(); // Skip newline
    }

    // Read encoded string length
    int encodedLength;
    in >> encodedLength;
    in.ignore(); // Skip newline

    // Read encoded string
    string encodedStr;
    getline(in, encodedStr);
    in.close();

    // Rebuild Huffman Tree
    Node* root = rebuildTree(huffmanCode);

    // Decode content
    string decodedText = decode(encodedStr, root);

    // Save decoded file
    string outputFile = filename.substr(0, filename.find_last_of('.')) + ".decoded.txt";
    ofstream out(outputFile);
    out << decodedText;
    out.close();
    cout << "File decoded and saved as: " << outputFile << "\n";
}

// Main program
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage:\n";
        cout << "  To compress: " << argv[0] << " compress <filename>\n";
        cout << "  To decompress: " << argv[0] << " decompress <filename.huff>\n";
        return 1;
    }

    string mode = argv[1];
    string filename = argv[2];

    if (mode == "compress") {
        ifstream in(filename);
        if (!in) {
            cerr << "Error: Could not open file for reading.\n";
            return 1;
        }

        string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
        in.close();

        unordered_map<char, string> huffmanCode;
        Node* root = nullptr;
        string encodedStr = encode(content, huffmanCode, root);
        writeEncodedFile(filename, encodedStr, huffmanCode);

    } else if (mode == "decompress") {
        readAndDecodeFile(filename);
    } else {
        cerr << "Invalid mode! Use 'compress' or 'decompress'.\n";
        return 1;
    }

    return 0;
}