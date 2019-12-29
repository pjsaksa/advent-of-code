#include "intcode.hh"

#include <iostream>
using namespace std;

int main()
{
    try {
        auto program = Program::read("input-09.txt");

        program.run({2});

        cout << as_csv(program.output) << endl;
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
