#ifndef ATHW1_FIXEDSET_HPP_
#define ATHW1_FIXEDSET_HPP_

#include <cassert>
#include <iterator>
#include <vector>

namespace athw1 {
template <class Integer>
class iterable_fixed_set {
private:
  std::vector<int> data_;
  std::vector<Integer> idxs_;

public:
  iterable_fixed_set() = default;

  explicit iterable_fixed_set(const int capacity): data_(capacity) { }

  template <class Iterator>
  iterable_fixed_set(const int capacity, Iterator begin, Iterator end)
    : data_(capacity) {
    for (; begin != end; ++begin) {
      data_[*begin] = 1;
      idxs_.push_back(*begin);
    }
  }

  bool contains(const Integer v) const {
    assert(v >= 0 && v < capacity());
    return data_[v];
  }

  bool insert(const Integer v) {
    assert(v >= 0 && v < capacity());
    if (contains(v))
      return false;

    data_[v] = 1;
    idxs_.push_back(v);
    return true;
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
    return data_.size();
  }

  bool empty() const {
    return idxs_.empty();
  }

  void clear() {
    if (size() > capacity() >> 1)
      std::fill(data_.begin(), data_.end(), 0);
    else
      for (const auto v: idxs_)
        data_[v] = 0;
    idxs_.clear();
  }

  auto begin() const {
    return idxs_.begin();
  }

  auto end() const {
    return idxs_.end();
  }
};
} // namespace athw1

#endif /* ATHW1_FIXEDSET_HPP_ */
