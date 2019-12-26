#include "intcode.hh"

#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

int& Program::operator[] (const int index)
{
    return m_code.at(index);
}

void Program::start(const std::deque<int>& newInput)
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
            const int& src1 = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);
            const int& src2 = access(m_ip + 2, m_code.at(m_ip) / 1000 % 10);
            int&       dst  = access(m_ip + 3, m_code.at(m_ip) / 10000 % 10);

            dst = src1 + src2;

            m_ip += 4;
        }
        break;

    case 2:
        {
            const int& src1 = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);
            const int& src2 = access(m_ip + 2, m_code.at(m_ip) / 1000 % 10);
            int&       dst  = access(m_ip + 3, m_code.at(m_ip) / 10000 % 10);

            dst = src1 * src2;

            m_ip += 4;
        }
        break;

    case 3:
        {
            if (input.empty()) {
                return State::NeedInput;
            }

            int& dst = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);

            dst = input.front();
            input.pop_front();

            m_ip += 2;
        }
        break;

    case 4:
        {
            int& src = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);

            output.push_back(src);

            m_ip += 2;
        }
        break;

    case 5:
        {
            int& cond = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);
            int& tgt  = access(m_ip + 2, m_code.at(m_ip) / 1000 % 10);

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
            int& cond = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);
            int& tgt  = access(m_ip + 2, m_code.at(m_ip) / 1000 % 10);

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
            int& cond1 = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);
            int& cond2 = access(m_ip + 2, m_code.at(m_ip) / 1000 % 10);
            int& dst   = access(m_ip + 3, m_code.at(m_ip) / 10000 % 10);

            dst = (cond1 < cond2);

            m_ip += 4;
        }
        break;

    case 8:
        {
            int& cond1 = access(m_ip + 1, m_code.at(m_ip) / 100 % 10);
            int& cond2 = access(m_ip + 2, m_code.at(m_ip) / 1000 % 10);
            int& dst   = access(m_ip + 3, m_code.at(m_ip) / 10000 % 10);

            dst = (cond1 == cond2);

            m_ip += 4;
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

Program::State Program::run(const std::deque<int>& newInput)
{
    start(newInput);
    return run();
}

Program Program::read(const string& fileName)
{
    ifstream input(fileName);
    int code;
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

int& Program::access(const int ip,
                     const int accessMode)
{
    switch (accessMode) {
    case 0: return m_code.at(m_code.at(ip));
    case 1: return m_code.at(ip);
    default: throw runtime_error("invalid access mode at " + to_string(ip));
    }
}
