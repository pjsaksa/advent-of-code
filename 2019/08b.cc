#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

enum {
    ImageSizeX = 25,
    ImageSizeY =  6,
    ImageSize  = ImageSizeX * ImageSizeY,
};

vector<char> readImage(const string& fileName)
{
    vector<char> image(ImageSize, '?');

    ifstream input(fileName);
    vector<char> layer(ImageSize);

    while (input.read(layer.data(), ImageSize))
    {
        for (int idx = 0;
             idx < ImageSize;
             ++idx)
        {
            if (image[idx] == '?')
            {
                switch (layer[idx]) {
                case '0': image[idx] = ' '; break;
                case '1': image[idx] = '#'; break;
                }
            }
        }
    }

    return image;
}

void printImage(const vector<char>& image)
{
    for (int idx = 0;
         idx < ImageSize;
         ++idx)
    {
        cout << image[idx];

        if ((idx + 1) % ImageSizeX == 0)
        {
            cout << endl;
        }
    }
}

int main()
{
    printImage(readImage("input-08.txt"));

    return 0;
}
