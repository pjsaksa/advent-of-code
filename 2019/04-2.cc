#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

enum {
    RangeStart = 372304,    // inclusive
    RangeEnd   = 847060,
};

int to_value(const vector<int>& numbers)
{
    int mult = 1;
    int value = 0;
    int n = numbers.size();

    while (--n >= 0) {
        value += numbers[n] * mult;
        mult *= 10;
    }

    return value;
}

int main()
{
    int part1Count = 0;
    int part2Count = 0;
    uint32_t doubleBits = 0;

    vector<int> numbers({3,7,2,3,0,4});

    auto iter = numbers.begin();
    const auto start = iter;
    const auto end   = numbers.end();

    for (;;)
    {
        if (iter == start) {
            if (*iter >= 10) {
                throw runtime_error("we should be done by now");
            }
        }
        else {
            if (*iter >= 10)
            {
                *iter = 0;
                --iter;
                ++ *iter;
                continue;
            }
            else if (*iter < *(iter - 1)) {
                *iter = *(iter - 1);
            }

            if (*iter == *(iter - 1)) {
                doubleBits |= (1 << (iter - start - 1));
            }
            else {
                doubleBits &= ~(1 << (iter - start - 1));
            }
        }

        if (!(iter + 1 == end)) {
            ++iter;
            continue;
        }

        //

        if (to_value(numbers) > RangeEnd) {
            break;
        }

        //

        ++ *iter;

        if (doubleBits == 0) {
            continue;
        }

        ++part1Count;

        //

        if ((doubleBits & 0b0011u) == 0b0001u) {
            ++part2Count;
            continue;
        }

        for (size_t bit = 1;
             bit < numbers.size();
             ++bit)
        {
            const uint32_t shift = bit - 1;

            if ((doubleBits & (0b0111u << shift)) == (0b0010u << shift)) {
                ++part2Count;
                break;
            }
        }
    }

    // ------

    cout << "Different password count, first part: " << part1Count << endl
         << "                         second part: " << part2Count << endl;

    return 0;
}
