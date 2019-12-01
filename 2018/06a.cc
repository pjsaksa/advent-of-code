#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
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

using identity_t = signed char;

enum { NoIdentity = -1 };

//

class Grid {
public:
    Grid(const Point& o, const Point& s)
        : m_origo(o),
          m_size(s),
          m_area(m_size.x * m_size.y, NoIdentity) {}

    identity_t  operator[] (const Point& point) const { return m_area[pointToIndex(point)]; }
    identity_t& operator[] (const Point& point)       { return m_area[pointToIndex(point)]; }

    const Point& origo() const { return m_origo; }
    const Point& size() const  { return m_size; }

private:
    Point m_origo;
    Point m_size;

    vector<identity_t> m_area;

    //

    int pointToIndex(const Point& point) const
    {
        if (point.x < m_origo.x) throw runtime_error("invalid point");
        if (point.y < m_origo.y) throw runtime_error("invalid point");

        const Point shifted = point - m_origo;

        if (shifted.x >= m_size.x) throw runtime_error("invalid point");
        if (shifted.y >= m_size.y) throw runtime_error("invalid point");

        return shifted.x + shifted.y * m_size.x;
    }
};

// ------------------------------------------------------------

struct IdentityPoint {
    identity_t identity = 0;
    Point point;

    IdentityPoint() = default;
    IdentityPoint(identity_t pi, const Point& po)
        : identity(pi), point(po) {}
};

using points_t = vector<IdentityPoint>;

//

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
            points.emplace_back(points.size(), point);
    }

    return points;
}

//

pair<Point, Point> findCorners(const points_t& points)
{
    Point minP(numeric_limits<int>::max(), numeric_limits<int>::max());
    Point maxP(numeric_limits<int>::min(), numeric_limits<int>::min());

    for (auto& idPoint : points)
    {
        if (idPoint.point.x < minP.x) minP.x = idPoint.point.x;
        if (idPoint.point.y < minP.y) minP.y = idPoint.point.y;
        if (idPoint.point.x > maxP.x) maxP.x = idPoint.point.x;
        if (idPoint.point.y > maxP.y) maxP.y = idPoint.point.y;
    }

    return {minP, maxP};
}

int manhattanDistance(const Point& a, const Point& b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

Grid plotGrid(const points_t& referencePoints)
{
    if (referencePoints.size() < 2)
        throw runtime_error("too few points");

    const pair<Point, Point> corners = findCorners(referencePoints);
    const Point gridOrigo(corners.first.x - 1,
                          corners.first.y - 1);
    const Point gridSize(corners.second.x + 2 - gridOrigo.x,
                         corners.second.y + 2 - gridOrigo.y);
    Grid grid(gridOrigo,
              gridSize);

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

            points_t localPoints = referencePoints;

            sort(localPoints.begin(),
                 localPoints.end(),
                 [&point = point](auto& left, auto& right)
                 {
                     return manhattanDistance(point, left.point) < manhattanDistance(point, right.point);
                 });

            const int lowestDist    = manhattanDistance(point, localPoints[0].point);
            const int lowestDist2nd = manhattanDistance(point, localPoints[1].point);

            if (lowestDist < lowestDist2nd)
            {
                grid[point] = localPoints[0].identity;
            }
        }
    }

    return grid;
}

// ------------------------------------------------------------

using areaSizes_t = vector<int>;

int& referAreaSize(areaSizes_t& sizes, identity_t id)
{
    static int s_dummy = 0;

    if (id < NoIdentity
        || id >= static_cast<identity_t>(sizes.size()))
    {
        throw runtime_error("invalid identity in grid");
    }

    if (id == NoIdentity)
        return s_dummy;

    return sizes[id];
}

areaSizes_t countAreas(const Grid& grid, int identityCount)
{
    areaSizes_t sizes(identityCount, 0);

    const Point& gridOrigo = grid.origo();
    const Point& gridSize = grid.size();

    // count each pixel

    for (int y = 0;
         y < gridSize.y;
         ++y)
    {
        for (int x = 0;
             x < gridSize.x;
             ++x)
        {
            ++referAreaSize(sizes, grid[Point(gridOrigo.x + x,
                                              gridOrigo.y + y)]);
        }
    }

    // invalidate infinite areas

    for (int x = 0;
         x < gridSize.x;
         ++x)
    {
        referAreaSize(sizes, grid[Point(gridOrigo.x + x, gridOrigo.y + 0)])              = NoIdentity;
        referAreaSize(sizes, grid[Point(gridOrigo.x + x, gridOrigo.y + gridSize.y - 1)]) = NoIdentity;
    }

    for (int y = 0;
         y < gridSize.y;
         ++y)
    {
        referAreaSize(sizes, grid[Point(gridOrigo.x + 0, gridOrigo.y + y)])              = NoIdentity;
        referAreaSize(sizes, grid[Point(gridOrigo.x + gridSize.x - 1, gridOrigo.y + y)]) = NoIdentity;
    }

    return sizes;
}

int main()
{
    try {
        const points_t points = readInput("input-06.txt");
        const Grid grid = plotGrid(points);
        const areaSizes_t sizes = countAreas(grid, points.size());

        auto maxIter = max_element(sizes.begin(),
                                   sizes.end());

        if (maxIter != sizes.end())
        {
            cout << "largest finite area is " << static_cast<int>(*maxIter) << " squares" << endl;
        }
    }
    catch (exception& e) {
        cout << "exception: " << e.what() << endl;
    }
}
