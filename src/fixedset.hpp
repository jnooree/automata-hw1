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
    const int size_;

  public:
    constexpr bool operator==(const fixed_set_const_iter &rhs) const {
      return p_ == rhs.p_;
    }

    constexpr bool operator!=(const fixed_set_const_iter &rhs) const {
      return !(*this == rhs);
    }

    constexpr fixed_set_const_iter &operator++() {
      if (p_ >= 0) {
        ++p_;
        if (p_ >= size_)
          p_ = -1;
      }

      return *this;
    }

    Integer operator*() const {
      return (*idxs_)[p_];
    }

  private:
    fixed_set_const_iter(const std::vector<Integer> &idxs, const int p)
      : idxs_(&idxs), p_(p), size_(idxs.size()) { }

    static fixed_set_const_iter end(const std::vector<Integer> &idxs) {
      return { idxs, -1 };
    }

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

  explicit fixed_set(const int size): data_(size) { }

  bool contains(const Integer v) const {
    assert(v >= 0 && v < data_.size());
    return data_[v] != 0;
  }

  void insert(const Integer v) {
    assert(v >= 0 && v < data_.size());
    if (data_[v] != 0)
      return;

    data_[v] = 1;
    idxs_.push_back(v);
  }

  void clear() {
    if (empty())
      return;

    std::fill(data_.begin(), data_.end(), 0);
    idxs_.clear();
  }

  bool empty() const {
    return idxs_.empty();
  }

  const_iterator end() const {
    return const_iterator::end(idxs_);
  }

  const_iterator begin() const {
    if (empty())
      return end();
    return const_iterator(idxs_, 0);
  }
};
} // namespace athw1

#endif /* ATHW1_FIXEDSET_HPP_ */
