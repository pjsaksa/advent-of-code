#include "intcode.hh"

#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

Program::data_t& Program::operator[] (const data_t addr)
{
    if (addr < 0)
        throw runtime_error("trying to access negative address: " + addr);
    if (m_code.size() <= static_cast<udata_t>( addr ))
        m_code.resize(addr + 1);

    return m_code.at(addr);
}

void Program::start(const std::deque<data_t>& newInput)
{
    m_ip = 0;
    input = newInput;
    output.clear();
}

Program::State Program::step()
{
    switch (m_code.at(m_ip) % 100) {
    case 1:
        {
            const data_t& src1 = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);
            const data_t& src2 = access(m_ip + 2, m_code.at(m_ip) / 1000 % 10);
            data_t&       dst  = access(m_ip + 3, m_code.at(m_ip) / 10000 % 10);

            dst = src1 + src2;

            m_ip += 4;
        }
        break;

    case 2:
        {
            const data_t& src1 = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);
            const data_t& src2 = access(m_ip + 2, m_code.at(m_ip) / 1000 % 10);
            data_t&       dst  = access(m_ip + 3, m_code.at(m_ip) / 10000 % 10);

            dst = src1 * src2;

            m_ip += 4;
        }
        break;

    case 3:
        {
            if (input.empty()) {
                return State::NeedInput;
            }

            data_t& dst = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);

            dst = input.front();
            input.pop_front();

            m_ip += 2;
        }
        break;

    case 4:
        {
            data_t& src = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);

            output.push_back(src);

            m_ip += 2;
        }
        break;

    case 5:
        {
            data_t& cond = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);
            data_t& tgt  = access(m_ip + 2, m_code.at(m_ip) / 1000 % 10);

            if (cond != 0) {
                m_ip = tgt;
            }
            else {
                m_ip += 3;
            }
        }
        break;

    case 6:
        {
            data_t& cond = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);
            data_t& tgt  = access(m_ip + 2, m_code.at(m_ip) / 1000 % 10);

            if (cond == 0) {
                m_ip = tgt;
            }
            else {
                m_ip += 3;
            }
        }
        break;

    case 7:
        {
            data_t& cond1 = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);
            data_t& cond2 = access(m_ip + 2, m_code.at(m_ip) / 1000 % 10);
            data_t& dst   = access(m_ip + 3, m_code.at(m_ip) / 10000 % 10);

            dst = (cond1 < cond2);

            m_ip += 4;
        }
        break;

    case 8:
        {
            data_t& cond1 = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);
            data_t& cond2 = access(m_ip + 2, m_code.at(m_ip) / 1000 % 10);
            data_t& dst   = access(m_ip + 3, m_code.at(m_ip) / 10000 % 10);

            dst = (cond1 == cond2);

            m_ip += 4;
        }
        break;

    case 9:
        {
            data_t& src = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);

            m_relBase += src;

            m_ip += 2;
        }
        break;

    case 99:
        return State::Halted;

    default:
        throw runtime_error("invalid code=" + to_string(m_code.at(m_ip)) + " at index=" + to_string(m_ip));
    }

    return State::Running;
}

Program::State Program::run()
{
    State state = State::Running;

    while (state == State::Running) {
        state = step();
    }

    return state;
}

Program::State Program::run(const std::deque<data_t>& newInput)
{
    start(newInput);
    return run();
}

Program Program::read(const string& fileName)
{
    ifstream input(fileName);
    data_t code;
    char comma;

    Program program;

    while (input >> code)
    {
        program.m_code.push_back(code);

        if (!(input >> comma
              && comma == ','))
        {
            break;
        }
    }

    return program;
}

Program::data_t& Program::access(const data_t addr,
                                 const int accessMode)
{
    switch (accessMode) {
    case 0: return (*this)[m_code.at(addr)];
    case 1: return (*this)[addr];
    case 2: return (*this)[m_code.at(addr) + m_relBase];
    default: throw runtime_error("invalid access mode at " + to_string(addr));
    }
}
