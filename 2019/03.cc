#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>
using namespace std;

class Point : public pair<int, int> {
public:
    using pair<int, int>::pair;

    int x() const { return first; }
    int y() const { return second; }
};

//

ostream& operator<< (ostream& out, const Point& p)
{
    return out << '(' << p.x() << ", " << p.y() << ')';
}

Point operator+ (const Point& a, const Point& b)
{
    return Point(a.x() + b.x(),
                 a.y() + b.y());
}

// -----

class Intersection {
public:
    Intersection(const Point &p, int s)
        : m_point(p),
          m_steps(s) {}

    const Point& point() const { return m_point; }
    int steps() const { return m_steps; }

private:
    Point m_point;
    int m_steps;
};

//

using intersections_t = vector<Intersection>;

// -----

int manhattanDistance(const Point& p)
{
    return abs(p.x()) + abs(p.y());
}

bool orderBy_manhattanDistance(const Intersection& a, const Intersection& b)
{
    return manhattanDistance(a.point()) < manhattanDistance(b.point());
}

bool orderBy_steps(const Intersection& a, const Intersection& b)
{
    return a.steps() < b.steps();
}

// ------------------------------------------------------------

class Segment {
public:
    Segment(Point pp1, Point pp2, int s)
        : m_point1(pp1),
          m_point2(pp2),
          m_steps(s)
    {
        if (m_point1 == m_point2) {
            throw runtime_error("zero length segment");
        }

        if (!horizontal()
            && !vertical())
        {
            throw runtime_error("segment is not horizontal nor vertical");
        }
    }

    const Point& point1() const { return m_point1; }
    const Point& point2() const { return m_point2; }
    int stepsToPoint1() const { return m_steps; }

    bool horizontal() const { return m_point1.y() == m_point2.y(); }
    bool vertical() const { return m_point1.x() == m_point2.x(); }

    int length() const
    {
        if (horizontal()) {
            return abs(m_point2.x() - m_point1.x());
        }
        else /* if (vertical()) */ {
            return abs(m_point2.y() - m_point1.y());
        }
    }

public:
    Point m_point1;
    Point m_point2;
    int m_steps;
};

// -----

bool between(int x1, int mid, int x2)
{
    return (x1 <= mid && mid <= x2)
        || (x2 <= mid && mid <= x1);
}

optional<Point> intersects(const Segment&a, const Segment& b)
{
    if (a.vertical()
        && b.horizontal())
    {
        if (between(b.point1().x(), a.point1().x(), b.point2().x())
            && between(a.point1().y(), b.point1().y(), a.point2().y()))
        {
            return Point(a.point1().x(), b.point1().y());
        }
    }
    else if (a.horizontal()
             && b.vertical())
    {
        if (between(a.point1().x(), b.point1().x(), a.point2().x())
            && between(b.point1().y(), a.point1().y(), b.point2().y()))
        {
            return Point(b.point1().x(), a.point1().y());
        }
    }

    return nullopt;
}

int distance(const Segment& s, const Point& p)
{
    return abs(s.point1().x() - p.x()) + abs(s.point1().y() - p.y());
}

// ------------------------------------------------------------

using wire_t  = vector<Segment>;
using wires_t = vector<wire_t>;

Point readIncrement(const string& incr)
{
    istringstream incrIss(incr);

    char direction;
    int distance;

    if (incrIss >> direction >> distance)
    {
        switch (direction) {
        case 'U': return Point(0,         -distance);
        case 'R': return Point(distance,  0);
        case 'D': return Point(0,         distance);
        case 'L': return Point(-distance, 0);
        }

        if (char excess;
            !(incrIss >> excess))
        {
            throw runtime_error("excess data");
        }
    }

    throw runtime_error("invalid increment [" + incr + "]");
}

wire_t readWire(const string& line)
{
    istringstream lineIss(line);
    string incr;

    wire_t wire;
    Point firstPoint(0, 0);
    int steps = 0;

    while (getline(lineIss, incr, ','))
    {
        const auto& segment = wire.emplace_back(firstPoint,
                                                firstPoint + readIncrement(incr),
                                                steps);

        steps += segment.length();
        firstPoint = segment.point2();
    }

    return wire;
}

wires_t readInput()
{
    ifstream ifs("input-03.txt");
    string line;

    wires_t wires;

    while (getline(ifs, line))
    {
        wires.push_back( readWire(line) );
    }

    return wires;
}

// ------------------------------------------------------------

intersections_t findIntersections(const wire_t& wire1,
                                  const wire_t& wire2)
{
    intersections_t intersections;

    for (auto& segment1 : wire1)
    {
        for (auto& segment2 : wire2)
        {
            if (const auto p = intersects(segment1,
                                          segment2);
                p)
            {
                const int combinedStepsToIntersection = ( segment1.stepsToPoint1() + distance(segment1, *p)
                                                          + segment2.stepsToPoint1() + distance(segment2, *p) );

                intersections.emplace_back(*p,
                                           combinedStepsToIntersection);
            }
        }
    }

    return intersections;
}

// ------------------------------------------------------------

int main()
{
    try
    {
        const auto wires = readInput();

        if (wires.size() != 2) {
            throw runtime_error("Failed to read 2 wires, got " + to_string(wires.size()));
        }

        auto intersections = findIntersections(wires[0],
                                               wires[1]);

        cout << intersections.size() << " intersections" << endl;

        if (intersections.empty()) {
            throw runtime_error("empty list of intersections");
        }

        //

        partial_sort(intersections.begin(),
                     intersections.begin() + 1,
                     intersections.end(),
                     orderBy_manhattanDistance);

        cout << "The closest is at coordinates " << intersections[0].point()
             << " = " << manhattanDistance(intersections[0].point()) << " squares" << endl;

        //

        partial_sort(intersections.begin(),
                     intersections.begin() + 1,
                     intersections.end(),
                     orderBy_steps);

        cout << "The fewest combined steps is at coordinates " << intersections[0].point()
             << " = " << intersections[0].steps() << " squares" << endl;

    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
    }
}
