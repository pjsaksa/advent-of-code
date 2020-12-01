#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

enum {
    MapSizeX = 10,
    MapSizeY = 10,
    MapSize = MapSizeX * MapSizeY,
};

vector<bool> readMap(const string& fileName)
{
    vector<bool> map(MapSize);

    ifstream ifs(fileName);
    string line;

    int bytesRead = 0;
    int lineCount = 0;

    while (getline(ifs, line))
    {
        if (line.size() != MapSizeX) {
            throw runtime_error("invalid line length, is " + to_string(line.size()) + ", should be " + to_string(MapSizeX));
        }

        for (char ch : line) {
            map[bytesRead++] = (ch == '#');
        }
        ++lineCount;
    }

    if (lineCount != MapSizeY) {
        throw runtime_error("invalid line count, is " + to_string(lineCount) + ", should be " + to_string(MapSizeY));
    }

    return map;
}

int main(int argc,
         char* argv[])
{
    const vector<string> args(argv + 1, argv + argc);

    string fileName = "input-10.txt";

    if (args.size() > 0) {
        fileName = args[0];
    }

    // -----

    try {
        struct {
            unsigned int visibleCount = 0;
            int x = -1;
            int y = -1;
        } highest;

        auto map = readMap(fileName);

        for (int origoY = 0;
             origoY < MapSizeY;
             ++origoY)
        {
            for (int origoX = 0;
                 origoX < MapSizeX;
                 ++origoX)
            {
                if (!map[origoY*MapSizeX + origoX])
                    continue;

                set<string> angles;

                cout << "--- (" << origoX << ", " << origoY << ")" << endl;

                for (int tgtY = 0;
                     tgtY < MapSizeY;
                     ++tgtY)
                {
                    for (int tgtX = 0;
                         tgtX < MapSizeX;
                         ++tgtX)
                    {
                        if (!map[tgtY*MapSizeX + tgtX])
                            continue;

                        const int deltaX = tgtX - origoX;
                        const int deltaY = tgtY - origoY;

                        if (deltaX == 0
                            && deltaY == 0)
                        {
                            continue;
                        }

                        const double a = ((deltaX != 0)  ? -atan(static_cast<double>(deltaY) / static_cast<double>(deltaX))
                                          : (deltaY > 0) ? M_PI / 2.0
                                          : /* (deltaY < 0) ? */ -M_PI / 2.0);

                        ostringstream oss;
                        oss << fixed << setprecision(6) << a;

                        angles.insert(oss.str());

                        cout << "angle: " << oss.str() << endl;
                    }
                }

                cout << "angle count: " << angles.size() << endl;

                if (angles.size() > highest.visibleCount)
                {
                    highest.visibleCount = angles.size();
                    highest.x = origoX;
                    highest.y = origoY;
                }
            }
        }

        cout << "Highest visibility is at (" << highest.x << ", " << highest.y << "), and it is " << highest.visibleCount << endl;
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
