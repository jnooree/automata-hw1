#ifndef ATHW1_FIXEDSET_HPP_
#define ATHW1_FIXEDSET_HPP_

#include <cassert>
#include <vector>

namespace athw1 {
template <class Integer>
class fixed_set;

namespace detail {
  template <class Integer>
  class fixed_set_const_iter {
  private:
    const std::vector<Integer> *idxs_;
    int p_;

  public:
    constexpr bool operator==(const fixed_set_const_iter &rhs) const {
      return p_ == rhs.p_;
    }

    constexpr bool operator!=(const fixed_set_const_iter &rhs) const {
      return !(*this == rhs);
    }

    constexpr fixed_set_const_iter &operator++() {
      ++p_;
      return *this;
    }

    constexpr int operator-(const fixed_set_const_iter &rhs) {
      return p_ - rhs.p_;
    }

    Integer operator*() const {
      return (*idxs_)[p_];
    }

  private:
    fixed_set_const_iter(const std::vector<Integer> &idxs, const int p)
      : idxs_(&idxs), p_(p) { }

    friend class fixed_set<Integer>;
  };
} // namespace detail

// Why not use std::unordered_set? because it's *TERRIBLY* slower.
template <class Integer>
class fixed_set {
private:
  std::vector<int> data_;
  std::vector<Integer> idxs_;

public:
  using const_iterator = detail::fixed_set_const_iter<Integer>;

  fixed_set() = default;

  explicit fixed_set(const int capacity): data_(capacity) { }

  template <class Iterator>
  fixed_set(const int capacity, Iterator begin, Iterator end): data_(capacity) {
    for (; begin != end; ++begin) {
      data_[*begin] = 1;
      idxs_.push_back(*begin);
    }
  }

  bool contains(const Integer v) const {
    assert(v >= 0 && v < capacity());
    return data_[v] != 0;
  }

  void insert(const Integer v) {
    assert(v >= 0 && v < capacity());
    if (contains(v))
      return;

    data_[v] = 1;
    idxs_.push_back(v);
  }

  void insert(const fixed_set &other) {
    insert(other.begin(), other.end());
  }

  template <class Iterator>
  void insert(Iterator begin, Iterator end) {
    for (; begin != end; ++begin)
      insert(*begin);
  }

  int size() const {
    return idxs_.size();
  }

  int capacity() const {
    return data_.size();
  }

  bool empty() const {
    return idxs_.empty();
  }

  void clear() {
    for (auto v: idxs_)
      data_[v] = 0;
    idxs_.clear();
  }

  const_iterator begin() const {
    return const_iterator(idxs_, 0);
  }

  const_iterator end() const {
    return const_iterator(idxs_, idxs_.size());
  }
};
} // namespace athw1

#endif /* ATHW1_FIXEDSET_HPP_ */
