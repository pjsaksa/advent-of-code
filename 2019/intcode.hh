#include <string>
#include <vector>

void readCodes(std::vector<int>& codes,
               const std::string& fileName);

void runCodes(std::vector<int>& codes,
              const std::vector<int>& inputs = {});
