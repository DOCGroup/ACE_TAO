#ifndef guard_unbounded_array_sequence_hpp
#define guard_unbounded_array_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for arrays.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "unbounded_array_allocation_traits.hpp"
#include "generic_sequence.hpp"

namespace TAO
{

template<typename array_traits>
class unbounded_array_sequence
{
public:
  typedef typename array_traits::value_type underlying_type;
  typedef typename array_traits::value_type * value_type;
  typedef typename array_traits::value_type const * const_value_type;

  typedef details::unbounded_array_allocation_traits<value_type,true> allocation_traits;
  typedef array_traits element_traits;
  typedef details::generic_sequence<value_type, allocation_traits, element_traits> implementation_type;

  inline unbounded_array_sequence()
    : impl_()
  {}
  inline explicit unbounded_array_sequence(CORBA::ULong maximum)
    : impl_(maximum)
  {}
  inline unbounded_array_sequence(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false)
    : impl_(maximum, length, data, release)
  {}
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
    impl_.length(length);
  }
  inline value_type const & operator[](CORBA::ULong i) const {
    return impl_[i];
  }
  inline value_type & operator[](CORBA::ULong i) {
    return impl_[i];
  }
  inline void replace(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false) {
    impl_.replace(maximum, length, data, release);
  }
  inline value_type const * get_buffer() const {
    return impl_.get_buffer();
  }
  inline value_type * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  inline void swap(unbounded_array_sequence & rhs) throw() {
    impl_.swap(rhs.impl_);
  }
  static value_type * allocbuf(CORBA::ULong maximum) {
    return implementation_type::allocbuf(maximum);
  }
  static void freebuf(value_type * buffer) {
    implementation_type::freebuf(buffer);
  }

private:
  implementation_type impl_;
};

} // namespace TAO


#endif // guard_unbounded_array_sequence_hpp
