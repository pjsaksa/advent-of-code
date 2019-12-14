#include "intcode.hh"

#include <iostream>
#include <stdexcept>
using namespace std;

enum { Result = 19690720 };

int main()
{
    try {
        vector<int> saved;

        readCodes(saved,
                  "input-02.txt");

        for (int noun = 0;
             noun < 100;
             ++noun)
        {
            for (int verb = 0;
                 verb < 100;
                 ++verb)
            {
                vector<int> codes = saved;

                codes.at(1) = noun;
                codes.at(2) = verb;

                runCodes(codes);

                if (codes.at(0) == Result)
                {
                    cout << "With noun " << noun << " and verb " << verb << " the value at position 0 is " << Result << endl
                         << "Answer: " << (100 * noun + verb) << endl;
                }
            }
        }
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
    }
}
