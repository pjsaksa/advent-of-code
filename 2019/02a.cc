#include "intcode.hh"

#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
    try {
        vector<int> codes;

        readCodes(codes,
                  "input-02.txt");

        codes.at(1) = 12;
        codes.at(2) = 2;

        runCodes(codes);

        cout << "Value at position 0 is " << codes.at(0) << endl;
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
