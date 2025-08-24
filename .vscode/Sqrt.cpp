#include <iostream>
using namespace std;

class SquareRoot {
public:
    // Binary Search Method for Square Root
    double sqrtBinarySearch(double number, double precision = 1e-6) {
        if (number < 0) return -1; // sqrt of negative number is undefined
        if (number == 0 || number == 1) return number;

        double low = 0, high = number;
        if (number < 1) high = 1;

        double mid;
        int steps = 0;

        while ((high - low) > precision) {
            mid = (low + high) / 2;
            if (mid * mid < number)
                low = mid;
            else
                high = mid;
            steps++;
        }

        cout << "  [Binary Search Steps: " << steps << "]" << endl;
        return (low + high) / 2;
    }

    // Babylonian (Heron's) Method for Square Root
    double sqrtBabylonian(double number, double precision = 1e-6) {
        if (number < 0) return -1;
        if (number == 0 || number == 1) return number;

        double guess = number / 2.0;
        double nextGuess = (guess + number / guess) / 2;

        int steps = 1;

        while (abs(nextGuess - guess) > precision) {
            guess = nextGuess;
            nextGuess = (guess + number / guess) / 2;
            steps++;
        }

        cout << "  [Babylonian Steps: " << steps << "]" << endl;
        return nextGuess;
    }
};

int main() {
    SquareRoot sr;

    double number = 15;
    double precision = 1e-6;

    cout << "Square Root of " << number << " using Binary Search: ";
    cout << sr.sqrtBinarySearch(number, precision) << endl;

    cout << "Square Root of " << number << " using Babylonian Method: ";
    cout << sr.sqrtBabylonian(number, precision) << endl;

    return 0;
}
