#ifndef AUTOMATA_HW1_AUTOMATA_H_
#define AUTOMATA_HW1_AUTOMATA_H_

#include <array>
#include <cassert>
#include <initializer_list>
#include <istream>
#include <iterator>
#include <ostream>
#include <string_view>
#include <utility>
#include <vector>

#include "fixedset.hpp"

namespace athw1 {
constexpr int ctoa(const char c) {
  switch (c) {
  case 'e':
    return 0;
  case '0':
    return 1;
  case '1':
    return 2;
  }
  // LCOV_EXCL_START
  assert(false);
  return -1;
  // LCOV_EXCL_STOP
}

class PartialTransFunc {
private:
  // 0 -> eps, 1 -> 0, 2 -> 1
  std::array<std::vector<int>, 3> mapping_;

public:
  // IO
  static PartialTransFunc from_spec(std::istream &spec_src);

  std::ostream &to_spec(std::ostream &os) const {
    for (const auto &qs: mapping_) {
      os << qs.size();
      for (const int dst: qs)
        os << ' ' << dst;
      os << '\n';
    }
    return os;
  }

  // Main API
  constexpr const std::vector<int> &operator()(int a) const {
    return mapping_[a];
  }

  // Modifiers
  void connect(int a, int q) {
    mapping_[a].push_back(q);
  }

  void connect(int a, std::initializer_list<int> qs) {
    mapping_[a].insert(mapping_[a].end(), qs.begin(), qs.end());
  }

  void rebase(const int offset) {
    for (auto &qs: mapping_)
      for (int &q: qs)
        q += offset;
  }

private:
  constexpr std::vector<int> &operator[](int a) {
    return mapping_[a];
  }
};

class TransFunc {
private:
  std::vector<PartialTransFunc> tbl_;

public:
  // Ctors
  TransFunc() = default;

  static TransFunc from_alphabet(int a) {
    TransFunc tf(2);
    tf.tbl_[0].connect(a, 1);
    return tf;
  }

  // IO
  static TransFunc from_spec(std::istream &spec_src, int N = 0);

  std::ostream &to_spec(std::ostream &os) const {
    for (int i = 0; i < tbl_.size(); ++i)
      tbl_[i].to_spec(os);
    return os;
  }

  // Main API
  const std::vector<int> &operator()(int q, int a) const {
    return tbl_[q](a);
  }

  // Modifiers
  int add_state() {
    const int next = size();
    tbl_.emplace_back();
    return next;
  }

  void connect_states(int src, int a, int dst) {
    tbl_[src].connect(a, dst);
  }

  void connect_states(int src, int a, std::initializer_list<int> dsts) {
    tbl_[src].connect(a, dsts);
  }

  TransFunc &join(TransFunc &&other);

  // Utilities
  int size() const {
    return tbl_.size();
  }

private:
  explicit TransFunc(int N): tbl_(N) { }

  void rebase(const int offset) {
    for (auto &func: tbl_)
      func.rebase(offset);
  }

  PartialTransFunc &operator[](int q) {
    return tbl_[q];
  }

  const PartialTransFunc &operator[](int q) const {
    return tbl_[q];
  }
};

class Automata {
private:
  TransFunc func_;
  int init_;
  int fini_;

public:
  // IO
  static Automata from_regex(const std::string_view &regex);

  static Automata from_spec(std::istream &spec_src) {
    int N, init, fini;
    spec_src >> N >> init >> fini;
    return { TransFunc::from_spec(spec_src, N), init, fini };
  }

  std::ostream &to_spec(std::ostream &os) const {
    os << size() << ' ' << initial() << ' ' << final() << '\n';
    return func_.to_spec(os);
  }

  // Main API
  bool accepts(const std::string_view &str) const;

  Automata &concat(Automata &&other);
  Automata &altern(Automata &&other);
  Automata &kleene();

  // Utilities
  int size() const {
    return func_.size();
  }

  constexpr int initial() const {
    return init_;
  }

  constexpr int final() const {
    return fini_;
  }

private:
  // Ctors
  Automata() = default;

  Automata(const TransFunc &tbl, int init, int fini)
    : func_(tbl), init_(init), fini_(fini) { }

  Automata(TransFunc &&tbl, int init, int fini) noexcept
    : func_(std::move(tbl)), init_(init), fini_(fini) { }

  // Internal methods
  void new_init_fini();
  void rebase_idxs(int offset);
  void join(Automata &other);
};
} // namespace athw1

#endif /* AUTOMATA_HW1_AUTOMATA_H_ */
