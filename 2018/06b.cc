#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

struct Point {
    int x = 0;
    int y = 0;

    Point() = default;
    Point(int px, int py)
        : x(px), y(py) {}
};

Point operator- (const Point& a, const Point& b)
{
    return Point(a.x - b.x,
                 a.y - b.y);
}

istream& operator>> (istream& in, Point& point)
{
    int x;
    int y;

    char comma;

    if (in >> x >> comma >> y)
    {
        if (comma == ',')
        {
            point.x = x;
            point.y = y;
        }
        else in.setstate(std::ios_base::failbit);
    }

    return in;
}

// ------------------------------------------------------------

using points_t = vector<Point>;
using volume_t = uint16_t;

enum {
    MaxVolume   = numeric_limits<volume_t>::max(),
    VolumeLimit = 10000,
};

// ------------------------------------------------------------

points_t readInput(const std::string& name)
{
    points_t points;

    std::ifstream input(name);
    std::string line;

    while (getline(input, line))
    {
        std::istringstream issLine(line);
        Point point;

        if (issLine >> point)
            points.push_back(point);
    }

    return points;
}

//

pair<Point, Point> findCorners(const points_t& points)
{
    Point minP(numeric_limits<int>::max(), numeric_limits<int>::max());
    Point maxP(numeric_limits<int>::min(), numeric_limits<int>::min());

    for (auto& point : points)
    {
        if (point.x < minP.x) minP.x = point.x;
        if (point.y < minP.y) minP.y = point.y;
        if (point.x > maxP.x) maxP.x = point.x;
        if (point.y > maxP.y) maxP.y = point.y;
    }

    return {minP, maxP};
}

int manhattanDistance(const Point& a, const Point& b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int accumulateDistances(const points_t& referencePoints)
{
    if (referencePoints.size() < 2)
        throw runtime_error("too few points");

    const pair<Point, Point> corners = findCorners(referencePoints);
    const Point gridOrigo(corners.first.x - 1,
                          corners.first.y - 1);
    const Point gridSize(corners.second.x + 2 - gridOrigo.x,
                         corners.second.y + 2 - gridOrigo.y);

    int regionSize = 0;

    for (int y = 0;
         y < gridSize.y;
         ++y)
    {
        for (int x = 0;
             x < gridSize.x;
             ++x)
        {
            const Point point(gridOrigo.x + x,
                              gridOrigo.y + y);

            volume_t volume = 0;

            for (auto& refPoint : referencePoints)
            {
                const volume_t dist = manhattanDistance(point, refPoint);

                // check for overflow

                if (dist > MaxVolume - volume)
                {
                    volume = MaxVolume;
                }
                else {
                    volume += dist;
                }
            }

            if (volume < VolumeLimit) {
                ++regionSize;
            }
        }
    }

    return regionSize;
}

int main()
{
    try {
        const points_t points = readInput("input-06.txt");
        const int regionSize = accumulateDistances(points);

        cout << "region size is " << regionSize << " squares" << endl;
    }
    catch (exception& e) {
        cout << "exception: " << e.what() << endl;
    }
}
