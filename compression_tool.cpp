#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

// Node structure for Huffman Tree
struct Node {
    char ch;         // Character stored in the node
    int freq;        // Frequency of the character
    Node *left, *right; // Left and right child
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparison struct for priority queue (min-heap)
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq; // Min-heap based on frequency
    }
};

// Compression class to handle building Huffman tree and encoding
class Compression {
private:
    string content; // Original content to compress

public:
    Compression(const string& in) : content(in) {}
    void buildtreeAndencode(string filename); // Function to build tree & encode data
};

// Recursive function to generate Huffman codes
void generateCodes(Node* root, string s, unordered_map<char, string> &huffmanCode) {
    if (!root) return;
    // Leaf node condition
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = s;
        cout << "Character: '" << (root->ch == '\n' ? "\\n" : string(1, root->ch)) 
             << "' Frequency: " << root->freq << " Code: " << s << endl;
    }
    generateCodes(root->left, s + '0', huffmanCode);
    generateCodes(root->right, s + '1', huffmanCode);
}

// Function to delete Huffman tree (free memory)
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// Function to write Huffman codes and encoded data to file
void writetofile(const unordered_map<char, string>& huffmanCode, const string& encodedStr, const string& filename) {
    ofstream out(filename + ".huff", ios::binary);
    if (!out) {
        cerr << "Unable to open output file" << endl;
        return;
    }

    // Write number of codes
    out << huffmanCode.size() << endl;
    
    // Write each character and its Huffman code
    for (auto &code : huffmanCode) {
        out << (int)code.first << " " << code.second << endl;
    }
    
    // Separator for encoded data
    out << "DATA" << endl;
    out << encodedStr;
    out.close();
    
    cout << "File compressed and saved as: " << filename << ".huff" << endl;
    cout << "Compressed size: " << (encodedStr.length() + 7) / 8 << " bytes" << endl;
}

// Function to build Huffman tree and encode content
void Compression::buildtreeAndencode(string filename) {
    // Build frequency map for each character
    unordered_map<char, int> freqMap;
    for (char c : content) {
        freqMap[c]++;
    }

    cout << "Character frequencies:" << endl;
    for (auto &pair : freqMap) {
        cout << "'" << (pair.first == '\n' ? "\\n" : string(1, pair.first)) 
             << "': " << pair.second << " times" << endl;
    }

    // Create priority queue (min-heap)
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto &pair : freqMap) {
        pq.push(new Node(pair.first, pair.second));
    }

    cout << "Building Huffman tree with " << pq.size() << " nodes..." << endl;

    // Build Huffman tree
    while (pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        Node *parent = new Node('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    if (pq.empty()) {
        cerr << "Error: No data to compress" << endl;
        return;
    }

    Node* root = pq.top();
    unordered_map<char, string> huffmanCode;
    
    cout << "Generating Huffman codes..." << endl;
    generateCodes(root, "", huffmanCode);

    // Encode the content using generated codes
    string encodedStr;
    for (char c : content) {
        encodedStr += huffmanCode[c];
    }

    cout << "Original text: " << content.length() * 8 << " bits" << endl;
    cout << "Encoded text: " << encodedStr.length() << " bits" << endl;
    if (!encodedStr.empty()) {
        cout << "Compression ratio: " << (float)(content.length() * 8) / encodedStr.length() << ":1" << endl;
    }

    // Write to compressed file
    writetofile(huffmanCode, encodedStr, filename);
    
    // Clean up memory
    deleteTree(root);
}

// Function to rebuild Huffman tree from saved codes (for decompression)
Node* rebuildtree(unordered_map<char,string>& huffman){
    Node* root = new Node('\0', 0);
    for(auto &pair : huffman){
        Node* curr = root;
        for(char bit : pair.second){
            if(bit == '0'){
                if(!curr->left){
                    curr->left = new Node('\0', 0);
                }
                curr = curr->left;
            } else {
                if(!curr->right){
                    curr->right = new Node('\0', 0);
                }
                curr = curr->right;
            }
        }
        curr->ch = pair.first;
    }
    return root;
}

// Decode Huffman encoded string using tree
string decode(string str, Node* root){
    string decoded = "";
    Node* current = root;
    for (char bit : str) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        
        // Leaf node reached
        if (!current->left && !current->right) {
            decoded += current->ch;
            current = root;
        }
    }
    return decoded;
}

// Function to read compressed file and decode it
void readanddecode(string filename){
    ifstream in(filename, ios::binary);
    if(!in) {
        cerr << "Unreadable data";
        return;
    }
    
    int n;
    in >> n;
    in.ignore();
    
    unordered_map<char, string> huffman;
    for(int i = 0; i < n; i++){
        int charCode;
        string code;
        in >> charCode >> code;
        huffman[(char)charCode] = code;
        in.ignore();
    }

    // Skip separator line
    string separator;
    getline(in, separator);
    
    // Read encoded string
    string encodedStr;
    getline(in, encodedStr);
    
    in.close();

    Node* root = rebuildtree(huffman);
    string decodedText = decode(encodedStr, root);

    // Save decoded content to file
    string outputFile = filename.substr(0, filename.find_last_of('.')) + ".decoded.txt";
    ofstream out(outputFile);
    out << decodedText;
    out.close();
    cout << "File decoded and saved as: " << outputFile << "\n";
}

// Main function: Handles compress & decompress modes
int main(int argc, char* argv[]) {
    cout << "Program started..." << endl;
    
    // Check command-line arguments
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " compress <filename>" << endl;
        cerr << "       " << argv[0] << " decompress <filename.huff>" << endl;
        return 1;
    }

    string mode = argv[1];
    string filename = argv[2];

    if (mode == "compress") {
        // Read file content
        ifstream in(filename);
        if (!in) {
            cerr << "Unable to open file: " << filename << endl;
            return 1;
        }

        string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
        in.close();

        cout << "Reading file: " << filename << endl;
        cout << "File size: " << content.length() << " characters" << endl;

        if (content.empty()) {
            cerr << "Error: File is empty" << endl;
            return 1;
        }

        Compression com(content);
        com.buildtreeAndencode(filename);
        cout << "Compression completed successfully!" << endl;
        
    } else if(mode == "decompress") {
        // Decompress file
        readanddecode(filename);
        
    } else {
        cerr << "Only 'compress/decompress' mode is supported" << endl;
        return 1;
    }
    
    return 0;
}
