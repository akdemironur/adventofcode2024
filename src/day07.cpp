#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

struct Prob {
  long long trg;
  std::vector<long long> numbers;
};

template <typename T> constexpr T llpow(T base, int exp) noexcept {
  T result = 1;
  while (exp > 0) {
    if (exp & 1) {
      result *= base;
    }
    base *= base;
    exp >>= 1;
  }
  return result;
}

constexpr int count_digits(long long n) noexcept {
  if (n == 0)
    return 1;
  if (n < 0)
    n = -n;
  return 1 + static_cast<int>(std::log10(n));
}

Prob parse_line(const std::string &line) {
  Prob data;
  std::stringstream ss(line);
  std::string temp;

  getline(ss, temp, ':');
  data.trg = std::stoull(temp);

  while (ss >> temp) {
    data.numbers.push_back(std::stoull(temp));
  }
  std::reverse(data.numbers.begin(), data.numbers.end());
  return data;
}

std::vector<Prob> parse_file(std::string_view filename) {
  std::vector<Prob> allData;
  std::ifstream file(filename.data());
  std::string line;

  if (!file.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  while (getline(file, line)) {
    if (!line.empty()) {
      allData.push_back(parse_line(line));
    }
  }

  return allData;
}

bool made_true(const Prob &prob, bool part2) {
  if (prob.trg < 0 || prob.numbers.empty()) {
    return false;
  }

  const long long firstNum = prob.numbers[0];
  const std::vector<long long> remainingNumbers(prob.numbers.begin() + 1,
                                                prob.numbers.end());

  if (prob.numbers.size() == 1) {
    const bool additionCase = (prob.trg - firstNum) == 0;
    const bool multiplicationCase =
        prob.trg % firstNum == 0 && prob.trg / firstNum == 1;

    if (part2) {
      const long long digits = count_digits(firstNum);
      const bool concatenationCase =
          prob.trg % llpow(10LL, digits) == firstNum &&
          prob.trg / llpow(10LL, digits) == 0;
      return additionCase || multiplicationCase || concatenationCase;
    }
    return additionCase || multiplicationCase;
  }

  Prob nextProb{.trg = 0LL, .numbers = remainingNumbers};

  nextProb.trg = prob.trg - firstNum;
  if (made_true(nextProb, part2)) {
    return true;
  }

  if (prob.trg % firstNum == 0) {
    nextProb.trg = prob.trg / firstNum;
    if (made_true(nextProb, part2)) {
      return true;
    }
  }

  if (part2) {
    const long long digits = count_digits(firstNum);
    if (prob.trg % llpow(10LL, digits) == firstNum) {
      nextProb.trg = prob.trg / llpow(10LL, digits);
      if (made_true(nextProb, part2)) {
        return true;
      }
    }
  }

  return false;
}

int main() {

  auto allProbs = parse_file("./inputs/day07.txt");
  long long sum = 0;
  long long sum2 = 0;
  for (const auto &p : allProbs) {
    if (made_true(p, false)) {
      sum += p.trg;
      sum2 += p.trg;
    } else {
      if (made_true(p, true)) {
        sum2 += p.trg;
      }
    }
  }
  std::cout << sum << std::endl;
  std::cout << sum2 << std::endl;
}