#include "intcode.hh"

#include <algorithm>
#include <iostream>
#include <sstream>
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

        enum { NumOfPrograms = 5 };
        vector<int> phases = {5,6,7,8,9};

        do {
            vector<Program> programs(NumOfPrograms,
                                     original);

            for (int i = 0;
                 i < NumOfPrograms;
                 ++i)
            {
                programs[i].input.push_back(phases[i]);
            }

            //

            int signal = 0;
            int haltCount = 0;

            for (int i = 0;
                 haltCount < NumOfPrograms;
                 i = (i+1) % NumOfPrograms)
            {
                programs[i].input.push_back(signal);

                auto state = programs[i].run();

                if (state == Program::State::Halted)
                {
                    if (haltCount != i) {
                        throw runtime_error(({
                                    ostringstream oss;
                                    oss << "programs are halting in an incorrect order: haltCount=" << haltCount << " i=" << i;
                                    oss.str();
                                }));
                    }

                    ++haltCount;
                }

                if (programs[i].output.empty()) {
                    throw runtime_error(({
                                ostringstream oss;
                                oss << "program did not leave input: haltCount=" << haltCount << " i=" << i;
                                oss.str();
                            }));
                }

                signal = programs[i].output.front();
                programs[i].output.pop_front();

                if (!programs[i].output.empty()) {
                    throw runtime_error(({
                                ostringstream oss;
                                oss << "program produced more than 1 output: haltCount=" << haltCount << " i=" << i;
                                oss.str();
                            }));
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
