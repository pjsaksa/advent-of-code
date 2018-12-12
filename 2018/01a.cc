#include <fstream>
#include <iostream>
using namespace std;

int main()
{
    ifstream input("input-01.txt");

    if (input) {
        auto freq = 0;
        auto change = 0;

        while (input >> change)
            freq += change;

        cout << "freq: " << freq << endl;
    }
    else {
        cerr << "input error" << endl;
    }
}
