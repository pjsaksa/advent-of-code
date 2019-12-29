#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

enum {
    ImageSizeX = 25,
    ImageSizeY =  6,
    ImageSize  = ImageSizeX * ImageSizeY,
};

int main()
{
    struct {
        unsigned int amountOfZeroes  = ImageSize + 1;
        unsigned int onesAndTwosMult = 0;
    } result;

    //

    ifstream input("input-08.txt");

    vector<char> pixels(ImageSize);

    while (input.read(pixels.data(), ImageSize))
    {
        vector<unsigned int> amounts(3, 0);

        for (char c : pixels) {
            switch (c) {
            case '0': ++amounts[0]; break;
            case '1': ++amounts[1]; break;
            case '2': ++amounts[2]; break;
            }
        }

        if (amounts[0] < result.amountOfZeroes)
        {
            result.amountOfZeroes = amounts[0];
            result.onesAndTwosMult = amounts[1] * amounts[2];
        }
    }

    cout << "(1*2) is " << result.onesAndTwosMult << " when 0-count was " << result.amountOfZeroes << endl;

    return 0;
}
