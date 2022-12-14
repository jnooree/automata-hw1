#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "automata.h"

namespace athw1 {
namespace {
  std::string read_strings(std::istream &is) {
    std::string s;
    std::getline(is, s);
    return s;
  }

  std::vector<std::string_view> split(const std::string &str, const int M) {
    std::vector<std::string_view> strings;
    strings.reserve(M);

    // Consider extra space after M
    int lastpos = 1;
    for (int i = 0; i < M - 1; ++i) {
      const int next = str.find(' ', lastpos);
      strings.push_back(std::string_view(str.data() + lastpos, next - lastpos));
      lastpos = next + 1;
    }
    strings.push_back(
      std::string_view(str.data() + lastpos, str.size() - lastpos));

    return strings;
  }
} // namespace
} // namespace athw1

int main() {
  int M;
  std::cin >> M;

  const auto line = athw1::read_strings(std::cin);
  const auto automata = athw1::Automata::from_spec(std::cin);
  const auto strings = athw1::split(line, M);

  for (const auto &s: strings)
    std::cout << (automata.accepts(s) ? "yes" : "no") << '\n';
  return 0;
}
