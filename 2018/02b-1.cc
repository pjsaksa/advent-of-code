#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    vector<string> ids;

    // first read all ids

    {
        ifstream input("input-02.txt");
        string id;

        while (input >> id)
        {
            ids.push_back(id);
        }
    }

    // iterate through all pairs

    for (auto leftIter = ids.cbegin();
         leftIter != ids.cend();
         ++leftIter)
    {
        for (auto rightIter = leftIter + 1;
             rightIter != ids.cend();
             ++rightIter)
        {
            auto& left  = *leftIter;
            auto& right = *rightIter;

            if (left.size() != right.size())
                continue;

            string match;

            for (string::size_type i = 0U;
                 i < left.size();
                 ++i)
            {
                if (left[i] == right[i])
                    match += left[i];
            }

            if (left.size() - match.size() == 1)
            {
                cout << "common letters: " << match << endl;
            }
        }
    }
}
