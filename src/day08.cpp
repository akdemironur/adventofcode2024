#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::vector<std::string> read_file(std::string_view filename) {
  std::vector<std::string> allData;
  std::ifstream file(filename.data());
  std::string line;

  if (!file.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  while (getline(file, line)) {
    if (!line.empty()) {
      allData.push_back(line);
    }
  }

  return allData;
}

typedef std::pair<size_t, size_t> Coord;

template <typename T1, typename T2> struct std::hash<std::pair<T1, T2>> {
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
  }
};

std::unordered_map<char, std::unordered_set<Coord>>
antenna_locations(const std::vector<std::string> &input) {
  std::unordered_map<char, std::unordered_set<Coord>> antennas;
  for (size_t row = 0; row < input.size(); ++row) {
    for (size_t col = 0; col < input[row].size(); ++col) {
      char key = input[row][col];
      if (key == '.') {
        continue;
      }
      antennas[key].insert({row, col});
    }
  }
  return antennas;
}

std::pair<Coord, Coord> antinode_location(const Coord &a1, const Coord &a2) {
  Coord diff = {a1.first - a2.first, a1.second - a2.second};
  Coord an1{a1.first + diff.first, a1.second + diff.second};
  Coord an2{a2.first - diff.first, a2.second - diff.second};
  return {an1, an2};
}

bool in_map(const Coord &c, std::vector<std::string> input) {
  const size_t rowMax = input.size();
  const size_t colMax = input[0].size();
  return (c.first >= 0 && c.second >= 0 && c.first < rowMax &&
          c.second < colMax);
}

std::unordered_set<Coord>
antinode_location_with_harmonics(Coord a1, Coord a2,
                                 const std::vector<std::string> &input) {
  Coord diff = {a1.first - a2.first, a1.second - a2.second};
  std::unordered_set<Coord> ans;
  while (in_map(a1, input)) {
    ans.insert(a1);
    a1.first += diff.first;
    a1.second += diff.second;
  }

  while (in_map(a2, input)) {
    ans.insert(a2);
    a2.first -= diff.first;
    a2.second -= diff.second;
  }
  return ans;
}

int main() {
  auto input = read_file("./inputs/day08.txt");
  auto antennas = antenna_locations(input);
  std::unordered_set<Coord> antinodes;
  std::unordered_set<Coord> antinodes_with_harmonics;
  for (const auto &[antenna_freq, coordinates] : antennas) {
    for (auto it = coordinates.begin(); it != coordinates.end(); ++it) {
      Coord c1 = *it;
      for (auto it2 = std::next(it); it2 != coordinates.end(); ++it2) {
        Coord c2 = *it2;
        auto [an1, an2] = antinode_location(c1, c2);
        if (in_map(an1, input)) {
          antinodes.insert(an1);
        }
        if (in_map(an2, input)) {
          antinodes.insert(an2);
        }

        auto ans = antinode_location_with_harmonics(c1, c2, input);
        for (const auto &an : ans) {
          antinodes_with_harmonics.insert(an);
        }
      }
    }
  }

  std::cout << antinodes.size() << std::endl;
  std::cout << antinodes_with_harmonics.size() << std::endl;
}