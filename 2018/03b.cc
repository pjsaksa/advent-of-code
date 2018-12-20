#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

//

struct Area {
    int id;
    int x1, y1;
    int x2, y2;

    Area() = default;
    Area(int pid, int px1, int py1, int px2, int py2)
     : id(pid), x1(px1), y1(py1), x2(px2), y2(py2) {}

    operator bool () const
    {
        return x1 < x2 && y1 < y2;
    }

    int size() const
    {
        return (x2 - x1) * (y2 - y1);
    }
};

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
            area = Area(id, left, top, left + width, top + height);
        }
        else {
            in.setstate(ios_base::failbit);
        }
    }

    return in;
}

bool doIntersect(const Area& a, const Area& b)
{
    const Area o(0,
                 max(a.x1, b.x1), max(a.y1, b.y1),
                 min(a.x2, b.x2), min(a.y2, b.y2));

    return bool{ o };
}

//

using areas_t = vector<Area>;

//

areas_t readInput(const string& name)
{
    areas_t areas;

    ifstream input(name);
    string line;

    while (getline(input, line))
    {
        std::istringstream issLine(line);
        Area area;

        if (issLine >> area)
            areas.push_back(area);
    }

    return areas;
}

int main()
{
    const areas_t areas = readInput("input-03.txt");

    for (auto leftIter = areas.begin();
         leftIter != areas.end();
         ++leftIter)
    {
        bool overlaps = false;

        for (auto rightIter = areas.begin();
             rightIter != areas.end();
             ++rightIter)
        {
            if (leftIter == rightIter)
                continue;

            if (doIntersect(*leftIter,
                            *rightIter))
            {
                overlaps = true;
                break;
            }
        }

        if (!overlaps) {
            cout << "not overlapping: " << leftIter->id << endl;
        }
    }
}
