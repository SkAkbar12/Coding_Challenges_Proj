#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;
int main(int argc, char* argv[]) {
    cout << "Argument count: " << argc << endl;
    for (int i = 0; i < argc; ++i) {
        cout << "Arg " << i << ": " << argv[i] << endl;
    }
    return 0;
}
