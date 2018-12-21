#include "04-common.hh"

#include <algorithm>
#include <iostream>
#include <map>

using namespace std;

//

using minuteSlot_t    = pair<int, int>;
using minuteSlotMap_t = map<minuteSlot_t, int>;

// ------------------------------------------------------------

minuteSlot_t findBusiestSlot(const entries_t& entries)
{
    minuteSlotMap_t allMinuteSlots;

    // scan

    forEachSleepCycle(entries,
                      [&allMinuteSlots](int guard, int sleepBegin, int sleepEnd)
                      {
                          for (int minute = sleepBegin;
                               minute < sleepEnd;
                               ++minute)
                          {
                              ++allMinuteSlots[{guard,minute}];
                          }
                      });

    // find largest

    return max_element(allMinuteSlots.cbegin(),
                       allMinuteSlots.cend(),
                       [](const auto& left, const auto& right)
                       {
                           return left.second < right.second;
                       }
                       )->first;
}

// ------------------------------------------------------------

int main()
{
    try {
        const auto slot = findBusiestSlot(readInput("input-04.txt"));

        cout <<
            "Guard #" << slot.first << " slept the most often during the minute slot " << slot.second << ".\n"
            "Answer: " << slot.first << '*' << slot.second << " = " << (slot.first * slot.second) << endl;
    }
    catch (exception& e) {
        cout << "exception: " << e.what() << endl;
    }
}
