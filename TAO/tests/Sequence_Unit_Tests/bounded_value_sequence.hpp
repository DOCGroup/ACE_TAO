#ifndef TAO_unbounded_value_sequence_hpp
#define TAO_unbounded_value_sequence_hpp
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

#include "allocation_traits.hpp"
#include "value_traits.hpp"
#include "generic_sequence.hpp"

namespace TAO
{

template<class T, CORBA::ULong MAX>
class bounded_value_sequence
{
public:
  // static CORBA::ULong const MAXIMUM = MAX;
  typedef T value_type;
  typedef details::bounded_allocation_traits<T,MAX,true> allocation_traits;
  typedef details::value_traits<T,true> element_traits;
  typedef details::generic_sequence<T, allocation_traits, element_traits> implementation_type;

  inline bounded_value_sequence()
    : impl_()
  {}
  inline bounded_value_sequence(
      CORBA::ULong length,
      T * data,
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
    impl_.length(length);
  }
  inline T const & operator[](CORBA::ULong i) const {
    return impl_[i];
  }
  inline value_type & operator[](CORBA::ULong i) {
    return impl_[i];
  }
  inline void replace(
      CORBA::ULong length,
      T * data,
      CORBA::Boolean release = false) {
    impl_.replace(MAX, length, data, release);
  }
  inline T const * get_buffer() const {
    return impl_.get_buffer();
  }
  inline T * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  inline void swap(bounded_value_sequence & rhs) throw() {
    impl_.swap(rhs.impl_);
  }
  static T * allocbuf(CORBA::ULong maximum) {
    return implementation_type::allocbuf(maximum);
  }
  static T * allocbuf() {
    return implementation_type::allocbuf(MAX);
  }
  static void freebuf(T * buffer)
  {
    implementation_type::freebuf(buffer);
  }

private:
  implementation_type impl_;
};

} // namespace TAO

#endif // TAO_unbounded_string_sequence_hpp
