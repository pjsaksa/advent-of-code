#include "intcode.hh"

#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
    try {
        auto program = Program::read("input-02.txt");

        program[1] = 12;
        program[2] = 2;

        program.run();

        cout << "Value at position 0 is " << program[0] << endl;
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
