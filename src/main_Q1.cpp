#include <iostream>
#include <string>

#include "automata.h"

int main() {
  std::string regex;
  std::getline(std::cin, regex);
  athw1::Automata::from_regex(regex).to_spec(std::cout);
  return 0;
}
