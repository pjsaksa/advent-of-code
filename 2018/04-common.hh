#ifndef AOC_2018_04_COMMON_HEADER
#define AOC_2018_04_COMMON_HEADER

#include <functional>
#include <iosfwd>
#include <string>
#include <vector>

struct TimeStamp {
    int month;
    int day;
    int hour;
    int minute;

    TimeStamp() = default;
    TimeStamp(int pmonth, int pday, int phour, int pminute)
        : month(pmonth), day(pday), hour(phour), minute(pminute) {}
};

inline bool operator< (const TimeStamp& left, const TimeStamp& right)
{
    return (left.month != right.month) ? (left.month < right.month)
        : (left.day != right.day)      ? (left.day < right.day)
        : (left.hour != right.hour)    ? (left.hour < right.hour)
        : (left.minute < right.minute);
}

std::istream& operator>> (std::istream& in, TimeStamp& stamp);

// ------------------------------------------------------------

struct LogEntry {
    enum class Action {
        Invalid,
        BeginsShift,
        FallsAsleep,
        WakesUp,
    };

    TimeStamp time;
    Action action;
    int guard;

    LogEntry() = default;
    LogEntry(const TimeStamp& ptime, Action paction, int pguard)
        : time(ptime), action(paction), guard(pguard) {}
};

inline bool operator< (const LogEntry& left, const LogEntry& right)
{
    return left.time < right.time;
}

std::istream& operator>> (std::istream& in, LogEntry& entry);

// ------------------------------------------------------------

using entries_t = std::vector<LogEntry>;

// ------------------------------------------------------------

entries_t readInput(const std::string& name);

void forEachSleepCycle(const entries_t& entries,
                       std::function<void(int, int, int)> callback);

#endif
