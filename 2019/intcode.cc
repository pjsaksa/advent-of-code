#include "intcode.hh"

#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

vector<int> readCodes(const string& fileName)
{
    ifstream input(fileName);
    int code;
    char comma;

    vector<int> codes;

    while (input >> code)
    {
        codes.push_back(code);

        if (!(input >> comma
              && comma == ','))
        {
            break;
        }
    }

    return codes;
}

namespace
{
    int& access(vector<int>& codes,
                const int ip,
                const int accessMode)
    {
        switch (accessMode) {
        case 0: return codes.at(codes.at(ip));
        case 1: return codes.at(ip);
        default: throw runtime_error("invalid access mode at " + to_string(ip));
        }
    }
}

vector<int> runCodes(vector<int>& codes,
                     const vector<int>& inputs)
{
    int ip = 0;
    bool running = true;

    auto input = inputs.begin();
    vector<int> output;

    while (running)
    {
        switch (codes.at(ip) % 100) {
        case 1:
            {
                const int& src1 = access(codes, ip + 1, codes.at(ip) / 100 % 10);
                const int& src2 = access(codes, ip + 2, codes.at(ip) / 1000 % 10);
                int&       dst  = access(codes, ip + 3, codes.at(ip) / 10000 % 10);

                dst = src1 + src2;

                ip += 4;
            }
            break;

        case 2:
            {
                const int& src1 = access(codes, ip + 1, codes.at(ip) / 100 % 10);
                const int& src2 = access(codes, ip + 2, codes.at(ip) / 1000 % 10);
                int&       dst  = access(codes, ip + 3, codes.at(ip) / 10000 % 10);

                dst = src1 * src2;

                ip += 4;
            }
            break;

        case 3:
            {
                if (input == inputs.end()) {
                    throw runtime_error("missing input");
                }

                int& dst = access(codes, ip + 1, codes.at(ip) / 100 % 10);

                dst = *input++;

                ip += 2;
            }
            break;

        case 4:
            {
                int& src = access(codes, ip + 1, codes.at(ip) / 100 % 10);

                output.push_back(src);

                ip += 2;
            }
            break;

        case 5:
            {
                int& cond = access(codes, ip + 1, codes.at(ip) / 100 % 10);
                int& tgt  = access(codes, ip + 2, codes.at(ip) / 1000 % 10);

                if (cond != 0) {
                    ip = tgt;
                }
                else {
                    ip += 3;
                }
            }
            break;

        case 6:
            {
                int& cond = access(codes, ip + 1, codes.at(ip) / 100 % 10);
                int& tgt  = access(codes, ip + 2, codes.at(ip) / 1000 % 10);

                if (cond == 0) {
                    ip = tgt;
                }
                else {
                    ip += 3;
                }
            }
            break;

        case 7:
            {
                int& cond1 = access(codes, ip + 1, codes.at(ip) / 100 % 10);
                int& cond2 = access(codes, ip + 2, codes.at(ip) / 1000 % 10);
                int& dst   = access(codes, ip + 3, codes.at(ip) / 10000 % 10);

                dst = (cond1 < cond2);

                ip += 4;
            }
            break;

        case 8:
            {
                int& cond1 = access(codes, ip + 1, codes.at(ip) / 100 % 10);
                int& cond2 = access(codes, ip + 2, codes.at(ip) / 1000 % 10);
                int& dst   = access(codes, ip + 3, codes.at(ip) / 10000 % 10);

                dst = (cond1 == cond2);

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

    return output;
}

ostream& operator<< (ostream& out,
                     const vector<int>& codes)
{
    out << '{';

    if (auto i = codes.begin();
        i != codes.end())
    {
        out << *i++;

        for (;
             i != codes.end();
             ++i)
        {
            cout << ',' << *i;
        }
    }

    out << '}';

    return out;
}
