#ifndef TAO_unbounded_value_sequence_hpp
#define TAO_unbounded_value_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for types with value-like
 * semantics.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "allocation_traits.hpp"
#include "value_traits.hpp"
#include "generic_sequence.hpp"

namespace TAO
{

template<class T>
class unbounded_value_sequence
{
public:
  typedef T value_type;
  typedef details::unbounded_allocation_traits<T,true> allocation_traits;
  typedef details::value_traits<T,true> element_traits;
  typedef details::generic_sequence<T, allocation_traits, element_traits> implementation_type;

  inline unbounded_value_sequence()
    : impl_()
  {}
  inline /* explicit */ unbounded_value_sequence(CORBA::ULong maximum)
    : impl_(maximum)
  {}
  inline unbounded_value_sequence(
      CORBA::ULong maximum,
      CORBA::ULong length,
      T * data,
      CORBA::Boolean release = false)
    : impl_(maximum, length, data, release)
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
    impl_.length(length);
  }
  inline T const & operator[](CORBA::ULong i) const {
    return impl_[i];
  }
  inline value_type & operator[](CORBA::ULong i) {
    return impl_[i];
  }
  inline void replace(
      CORBA::ULong maximum,
      CORBA::ULong length,
      T * data,
      CORBA::Boolean release = false) {
    impl_.replace(maximum, length, data, release);
  }
  inline T const * get_buffer() const {
    return impl_.get_buffer();
  }
  inline T * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  inline void swap(unbounded_value_sequence & rhs) throw() {
    impl_.swap(rhs.impl_);
  }
  static T * allocbuf(CORBA::ULong maximum) {
    return implementation_type::allocbuf(maximum);
  }
  static void freebuf(T * buffer) {
    implementation_type::freebuf(buffer);
  }

private:
  implementation_type impl_;
};

} // namespace TAO

#endif // TAO_unbounded_string_sequence_hpp

