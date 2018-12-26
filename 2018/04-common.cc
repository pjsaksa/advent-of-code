#include "04-common.hh"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

std::istream& operator>> (std::istream& in, TimeStamp& stamp)
{
    // "1518-06-12 23:57"

    int year;       // cached values
    int month;
    int day;
    int hour;
    int minute;

    char minus1;    // separators
    char minus2;
    char colon;

    if (in >> year >> minus1 >> month >> minus2 >> day >> hour >> colon >> minute)
    {
        if (minus1 == '-'
            && minus2 == '-'
            && colon == ':')
        {
            if (year == 1518
                && month >= 1 && month <= 12
                && day >= 1 && day <= 31
                && hour >= 0 && hour <= 23
                && minute >= 0 && minute <= 59)
            {
                stamp = TimeStamp(month, day, hour, minute);
            }
            else {
                std::cerr <<
                    "TimeStamp values are not valid: "
                    "[" << year << '-' << month << '-' << day << ' ' << hour << ':' << minute << ']' << std::endl;
            }
        }
        else in.setstate(std::ios_base::failbit);
    }

    return in;
}

// ------------------------------------------------------------

std::istream& operator>> (std::istream& in, LogEntry& entry)
{
    // [1518-06-12 23:57] Guard #2633 begins shift
    // [1518-04-10 00:56] falls asleep
    // [1518-10-22 00:36] wakes up

    char lbrac;         // separators
    char rbrac;

    TimeStamp time;     // cached values
    int guard;

    std::string restOfLine;

    if (in >> lbrac >> time >> rbrac >> std::ws
        && getline(in, restOfLine))
    {
        if (lbrac == '['
            && rbrac == ']')
        {
            if (restOfLine == "falls asleep")
            {
                entry = LogEntry(time, LogEntry::Action::FallsAsleep, guard);
            }
            else if (restOfLine == "wakes up")
            {
                entry = LogEntry(time, LogEntry::Action::WakesUp, guard);
            }
            else {
                std::istringstream lineIn(restOfLine);

                std::string textGuard;   // separators
                char hash;
                std::string textBegins;
                std::string textShift;

                if (lineIn >> textGuard >> hash >> guard >> textBegins >> textShift)
                {
                    if (textGuard == "Guard"
                        && hash == '#'
                        && textBegins == "begins"
                        && textShift == "shift")
                    {
                        entry = LogEntry(time, LogEntry::Action::BeginsShift, guard);
                    }
                    else in.setstate(std::ios_base::failbit);
                }
                else in.setstate(std::ios_base::failbit);
            }
        }
        else in.setstate(std::ios_base::failbit);
    }

    return in;
}

// ------------------------------------------------------------

entries_t readInput(const std::string& name)
{
    entries_t entries;

    std::ifstream input(name);
    std::string line;

    while (getline(input, line))
    {
        std::istringstream issLine(line);
        LogEntry entry;

        if (issLine >> entry)
            entries.push_back(entry);
    }

    stable_sort(entries.begin(),
                entries.end());

    return entries;
}

// ------------------------------------------------------------

void forEachSleepCycle(const entries_t& entries,
                       std::function<void(int, int, int)> callback)
{
    enum {
        NoGuard     =  0,   // guard 0 does no exist in the input
        NotSleeping = -1,   // minute -1 does not exist in the input
    };

    int guard = NoGuard;
    int sleepBegin = NotSleeping;

    for (auto& entry : entries)
    {
        switch (entry.action) {
        default:
        case LogEntry::Action::Invalid:
            throw std::runtime_error("log entry has invalid action");

        case LogEntry::Action::BeginsShift:
            if (sleepBegin != NotSleeping)
                throw std::runtime_error("elves are changing shift while sleeping");

            //

            guard = entry.guard;
            sleepBegin = NotSleeping;
            break;

        case LogEntry::Action::FallsAsleep:
            if (guard == NoGuard)
                throw std::runtime_error("elves are falling asleep before their shift begins");
            if (sleepBegin != NotSleeping)
                throw std::runtime_error("elves are falling asleep while sleeping");

            //

            sleepBegin = entry.time.minute;
            break;

        case LogEntry::Action::WakesUp:
            if (guard == NoGuard)
                throw std::runtime_error("elves are waking up before their shift begins");
            if (sleepBegin == NotSleeping)
                throw std::runtime_error("elves are waking up before falling asleep");

            const int sleepEnd = entry.time.minute;

            if (sleepBegin == sleepEnd)
                throw std::runtime_error("elves are sleeping 0 minutes");
            if (sleepBegin > sleepEnd)
                throw std::runtime_error("elves are sleeping negative minutes");

            //

            callback(guard, sleepBegin, sleepEnd);
            sleepBegin = NotSleeping;
            break;
        }
    }
}
