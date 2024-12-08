#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
std::vector<std::string> parse_file(std::string_view filename) {
  std::ifstream file(filename.data());
  std::string line;
  std::vector<std::string> input;
  while (std::getline(file, line)) {
    input.push_back(line);
  }

  return input;
}
bool check_letter(const std::vector<std::string> &input, int row, int col,
                  char c) {
  if (row < 0 || col < 0 || row >= (int)input.size() ||
      col >= (int)input[row].size()) {
    return false;
  }
  return input[row][col] == c;
}
int xmas(const std::vector<std::string> &input, int row, int col, int shift,
         const std::vector<int> &directions_row,
         const std::vector<int> &directions_col, const std::string &letters) {
  if (input[row][col] != letters[shift]) {
    return 0;
  }
  int numxmas = 0;
  for (size_t i = 0; i < directions_row.size(); ++i) {
    int dr = directions_row[i];
    int dc = directions_col[i];
    bool isxmas = true;
    for (int k = 0; k < (int)letters.size(); ++k) {
      isxmas = isxmas && check_letter(input, row + dr * (k - shift),
                                      col + dc * (k - shift), letters[k]);
    }
    numxmas += isxmas;
  }
  return numxmas;
}

int main() {
  auto input = parse_file("./inputs/day04.txt");

  std::vector<int> directions_row_all = {1, 1, 0, -1, -1, -1, 0, 1};
  std::vector<int> directions_col_all = {0, -1, -1, -1, 0, 1, 1, 1};
  std::vector<int> directions_row_diag = {1, -1, -1, 1};
  std::vector<int> directions_col_diag = {-1, -1, 1, 1};
  int numxmas = 0;
  for (size_t row = 0; row < input.size(); ++row) {
    for (size_t col = 0; col < input[row].size(); ++col) {
      numxmas += xmas(input, row, col, 0, directions_row_all,
                      directions_col_all, "XMAS");
    }
  }
  std::cout << numxmas << std::endl;

  int nummasx = 0;
  for (size_t row = 0; row < input.size(); ++row) {
    for (size_t col = 0; col < input[row].size(); ++col) {
      nummasx += (xmas(input, row, col, 1, directions_row_diag,
                       directions_col_diag, "MAS") == 2);
    }
  }
  std::cout << nummasx << std::endl;
}