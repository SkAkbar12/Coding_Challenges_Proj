#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
using namespace std;

int main() {
    string text = "ggfysgdfsyfuvg vyus yusdysu gsyufg yufgf gsyudg uy gyuf gyufgy syuf\n"
                  "nhfkd dfc dds khsdkuch jh dshfgsdj gdfgdsgsgc\n"
                  "chjsgcjsagfjyuc ghjghjcgdjcg djcjcgdjcgjcjcghjcgcgc";
    
    istringstream in(text);  // make a stream from string
    
    string line;
    while (getline(in, line)) {
        cout << "Line: " << line << endl;

        // Wrap line in its own stream
        istringstream linestream(line);

        // Create iterators
        istream_iterator<string> start(linestream);
        istream_iterator<string> end;

        // Print words
        while (start != end) {
            cout << *start << " ";
            ++start;
        }
        cout << "\n";
    }
}
