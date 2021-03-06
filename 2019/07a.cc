#include "intcode.hh"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(int argc,
         char* argv[])
{
    const vector<string> args(argv + 1, argv + argc);

    string fileName = "input-07.txt";

    if (args.size() > 0) {
        fileName = args[0];
    }

    // -----

    struct {
        int signal = 0;
        vector<int> phases;
    } highest;

    try {
        const auto original = Program::read(fileName);

        vector<int> phases = {0,1,2,3,4};

        do {
            int signal = 0;

            for (int phase : phases)
            {
                auto program = original;

                program.run({phase, signal});

                if (program.output.size() == 1) {
                    signal = program.output[0];
                }
                else {
                    runtime_error("the amount of outputs is not 1");
                }
            }

            if (signal > highest.signal)
            {
                highest.signal = signal;
                highest.phases = phases;
            }
        }
        while (next_permutation(phases.begin(),
                                phases.end()));

        cout << "phases " << as_csv(highest.phases) << " produced highest signal " << highest.signal << endl;
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
