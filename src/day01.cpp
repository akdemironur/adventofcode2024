#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

std::pair<std::vector<int>, std::vector<int>>
read_input(std::string_view filename) {
  std::vector<int> left;
  std::vector<int> right;

  std::ifstream file(filename.data());
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: " + std::string(filename));
  }

  int l, r;
  while (file >> l >> r) {
    left.push_back(l);
    right.push_back(r);
  }

  return {left, right};
}

int main() {
  auto [left, right] = read_input("inputs/day01.txt");
  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());
  auto sum_of_distances = std::accumulate(std::views::zip(left, right).begin(),
                                          std::views::zip(left, right).end(), 0,
                                          [](int acc, const auto &pair) {
                                            const auto &[l, r] = pair;
                                            return acc + std::abs(l - r);
                                          });

  auto right_counts = std::ranges::fold_left(
      right, std::unordered_map<int, int>{}, [](auto map, const int num) {
        map[num]++;
        return map;
      });

  auto similarity_score =
      std::ranges::fold_left(left, 0, [&right_counts](int acc, const int l) {
        return acc + l * right_counts[l];
      });

  std::cout << sum_of_distances << std::endl;
  std::cout << similarity_score << std::endl;

  return 0;
}
