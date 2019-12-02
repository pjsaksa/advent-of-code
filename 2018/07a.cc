#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Task {
public:
    void addBlock(Task& other)
    {
        m_blocks.push_back(&other);
        ++other.m_blockedBy;
    }

    bool canBeCompleted() const
    {
        return !m_completed
            && m_blockedBy == 0;
    }

    void complete()
    {
        if (!canBeCompleted())
            throw runtime_error("completing task which can not be completed");

        for (auto* task : m_blocks)
        {
            --task->m_blockedBy;
        }

        m_completed = true;
    }

private:
    bool m_completed = false;
    int m_blockedBy  = 0;

    vector<Task*> m_blocks;
};

//

using tasks_t = map<char, Task>;

//

istream& operator>> (istream& in, const string& text)
{
    string input;

    if (in >> input
        && input != text)
    {
        in.setstate(ios_base::failbit);
    }

    return in;
}

//

void readDependency(const string& line, tasks_t& tasks)
{
    // "Step G must be finished before step N can begin."

    istringstream in(line);

    char step1;
    char step2;

    if (in >> "Step" >> step1 >> "must" >> "be" >> "finished" >> "before" >> "step" >> step2 >> "can" >> "begin.")
    {
        auto iter1 = tasks.find(step1);
        auto iter2 = tasks.find(step2);

        if (iter1 != tasks.end()
            && iter2 != tasks.end())
        {
            auto& task1 = iter1->second;
            auto& task2 = iter2->second;

            task1.addBlock(task2);
        }
        else in.setstate(ios_base::failbit);
    }

    if (!in)
        throw runtime_error("invalid input");
}

void readInput(const string& name, tasks_t& tasks)
{
    ifstream input(name);
    string line;

    while (getline(input, line)) {
        readDependency(line, tasks);
    }
}

int main()
{
    try {
        tasks_t tasks;

        // all alphabets a-z appear to be included

        for (int k = 0;
             k < 26;
             ++k)
        {
            tasks.insert(make_pair(static_cast<char>('A' + k), Task()));
        }

        // fill dependencies

        readInput("input-07.txt", tasks);

        // complete

        bool restart;
        do {
            restart = false;

            for (auto& unit : tasks)
            {
                const char id = unit.first;
                Task& task    = unit.second;

                //

                if (task.canBeCompleted())
                {
                    task.complete();

                    cout << id;

                    restart = true;     // start from beginning
                    break;
                }
            }
        }
        while (restart);

        cout << endl;
    }
    catch (exception& e) {
        cout << "exception: " << e.what() << endl;
    }
}
