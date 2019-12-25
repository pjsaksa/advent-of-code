#include "intcode.hh"

#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
    try {
        auto codes  = readCodes("input-05.txt");
        auto output = runCodes(codes,
                               {1});

        cout << output << endl;
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
