#include <algorithm>
#include <stdexcept>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

//

struct Area {
    int x1, y1;
    int x2, y2;

    Area() = default;
    Area(int px1, int py1, int px2, int py2)
     : x1(px1), y1(py1), x2(px2), y2(py2) {}
};

using areas_t = vector<Area>;

//

istream& operator>> (istream& in, Area& area)
{
    // e.g. "#15 @ 348,421: 16x11"

    int id;         // cached values
    int left;
    int top;
    int width;
    int height;

    char hash;      // separators
    char at;
    char comma;
    char colon;
    char x;

    if (in >> hash >> id >> at >> left >> comma >> top >> colon >> width >> x >> height)
    {
        if (hash == '#'
            && at == '@'
            && comma == ','
            && colon == ':'
            && x == 'x')
        {
            area = Area(left, top, left + width, top + height);
        }
        else {
            in.setstate(ios_base::failbit);
        }
    }

    return in;
}

class Bitmap {
public:
    enum {
        SizeX     =  999,
        SizeY     = 1000,
        TotalSize = SizeX * SizeY,
    };

    //

    Bitmap()
    {
        m_array.fill(0U);
    }

    void set(int x, int y)
    {
        if (x < 0 || x > SizeX)
            throw runtime_error("Bitmap::set(): invalid x");
        if (y < 0 || y > SizeY)
            throw runtime_error("Bitmap::set(): invalid y");

        //

        const auto offset = SizeX * y + x;

        if (offset < 0 || offset >= TotalSize)
            throw runtime_error("Bitmap::set(): invalid offset");

        if (m_array[offset] < 0xFFU)
            ++m_array[offset];
    }

    int size() const
    {
        return count_if(m_array.begin(),
                        m_array.end(),
                        [](auto v)
                        {
                            return v >= 2;
                        });
    }

private:
    std::array<uint8_t, TotalSize> m_array;
};

areas_t readInput(const string& name)
{
    areas_t areas;

    ifstream input(name);
    string line;

    auto maxX = 0;
    auto maxY = 0;

    while (getline(input, line))
    {
        std::istringstream issLine(line);
        Area area;

        if (issLine >> area) {
            areas.push_back(area);

            if (maxX < area.x2) maxX = area.x2;
            if (maxY < area.y2) maxY = area.y2;
        }
    }

    //

    if (maxX != Bitmap::SizeX
        || maxY != Bitmap::SizeY)
    {
        throw runtime_error("input sizes do not match with bitmap size, recalibrate the code\n");
    }

    return areas;
}

int main()
{
    try {
        const areas_t areas = readInput("input-03.txt");

        //

        Bitmap bitmap;

        for (const auto& area : areas)
        {
            for (auto x = area.x1;
                 x < area.x2;
                 ++x)
            {
                for (auto y = area.y1;
                     y < area.y2;
                     ++y)
                {
                    bitmap.set(x, y);
                }
            }
        }

        cout << "total area: " << bitmap.size() << endl;
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
    }
}
