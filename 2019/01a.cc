#include <fstream>
#include <iostream>
using namespace std;

int main()
{
    ifstream input("input-01.txt");
    int mass;
    int totalFuel = 0;

    while (input >> mass)
    {
        const int fuel = (mass / 3) - 2;

        totalFuel += fuel;
    }

    cout << "Total fuel: " << totalFuel << endl;
}
