#include "04-common.hh"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

//

using indexedMinutes_t = map<int, int>;
using minuteSlot_t     = pair<int, int>;
using minuteDistr_t    = vector<minuteSlot_t>;

// ------------------------------------------------------------

indexedMinutes_t accumulateSleepingMinutes(const entries_t& entries)
{
    indexedMinutes_t totalSleepingMinutes;

    forEachSleepCycle(entries,
                      [&totalSleepingMinutes](int guard, int sleepBegin, int sleepEnd)
                      {
                          const int duration = sleepEnd - sleepBegin;

                          totalSleepingMinutes[guard] += duration;
                      });

    return totalSleepingMinutes;
}

// ------------------------------------------------------------

minuteSlot_t accumulateMinuteSlots(const entries_t& entries, int watchedGuard)
{
    minuteDistr_t distribution(60);

    // initialize

    for (int minute = 0;
         minute < 60;
         ++minute)
    {
        distribution[minute] = {minute, 0};
    }

    // scan

    forEachSleepCycle(entries,
                      [&distribution, watchedGuard](int sleepingGuard, int sleepBegin, int sleepEnd)
                      {
                          if (watchedGuard == sleepingGuard)
                          {
                              for (int minute = sleepBegin;
                                   minute < sleepEnd;
                                   ++minute)
                              {
                                  ++distribution[minute].second;
                              }
                          }
                      });

    // find largest

    return *max_element(distribution.begin(),
                        distribution.end(),
                        [](const minuteSlot_t& left, const minuteSlot_t& right)
                        {
                            return left.second < right.second;
                        });
}

// ------------------------------------------------------------

int main()
{
    try {
        const auto entries = readInput("input-04.txt");
        const auto totalSleepingMinutes = accumulateSleepingMinutes(entries);

        const auto guardIter = max_element(totalSleepingMinutes.begin(),
                                           totalSleepingMinutes.end(),
                                           [](auto& left, auto& right)
                                           {
                                               return left.second < right.second;
                                           });

        const auto slot = accumulateMinuteSlots(entries, guardIter->first);

        cout <<
            "Guard #" << guardIter->first << " slept " << guardIter->second << " minutes.\n"
            "The minute slot " << slot.first << " was the most sleepy.\n"
            "Answer: " << guardIter->first << '*' << slot.first << " = " << (guardIter->first * slot.first) << endl;

    }
    catch (exception& e) {
        cout << "exception: " << e.what() << endl;
    }
}
