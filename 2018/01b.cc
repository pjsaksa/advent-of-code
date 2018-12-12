#include <fstream>
#include <iostream>
#include <set>
using namespace std;

int main()
{
    ifstream input("input-01.txt");

    auto freq = 0;
    auto change = 0;
    set<int> freqStore;

    while (input) {
        while (input >> change)
        {
            freq += change;

            if (freqStore.count(freq) > 0) {
                cout << "freq: " << freq << endl;
                return 0;
            }

            freqStore.emplace(freq);
        }

        // rewind

        input.clear();
        input.seekg(0);
    }

    cerr << "input error" << endl;
}
