#include "intcode.hh"

#include <iostream>
#include <stdexcept>
using namespace std;

enum { Result = 19690720 };

int main()
{
    try {
        const auto original = Program::read("input-02.txt");

        for (int noun = 0;
             noun < 100;
             ++noun)
        {
            for (int verb = 0;
                 verb < 100;
                 ++verb)
            {
                auto program = original;

                program[1] = noun;
                program[2] = verb;

                program.run();

                if (program[0] == Result)
                {
                    cout << "With noun " << noun << " and verb " << verb << " the value at position 0 is " << Result << endl
                         << "Answer: " << (100 * noun + verb) << endl;
                }
            }
        }
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
