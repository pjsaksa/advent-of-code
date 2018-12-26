#include <cctype>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

//

using unit_t    = char;
using polymer_t = vector<unit_t>;

struct UnitType {
    unit_t type;
    bool polarity;

    UnitType(unit_t input)
        : type(tolower(input)),
          polarity(isupper(input)) {}
};

int main()
{
    std::ifstream input("input-05.txt");
    polymer_t polymer;
    unit_t unit;

    while (input >> unit)
    {
        const UnitType unitT(unit);

        if (!polymer.empty())
        {
            const unit_t last = polymer.back();
            const UnitType lastT(last);

            if (unitT.type == lastT.type
                && unitT.polarity != lastT.polarity)
            {
                polymer.pop_back();
                continue;
            }
        }

        polymer.push_back(unit);
    }

    cout << "remaining polymer size: " << polymer.size() << endl;
}
