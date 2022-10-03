#include <ios>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "automata.h"

namespace athw1 {
namespace {
  constexpr auto kStreamMax = std::numeric_limits<std::streamsize>::max();

  std::string read_string(std::istream &is) {
    std::string s;
    std::getline(is, s);
    return s;
  }
} // namespace
} // namespace athw1

int main() {
  int M;
  std::cin >> M;
  std::cin.ignore(athw1::kStreamMax, '\n');

  std::vector<std::string> strings;
  strings.reserve(M);
  for (int i = 0; i < M; ++i)
    strings.push_back(athw1::read_string(std::cin));

  auto automata = athw1::Automata::from_spec(std::cin);
  for (const auto &s: strings)
      std::cout << (automata.accepts(s) ? "yes" : "no") << '\n';

  return 0;
}
