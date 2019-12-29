#include "intcode.hh"

#include <iostream>
using namespace std;

int main(int argc,
         char* argv[])
{
    const vector<string> args(argv + 1, argv + argc);

    string fileName = "input-09.txt";

    if (args.size() > 0) {
        fileName = args[0];
    }

    // -----

    try {
        auto program = Program::read(fileName);

        program.run({1});

        cout << as_csv(program.output) << endl;
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
