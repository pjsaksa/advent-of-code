#include "02-common.hh"

#include <fstream>
#include <stdexcept>
using namespace std;

void readCodes(vector<int>& codes)
{
    ifstream input("input-02.txt");
    int code;
    char comma;

    while (input >> code)
    {
        codes.push_back(code);

        if (!(input >> comma
              && comma == ','))
        {
            break;
        }
    }
}

void runCodes(vector<int>& codes)
{
    int ip = 0;
    bool running = true;

    while (running)
    {
        switch (codes.at(ip)) {
        case 1:
            {
                const int srcIdx1 = codes.at(ip + 1);
                const int srcIdx2 = codes.at(ip + 2);
                const int dstIdx  = codes.at(ip + 3);

                codes.at(dstIdx) = codes.at(srcIdx1) + codes.at(srcIdx2);

                ip += 4;
            }
            break;

        case 2:
            {
                const int srcIdx1 = codes.at(ip + 1);
                const int srcIdx2 = codes.at(ip + 2);
                const int dstIdx  = codes.at(ip + 3);

                codes.at(dstIdx) = codes.at(srcIdx1) * codes.at(srcIdx2);

                ip += 4;
            }
            break;

        case 99:
            running = false;
            break;

        default:
            throw runtime_error("invalid code=" + to_string(codes.at(ip)) + " at index=" + to_string(ip));
        }
    }
}
