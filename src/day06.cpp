#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

template <typename T1, typename T2> struct std::hash<std::pair<T1, T2>> {
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
  }
};

std::vector<std::string> parse_file(std::string_view filename) {
  std::ifstream file(filename.data());
  std::string line;
  std::vector<std::string> input;
  while (std::getline(file, line)) {
    input.push_back(std::move(line));
  }
  return input;
}

typedef std::pair<size_t, size_t> Coord;

constexpr Coord rotate(const Coord &d) { return {d.second, -d.first}; }

Coord initial_coord(std::vector<std::string> &input) {
  for (size_t i = 0; i < input.size(); ++i) {
    for (size_t j = 0; j < input[i].size(); ++j) {
      if (input[i][j] == '^') {
        input[i][j] = '.';
        return {i, j};
      }
    }
  }
  return {-1, -1};
}

bool valid_coord(const std::vector<std::string> &input, const Coord &position) {
  return position.first < input.size() && position.second < input[0].size() &&
         position.first >= 0 && position.second >= 0;
}

bool path_loops_with_patch(
    const std::vector<std::string> &input, const Coord &pos, const Coord &dir,
    const Coord &obstacle,
    const std::unordered_set<std::pair<Coord, Coord>> &visited_with_direction) {

  std::unordered_set<std::pair<Coord, Coord>> local_visited_with_direction;
  Coord current_pos = pos;
  Coord current_dir = dir;
  while (valid_coord(input, current_pos)) {
    Coord next = {current_pos.first + current_dir.first,
                  current_pos.second + current_dir.second};

    if (!valid_coord(input, next))
      return false;

    if (next == obstacle || input[next.first][next.second] == '#') {
      current_dir = rotate(current_dir);
    } else {
      current_pos = next;
    }

    if (local_visited_with_direction.contains({current_pos, current_dir}) ||
        visited_with_direction.contains({current_pos, current_dir}))
      return true;
    local_visited_with_direction.insert({current_pos, current_dir});
  }
  return false;
}

int main() {
  auto input = parse_file("./inputs/day06.txt");
  auto position = initial_coord(input);
  std::unordered_set<Coord> visited{position};
  Coord direction = {-1, 0};
  std::unordered_set<std::pair<Coord, Coord>> visited_with_direction{
      {position, direction}};
  std::unordered_set<Coord> possible_obstacles;
  while (valid_coord(input, position)) {
    Coord new_pos = {position.first + direction.first,
                     position.second + direction.second};
    if (!valid_coord(input, new_pos)) {
      break;
    }

    if (input[new_pos.first][new_pos.second] == '#') {
      direction = rotate(direction);
    } else {
      if (!visited.contains(new_pos) &&
          path_loops_with_patch(input, position, direction, new_pos,
                                visited_with_direction)) {
        possible_obstacles.insert(new_pos);
      }
      position = new_pos;
    }
    visited.insert(position);
    visited_with_direction.insert({position, direction});
  }
  std::cout << visited.size() << std::endl;
  std::cout << possible_obstacles.size() << std::endl;
  return 0;
}