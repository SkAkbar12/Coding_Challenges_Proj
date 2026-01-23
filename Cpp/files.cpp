#include <iostream>
#include <fstream>
#include <string>
#include <sstream>  // You missed this include, required for istringstream

using namespace std;

i/*nt main() {
    ifstream file("example.txt");  // Open the file for reading
    string line;
    int c=0;

    while (getline(file, line)) {  // Read one line at a time
        // cout << "Line read: " << line << endl;

        istringstream iss(line);  // Convert the line to a stream to split words

        string word;
        while (iss >> word) {     // Extract words from the line
            cout << "Word: " << word << endl;
            c++;
        }
        cout<<" "<<c;
    }

    file.close();  // Close the file
    return 0;
}*/


using namespace std;

int main() {
    ifstream file("example.txt");

    istream_iterator<string> start(file);  // start reading words
    istream_iterator<string> end;          // end marker

    int wordCount = distance(start, end);  // count how many words
    //or
    /*int count = 0;
while (start != end) {
    ++count;
    ++start;
}
*/
    cout << "Total number of words: " << wordCount << endl;

    return 0;
}

