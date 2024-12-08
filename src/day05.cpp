#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>>
parse_file(std::string_view filename) {
  std::ifstream file(filename.data());
  std::string line;
  std::vector<std::vector<int>> pairs;
  std::vector<std::vector<int>> seqs;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }
    bool reading_pair = line.contains('|');
    char delimiter = reading_pair ? '|' : ',';
    std::stringstream ss(line);
    std::vector<int> numbers;
    std::string number_str;
    while (std::getline(ss, number_str, delimiter)) {
      numbers.push_back(std::stoi(number_str));
    }
    if (reading_pair) {
      pairs.push_back(numbers);
    } else {
      seqs.push_back(numbers);
    }
  }
  return {pairs, seqs};
}

bool check_and_fix(std::unordered_map<int, std::set<int>> &befores,
                   std::vector<int> &seq) {
  bool valid = true;
  for (size_t i = 0; i < seq.size(); ++i) {
    for (size_t j = i + 1; j < seq.size(); ++j) {
      if (befores[seq[i]].contains(seq[j])) {
        valid = false;
        std::swap(seq[i], seq[j]);
      }
    }
  }
  return valid;
}

int main() {
  auto [pairs, arrays] = parse_file("./inputs/day05.txt");
  std::unordered_map<int, std::set<int>> befores;
  for (const auto &pair : pairs) {
    befores[pair[1]].insert(pair[0]);
  }
  int sum_valid = 0;
  int sum_invalid = 0;
  for (auto &arr : arrays) {
    bool valid = check_and_fix(befores, arr);
    int mid = arr[arr.size() / 2];
    if (valid) {
      sum_valid += mid;
    } else {
      sum_invalid += mid;
    }
  }
  std::cout << sum_valid << std::endl;
  std::cout << sum_invalid << std::endl;
}
