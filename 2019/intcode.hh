#include <deque>
#include <ostream>
#include <string>
#include <vector>

#include <cstdint>

class Program {
public:
    enum {
        TechnicalAddressLimit = 1024 * 1024,
    };

    enum class State : uint32_t {
        Halted,
        Running,
        NeedInput,
    };

    using data_t  = int64_t;
    using udata_t = uint64_t;

    //

    std::deque<data_t> input;
    std::deque<data_t> output;

    //

    data_t& operator[] (const data_t addr);

    //

    void start(const std::deque<data_t>& input = {});

    State step();
    State run();

    State run(const std::deque<data_t>& input);

    //

    static Program read(const std::string& fileName);

private:
    std::vector<data_t> m_code;
    data_t m_ip = 0;
    data_t m_relBase = 0;

    //

    data_t& access(data_t addr,
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
