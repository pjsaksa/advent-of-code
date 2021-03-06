#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//

vector<string> read_ids(const string& name)
{
    vector<string> ids;

    ifstream input(name);
    string id;

    while (input >> id) {
        ids.push_back(id);
    }

    return ids;
}

string matching_chars(const string& left,
                      const string& right)
{
    string match;

    for (string::size_type index = 0U;
         index < left.size();
         ++index)
    {
        if (left[index] == right[index])
            match += left[index];
    }

    return match;
}

void compare_two_ids(const string& left,
                     const string& right)
{
    if (left.size() != right.size())
        return;

    const auto match = matching_chars(left, right);

    // Assume best matches differ by 1 character:

    if (left.size() - match.size() == 1) {
        cout << "common letters: " << match << endl;
    }
}

template<typename container_type, typename callback_type>
void for_each_pair(const container_type& ids,
                   callback_type callback)
{
    const auto last = ids.end();

    for (auto left = ids.begin();
         left != last;
         ++left)
    {
        for (auto right = left + 1;
             right != last;
             ++right)
        {
            callback(*left, *right);
        }
    }
}

int main()
{
    for_each_pair(read_ids("input-02.txt"),
                  compare_two_ids);
}
