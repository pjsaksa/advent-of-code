#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

//

using unit_t    = char;
using polymer_t = vector<unit_t>;
using types_t   = set<unit_t>;

struct UnitType {
    unit_t type;
    bool polarity;

    UnitType(unit_t input)
        : type(tolower(input)),
          polarity(isupper(input)) {}
};

void addToPolymer(polymer_t& polymer, unit_t unit)
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
            return;
        }
    }

    polymer.push_back(unit);
}

polymer_t cleanPolymer(const polymer_t& polymer, unit_t removed)
{
    polymer_t result;
    UnitType removedT(removed);

    for (auto unit : polymer)
    {
        if (UnitType(unit).type == removedT.type)
            continue;

        addToPolymer(result, unit);
    }

    return result;
}

polymer_t readInput(const string& name)
{
    std::ifstream input(name);
    polymer_t polymer;
    unit_t unit;

    while (input >> unit) {
        addToPolymer(polymer, unit);
    }

    return polymer;
}

types_t allTypes(const polymer_t& polymer)
{
    types_t types;

    for (auto unit : polymer)
    {
        types.insert(UnitType(unit).type);
    }

    return types;
}

int main()
{
    const polymer_t originalPolymer = readInput("input-05.txt");
    const types_t uniqueTypes = allTypes(originalPolymer);

    map<unit_t, polymer_t> cleanedPolymers;

    for (auto removed : uniqueTypes)
    {
        cleanedPolymers[removed] = cleanPolymer(originalPolymer, removed);
    }

    const auto minIter = min_element(cleanedPolymers.begin(),
                                     cleanedPolymers.end(),
                                     [](const auto& left, const auto& right)
                                     {
                                         return left.second.size() < right.second.size();
                                     });

    cout << "minimum polymer size: " << minIter->second.size() << " ('" << minIter->first <<  "' removed)" <<  endl;
}
