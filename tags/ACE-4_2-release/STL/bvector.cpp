#define STL_BVECTOR
#define SA_STD std
#include <bvector.h>

namespace std
{

Allocator<unsigned int> bit_vector::static_allocator;

inline bool operator==(const bit_vector& x, const bit_vector& y) {
    return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}

inline bool operator<(const bit_vector& x, const bit_vector& y) {
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

void swap(bit_vector::reference x, bit_vector::reference y) {
    bool tmp = x;
    x = y;
    y = tmp;
}

void bit_vector::insert_aux(iterator position, bool x) {
    if (finish.p != end_of_storage) {
  copy_backward(position, finish - 1, finish);
  *position = x;
  ++finish;
    } else {
  size_type len = size() ? 2 * size() : __WORD_BIT;
  unsigned int* q = bit_alloc(len);
  iterator i = copy(begin(), position, iterator(q, 0));
  *i++ = x;
  finish = copy(position, end(), i);
  static_allocator.deallocate(start.p);
  end_of_storage = q + (len + __WORD_BIT - 1)/__WORD_BIT;
  start = iterator(q, 0);
    }
}

void bit_vector::insert(iterator position, size_type n, bool x) {
    if (n == 0) return;
    if (capacity() - size() >= n) {
  copy_backward(position, end(), finish + n);
  fill(position, position + n, x);
  finish += n;
    } else {
  size_type len = size() + max(size(), n);
  unsigned int* q = bit_alloc(len);
  iterator i = copy(begin(), position, iterator(q, 0));
  fill_n(i, n, x);
  finish = copy(position, end(), i + n);
  static_allocator.deallocate(start.p);
  end_of_storage = q + (n + __WORD_BIT - 1)/__WORD_BIT;
  start = iterator(q, 0);
    }
}

void bit_vector::insert(iterator position, const_iterator first, 
      const_iterator last) {
    if (first == last) return;
    size_type n = 0;
    distance(first, last, n);
    if (capacity() - size() >= n) {
  copy_backward(position, end(), finish + n);
  copy(first, last, position);
  finish += n;
    } else {
  size_type len = size() + max(size(), n);
  unsigned int* q = bit_alloc(len);
  iterator i = copy(begin(), position, iterator(q, 0));
  i = copy(first, last, i);
  finish = copy(position, end(), i);
  static_allocator.deallocate(start.p);
  end_of_storage = q + (len + __WORD_BIT - 1)/__WORD_BIT;
  start = iterator(q, 0);
    }
}

}