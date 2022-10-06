#ifndef ATHW1_FIXEDSET_HPP_
#define ATHW1_FIXEDSET_HPP_

#include <cassert>
#include <vector>

namespace athw1 {
template <class Integer>
class iterable_fixed_set;

// Thin wrapper around std::vector<int> providing a fixed-size set
class fixed_set {
private:
  std::vector<int> data_;

public:
  fixed_set() = default;

  explicit fixed_set(const int capacity): data_(capacity) { }

  template <class Iterator>
  fixed_set(const int capacity, Iterator begin, Iterator end): data_(capacity) {
    for (; begin != end; ++begin)
      data_[*begin] = 1;
  }

  bool contains(const int v) const {
    assert(v >= 0 && v < capacity());
    return static_cast<bool>(data_[v]);
  }

  void insert(const int v) {
    assert(v >= 0 && v < capacity());
    data_[v] = 1;
  }

  template <class Iterator>
  void insert(Iterator begin, Iterator end) {
    for (; begin != end; ++begin)
      insert(*begin);
  }

  int capacity() const {
    return data_.size();
  }

  void clear() {
    std::fill(data_.begin(), data_.end(), 0);
  }

  template <class Integer>
  friend class iterable_fixed_set;
};

namespace detail {
  template <class Integer>
  class iterable_fixed_set_const_iter {
  private:
    const std::vector<Integer> *idxs_;
    int p_;

  public:
    constexpr bool operator==(const iterable_fixed_set_const_iter &rhs) const {
      return p_ == rhs.p_;
    }

    constexpr bool operator!=(const iterable_fixed_set_const_iter &rhs) const {
      return !(*this == rhs);
    }

    constexpr iterable_fixed_set_const_iter &operator++() {
      ++p_;
      return *this;
    }

    constexpr int operator-(const iterable_fixed_set_const_iter &rhs) {
      return p_ - rhs.p_;
    }

    Integer operator*() const {
      return (*idxs_)[p_];
    }

  private:
    iterable_fixed_set_const_iter(const std::vector<Integer> &idxs, const int p)
      : idxs_(&idxs), p_(p) { }

    friend class iterable_fixed_set<Integer>;
  };
} // namespace detail

template <class Integer>
class iterable_fixed_set {
private:
  fixed_set set_;
  std::vector<Integer> idxs_;

public:
  using const_iterator = detail::iterable_fixed_set_const_iter<Integer>;

  iterable_fixed_set() = default;

  explicit iterable_fixed_set(const int capacity): set_(capacity) { }

  template <class Iterator>
  iterable_fixed_set(const int capacity, Iterator begin, Iterator end)
    : set_(capacity) {
    for (; begin != end; ++begin) {
      set_.insert(*begin);
      idxs_.push_back(*begin);
    }
  }

  bool contains(const Integer v) const {
    assert(v >= 0 && v < capacity());
    return set_.contains(v);
  }

  void insert(const Integer v) {
    assert(v >= 0 && v < capacity());
    if (contains(v))
      return;

    set_.insert(v);
    idxs_.push_back(v);
  }

  void insert(const iterable_fixed_set &other) {
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
    return set_.capacity();
  }

  bool empty() const {
    return idxs_.empty();
  }

  void clear() {
    if (size() > capacity() >> 1) {
      set_.clear();
      idxs_.clear();
    } else {
      for (const auto v: idxs_)
        set_.data_[v] = 0;
      idxs_.clear();
    }
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
