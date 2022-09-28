#include <iostream>
#include <string>

#include "automata.h"

int main() {
  using Automata = athw1::Automata;

  std::string regex;
  std::cin >> regex;

  Automata::from_regex(regex).to_spec(std::cout);

  return 0;
}
