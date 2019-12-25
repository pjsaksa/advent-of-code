#include <ostream>
#include <string>
#include <vector>

std::vector<int> readCodes(const std::string& fileName);
std::vector<int> runCodes(std::vector<int>& codes,
                          const std::vector<int>& inputs = {});

std::ostream& operator<< (std::ostream& out,
                          const std::vector<int>& codes);
