#include <deque>
#include <ostream>
#include <string>
#include <vector>

class Program {
public:
    enum class State : uint32_t {
        Halted,
        Running,
        NeedInput,
    };

    //

    std::deque<int> input;
    std::deque<int> output;

    //

    int& operator[] (const int index);

    //

    void start(const std::deque<int>& input = {});

    State step();
    State run();

    State run(const std::deque<int>& input);

    //

    static Program read(const std::string& fileName);

private:
    std::vector<int> m_code;
    int m_ip = 0;

    //

    int& access(int ip,
                int accessMode);
};

// -----

template< typename Cont >
class as_csv {
public:
    as_csv(const Cont& c)
        : m_container(c) {}

    template< typename charT, typename traits >
    friend std::basic_ostream< charT, traits >& operator<< (std::basic_ostream< charT, traits >& out,
                                                            as_csv&& manip)
    {
        out << '{';

        if (auto iter = begin(manip.m_container);
            iter != end(manip.m_container))
        {
            out << *iter++;

            for (;
                 iter != end(manip.m_container);
                 ++iter)
            {
                out << ',' << *iter;
            }
        }

        out << '}';

        return out;
    }

private:
    const Cont& m_container;
};
