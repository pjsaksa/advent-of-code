#include <atomic>
#include <chrono>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
using namespace chrono_literals;

//

struct Area {
    int x1, y1;
    int x2, y2;

    Area() = default;
    Area(int px1, int py1, int px2, int py2)
     : x1(px1), y1(py1), x2(px2), y2(py2) {}

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
            area = Area(left, top, left + width, top + height);
        }
        else {
            in.setstate(ios_base::failbit);
        }
    }

    return in;
}

optional<Area> intersection(const Area& a, const Area& b)
{
    const Area o(max(a.x1, b.x1), max(a.y1, b.y1),
                 min(a.x2, b.x2), min(a.y2, b.y2));

    if (o)
        return o;
    else
        return {};
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

areas_t storeIntersections(const areas_t& areas)
{
    areas_t overlaps;

    for (auto leftIter = areas.begin();
         leftIter != areas.end();
         ++leftIter)
    {
        for (auto rightIter = leftIter + 1;
             rightIter != areas.end();
             ++rightIter)
        {
            auto overlap = intersection(*leftIter,
                                        *rightIter);

            if (overlap) {
                overlaps.push_back(*overlap);
            }
        }
    }

    return overlaps;
}

int calculateExcessArea(const Area& area,
                        areas_t::const_iterator begin,
                        areas_t::const_iterator end)
{
    auto result = 0;

    for (auto iter = begin;
         iter != end;
         ++iter)
    {
        auto overlap = intersection(*iter,
                                    area);

        if (overlap) {
            result += overlap->size() - calculateExcessArea(*overlap,
                                                            begin,
                                                            iter);
        }
    }

    return result;
}

//

atomic_int s_numberOfTasks = 0;

int calculateOverlapArea(areas_t::const_iterator begin,
                         areas_t::const_iterator iter)
{
    ++s_numberOfTasks;

    const int result = iter->size() - calculateExcessArea(*iter,
                                                          begin,
                                                          iter);

    --s_numberOfTasks;
    return result;
}

int main()
{
    static const areas_t overlaps = storeIntersections(readInput("input-03.txt"));
    vector<future<int>> areaSizes;

    /* readInput() returns the areas given as input. 'overlaps' contains their intersections, so calculating the union
     * of these areas is the answer to the puzzle.
     *
     * Area sizes are calculated individually so that these calculations can be done in parallel. The results are stored
     * into 'areaSizes' and finally accumulated to 'totalResult'.
     */

    for (auto iter = overlaps.begin();
         iter != overlaps.end();
         ++iter)
    {
        // First add item into the vector, then refer to it and start an asyncronic task. This prevents race conditions.

        auto sizeIter = areaSizes.insert(areaSizes.end(), future<int>());

        *sizeIter = async(calculateOverlapArea,
                          overlaps.begin(),
                          iter);

        //
        cout << "tasks: " << setw(4) << distance(overlaps.begin(), iter) << " / " << overlaps.size() << '\r'
             << flush;
        //

        /* If the task completes within one second, start another task immediately. Otherwise start another task (after
         * that one second) unless there already exists too many tasks.
         */

        future_status status = sizeIter->wait_for(1s);

        if (status != future_status::ready) {
            while (s_numberOfTasks >= 4) {
                this_thread::sleep_for(500ms);
            }
        }
    }

    cout << "tasks: " << overlaps.size() << " / " << overlaps.size() << endl;

    // Calculate the sum of all individual area sizes.

    auto totalResult = 0;

    for (auto& f : areaSizes) {
        totalResult += f.get();
    }

    cout << "total area: " << totalResult << endl;
}
