#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class Object {
public:
    int orbitCount = 0;
    mutable int track = 0;

    //

    Object(const string& n)
        : m_name(n) {}

    const string& name() const
    {
        return m_name;
    }

    Object* center() const
    {
        return m_center;
    }

    bool orphan() const
    {
        return m_center == nullptr
            && m_name != "COM";
    }

    void forEachSatellite(function<void(Object&)> oper)
    {
        for (auto& s : m_satellites)
        {
            oper(*s);
        }
    }

    void addSatellite(Object& satellite)
    {
        m_satellites.push_back(&satellite);
        satellite.m_center = this;
    }

private:
    string m_name;

    Object* m_center = nullptr;
    vector<Object*> m_satellites;
};

using objects_t = map<string, unique_ptr<Object>>;

void countOrbits(Object& obj)
{
    if (Object* center = obj.center();
        center)
    {
        obj.orbitCount = center->orbitCount + 1;
    }

    obj.forEachSatellite(countOrbits);
}

objects_t readObjects(const string& fileName)
{
    ifstream ifs(fileName);
    string line;

    objects_t objects;

    while (getline(ifs, line))
    {
        const string::size_type arc = line.find(')');

        if (arc == string::npos)
            throw runtime_error("line does not have ) character: " + line);

        const string o1 = line.substr(0, arc);
        const string o2 = line.substr(arc + 1);

        if (objects.find(o1) == objects.end())
            objects.insert(make_pair(o1, make_unique<Object>(o1)));
        if (objects.find(o2) == objects.end())
            objects.insert(make_pair(o2, make_unique<Object>(o2)));

        objects[o1]->addSatellite(*objects[o2]);
    }

    for (const auto& o : objects)
    {
        if (o.second->orphan()) {
            throw runtime_error("orphan object found: " + o.second->name());
        }
    }

    //

    auto iter = objects.find("COM");
    if (iter == objects.end()) {
        throw runtime_error("no COM found");
    }

    countOrbits(*iter->second);

    return objects;
}

int trackOrbits(const objects_t& objects,
                const string& name1,
                const string& name2)
{
    {
        auto iter = objects.find(name1);
        if (iter == objects.end()) {
            throw runtime_error("no " + name1 + " found");
        }

        Object* obj = iter->second.get();
        obj = obj->center();

        for (int trackNum = 0;
             obj != nullptr;
             obj = obj->center())
        {
            obj->track = trackNum++;
        }
    }

    {
        auto iter = objects.find(name2);
        if (iter == objects.end()) {
            throw runtime_error("no " + name2 + " found");
        }

        Object* obj = iter->second.get();
        obj = obj->center();

        for (int trackNum = 0;
             obj != nullptr;
             obj = obj->center())
        {
            if (obj->track) {
                return obj->track + trackNum;
            }

            obj->track = trackNum++;
        }
    }

    throw runtime_error("no path found between " + name1 + " and " + name2);
}

int main()
{
    try {
        const objects_t objects = readObjects("input-06.txt");

        cout << "objects: " << objects.size() << endl;

        int totalOrbitCount = 0;

        for (const auto& obj : objects)
        {
            totalOrbitCount += obj.second->orbitCount;
        }

        cout << "Total orbit count: " << totalOrbitCount << endl;
        cout << "Orbits between YOU and SAN: " << trackOrbits(objects, "YOU", "SAN") << endl;
    }
    catch (exception& e) {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
