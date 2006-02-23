#ifndef guard_bounded_value_sequence_hpp
#define guard_bounded_value_sequence_hpp
/**
 * @file
 *
 * @brief Implement bounded sequences for types with value-like
 * semantics.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "tao/Bounded_Value_Allocation_Traits_T.h"
#include "tao/Value_Traits_T.h"
#include "tao/Generic_Sequence_T.h"

namespace TAO
{

template<class T, CORBA::ULong MAX>
class bounded_value_sequence
{
public:
  typedef T value_type;
  typedef T element_type;
  typedef T const const_value_type;
  typedef value_type & subscript_type;
  typedef value_type const & const_subscript_type;

  typedef details::bounded_value_allocation_traits<value_type,MAX,true> allocation_traits;
  typedef details::value_traits<value_type,true> element_traits;
  typedef details::generic_sequence<value_type, allocation_traits, element_traits> implementation_type;

  inline bounded_value_sequence()
    : impl_()
  {}
  inline bounded_value_sequence(
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false)
    : impl_(MAX, length, data, release)
  {}
  /* Use default ctor, operator= and dtor */
  inline CORBA::ULong maximum() const {
    return impl_.maximum();
  }
  inline CORBA::Boolean release() const {
    return impl_.release();
  }
  inline CORBA::ULong length() const {
    return impl_.length();
  }
  inline void length(CORBA::ULong length) {
    implementation_type::range::check_length(length, MAX);
    impl_.length(length);
  }
  inline value_type const & operator[](CORBA::ULong i) const {
    return impl_[i];
  }
  inline value_type & operator[](CORBA::ULong i) {
    return impl_[i];
  }
  inline void replace(
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false) {
    impl_.replace(MAX, length, data, release);
  }
  inline value_type const * get_buffer() const {
    return impl_.get_buffer();
  }
  inline value_type * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  inline void swap(bounded_value_sequence & rhs) throw() {
    impl_.swap(rhs.impl_);
  }
  static value_type * allocbuf(CORBA::ULong maximum) {
    return implementation_type::allocbuf(maximum);
  }
  static value_type * allocbuf() {
    return implementation_type::allocbuf(MAX);
  }
  static void freebuf(value_type * buffer)
  {
    implementation_type::freebuf(buffer);
  }

private:
  implementation_type impl_;
};

} // namespace TAO

#endif // guard_bounded_string_sequence_hpp
