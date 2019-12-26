#include "intcode.hh"

#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
    try {
        auto program = Program::read("input-05.txt");

        program.run({1});

        cout << as_csv(program.output) << endl;
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
