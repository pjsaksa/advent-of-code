#include <fstream>
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main()
{
    ifstream input("input-02.txt");

    auto pairs = 0;
    auto trios = 0;

    string id;

    while (input >> id)
    {
        map<char, int> letters;

        for (auto letter : id) {
            ++letters[letter];
        }

        map<int, int> letterCounts;

        for (auto lc : letters) {
            ++letterCounts[lc.second];
        }

        if (letterCounts[2] > 0) ++pairs;
        if (letterCounts[3] > 0) ++trios;
    }

    cout << "checksum: " << (pairs*trios) << endl;
}
