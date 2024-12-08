#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

struct Instruction {
  size_t position;
  std::string type;
  int num1;
  int num2;
};
struct Pattern {
  std::regex regex;
  std::string type;
};

std::array<Pattern, 3> patterns = {
    {{std::regex("mul\\((\\d+),(\\d+)\\)"), "mul"},
     {std::regex("do\\(\\)"), "do"},
     {std::regex("don't\\(\\)"), "dont"}}};

int main() {
  std::string corruptedMemory;
  std::ifstream file("./inputs/day03.txt");
  std::string line;
  while (std::getline(file, line)) {
    corruptedMemory += line;
  }

  auto begin = corruptedMemory.begin();
  auto end = corruptedMemory.end();
  std::vector<Instruction> instructions;
  for (const auto &pattern : patterns) {
    for (std::sregex_iterator i(begin, end, pattern.regex);
         i != std::sregex_iterator(); ++i) {
      std::smatch match = *i;
      if (pattern.type == "mul") {
        instructions.push_back({static_cast<size_t>(match.position()),
                                pattern.type, std::stoi(match[1].str()),
                                std::stoi(match[2].str())});
      } else {
        instructions.push_back(
            {static_cast<size_t>(match.position()), pattern.type, 0, 0});
      }
    }
  }

  std::sort(instructions.begin(), instructions.end(),
            [](const Instruction &a, const Instruction &b) {
              return a.position < b.position;
            });

  long long sum_all = 0;
  long long sum_enabled = 0;
  bool enabled = true;

  for (const auto &instr : instructions) {
    if (instr.type == "do") {
      enabled = true;
    }
    if (instr.type == "dont") {
      enabled = false;
    }
    if (instr.type == "mul") {
      long long num = instr.num1 * instr.num2;
      sum_all += num;
      sum_enabled += enabled ? num : 0;
    }
  }

  std::cout << "Sum of all muls: " << sum_all << std::endl;
  std::cout << "Sum of enabled muls: " << sum_enabled << std::endl;

  return 0;
}