#include "automata.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <stack>
#include <unordered_map>
#include <vector>

#include "fixedset.hpp"

namespace athw1 {
PartialTransFunc PartialTransFunc::from_spec(std::istream &spec_src) {
  PartialTransFunc pf;

  int n, q;
  for (int a = 0; a < 3; ++a) {
    spec_src >> n;
    pf[a].reserve(n);

    for (int j = 0; j < n; ++j) {
      spec_src >> q;
      pf.connect(a, q);
    }
  }

  return pf;
}

TransFunc &TransFunc::join(TransFunc &&other) {
  if (size() < other.size())
    std::swap(*this, other);

  other.rebase(size());
  tbl_.reserve(size() + other.size());
  std::move(other.tbl_.begin(), other.tbl_.end(), std::back_inserter(tbl_));

  return *this;
}

TransFunc TransFunc::from_spec(std::istream &spec_src, const int N) {
  TransFunc func;
  func.tbl_.reserve(N);

  for (int i = 0; i < N; ++i)
    func.tbl_.emplace_back(PartialTransFunc::from_spec(spec_src));

  return func;
}

namespace {
  using FixedSet = fixed_set<int>;
  using Cache = std::unordered_map<int, FixedSet>;

#ifndef NDEBUG
  size_t total_recursion = 0UL;
#endif

  void deterministic_transitions(const TransFunc &func,
                                 const FixedSet &curr_states,
                                 FixedSet &next_states, const int a) {
    assert(a != 0);
    for (auto q: curr_states) {
      const auto &next = func(q, a);
      assert(next.size() <= 1);
      if (!next.empty())
        next_states.insert(next[0]);
    }
  }

  // Only epsilon transtitions are non-deterministic
  void epsilon_transitions(const TransFunc &func, FixedSet &states,
                           Cache &cache) {
    using CacheEntry = Cache::mapped_type;

    Cache temp;

    auto mark = [&](auto &&self, const int q) -> const CacheEntry & {
#ifndef NDEBUG
      total_recursion += 1;
#endif

      auto cit = cache.find(q);
      if (cit != cache.end())
        return cit->second;

      auto tit = temp.find(q);
      if (tit != temp.end())
        return tit->second;

      CacheEntry &current = temp.emplace(q, states.capacity()).first->second;
      for (auto p: func(q, 0)) {
        current.insert(p);

        const CacheEntry &other = self(self, p);
        current.insert(other.begin(), other.end());
      }
      return cache.emplace(q, std::move(current)).first->second;
    };

    for (auto q: states) {
      mark(mark, q);
      temp.clear();
    }

    for (auto q: states) {
      const CacheEntry &entry = cache.find(q)->second;
      states.insert(entry.begin(), entry.end());
    }
  }
} // namespace

bool Automata::accepts(const std::string_view &str) const {
  FixedSet s(size()), t(size());
  auto *curr_states = &s, *next_states = &t;

  // State cache for epsilon transitions
  Cache cache(size());

  curr_states->insert(initial());
  epsilon_transitions(func_, *curr_states, cache);

  for (const char a: str) {
    // Bookkeeping
    next_states->clear();

    // Do transitions
    deterministic_transitions(func_, *curr_states, *next_states, ctoa(a));
    epsilon_transitions(func_, *next_states, cache);

    // Short-circuit
    if (next_states->empty())
      return false;

    // Now next_states is the new current state
    std::swap(curr_states, next_states);
  }

#ifndef NDEBUG
  std::cerr << "Epsilon transitions: " << total_recursion << '\n';
#endif

  return curr_states->contains(final());
}

Automata &Automata::concat(Automata &&other) {
  join(other);

  const int my_fini = fini_, your_init = other.init_;
  fini_ = other.fini_;

  func_.connect_states(my_fini, 0, your_init);

  return *this;
}

Automata &Automata::altern(Automata &&other) {
  join(other);

  const int my_init = init_, my_fini = fini_, your_init = other.init_,
            your_fini = other.fini_;

  new_init_fini();

  func_.connect_states(init_, 0, { my_init, your_init });
  func_.connect_states(my_fini, 0, fini_);
  func_.connect_states(your_fini, 0, fini_);

  return *this;
}

Automata &Automata::kleene() {
  const int last_init = init_, last_fini = fini_;

  new_init_fini();

  auto next_states = { last_init, fini_ };
  func_.connect_states(init_, 0, next_states);
  func_.connect_states(last_fini, 0, next_states);

  return *this;
}

namespace {
  template <class T>
  using stack = std::stack<T, std::vector<T>>;

  void handle_closing_paren(stack<char> &ops, stack<Automata> &nfas) {
    if (ops.top() == '*') {
      nfas.top().kleene();
    } else {
      Automata rhs = std::move(nfas.top());
      nfas.pop();
      Automata &lhs = nfas.top();

      if (ops.top() == '.')
        lhs.concat(std::move(rhs));
      else
        lhs.altern(std::move(rhs));
    }
    ops.pop();
  }
} // namespace

Automata Automata::from_regex(const std::string_view &regex) {
  stack<char> ops;
  stack<Automata> nfas;

  for (const char a: regex) {
    switch (a) {
    case 'e':
    case '0':
    case '1':
      nfas.push(Automata(TransFunc::from_alphabet(ctoa(a)), 0, 1));
      break;
    case '*':
    case '.':
    case '+':
      ops.push(a);
      break;
    case ')':
      handle_closing_paren(ops, nfas);
      break;
    }
  }

  assert(ops.empty());
  assert(nfas.size() == 1);

  return std::move(nfas.top());
}

void Automata::new_init_fini() {
  init_ = func_.add_state();
  fini_ = func_.add_state();
}

void Automata::rebase_idxs(const int offset) {
  init_ += offset;
  fini_ += offset;
}

void Automata::join(Automata &other) {
  if (size() < other.size())
    rebase_idxs(other.size());
  else
    other.rebase_idxs(size());

  func_.join(std::move(other.func_));
}
} // namespace athw1
