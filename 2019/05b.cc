#include "intcode.hh"

#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
    try {
        vector<int> codes;

        readCodes(codes,
                  "input-05.txt");

        runCodes(codes,
                 {5});
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
