#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

typedef std::vector<std::vector<int>> Input;

auto read_input(std::string_view filename) -> Input {
  Input result;
  std::ifstream file(filename.data());

  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: " + std::string(filename));
  }

  std::string line;
  while (std::getline(file, line)) {
    std::vector<int> numbers;
    std::istringstream iss(line);
    int number;

    while (iss >> number) {
      numbers.push_back(number);
    }

    if (!numbers.empty()) {
      result.push_back(std::move(numbers));
    }
  }

  return result;
}

auto is_safe_with_remove(const std::vector<int> &row, size_t idx) -> bool {
  bool increasing = false;
  bool decreasing = false;
  int prev = idx == 0 ? row[1] : row[0];
  for (size_t i = idx == 0 ? 2 : 1; i < row.size(); i++) {
    if (i == idx) {
      continue;
    }
    increasing |= row[i] > prev;
    decreasing |= row[i] < prev;
    int diff = std::abs(row[i] - prev);

    if ((increasing && decreasing) || (diff > 3) || (diff == 0)) {
      return false;
    }
    prev = row[i];
  }
  return true;
}

auto main() -> int {
  auto input = read_input("inputs/day02.txt");
  int safe_count = 0;
  int safe_count_with_remove = 0;
  for (const auto &row : input) {
    bool safe = is_safe_with_remove(row, -1);
    safe_count += safe;
    if (!safe) {
      for (size_t i = 0; i < row.size(); i++) {
        if (is_safe_with_remove(row, i)) {
          safe_count_with_remove++;
          break;
        }
      }
    }
  }
  std::cout << safe_count << std::endl;
  std::cout << safe_count_with_remove + safe_count << std::endl;
  return 0;
}
