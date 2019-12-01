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
        while ((mass = mass / 3 - 2) > 0)
        {
            totalFuel += mass;
        }
    }

    cout << "Total fuel: " << totalFuel << endl;
}
