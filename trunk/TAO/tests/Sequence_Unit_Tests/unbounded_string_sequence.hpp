#ifndef guard_unbounded_string_sequence_hpp
#define guard_unbounded_string_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for strings.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "allocation_traits.hpp"
#include "string_traits.hpp"
#include "generic_sequence.hpp"
#include "string_sequence_element.hpp"

namespace TAO
{

class unbounded_string_sequence
{
public:
  typedef char * value_type;
  typedef char const * const_value_type;

  typedef details::string_traits<char,true> element_traits;
  typedef details::unbounded_reference_allocation_traits<char*,element_traits,true> allocation_traits;

  typedef details::string_sequence_element<element_traits> element_type;

  typedef details::generic_sequence<char*, allocation_traits, element_traits> implementation_type;

  inline unbounded_string_sequence()
    : impl_()
  {}
  inline /* explicit */ unbounded_string_sequence(CORBA::ULong maximum)
    : impl_(maximum)
  {}
  inline unbounded_string_sequence(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
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
  inline const_value_type operator[](CORBA::ULong i) const {
    return impl_[i];
  }
  inline element_type operator[](CORBA::ULong i) {
    return element_type(impl_[i], release());
  }
  inline const_value_type const * get_buffer() const {
    return impl_.get_buffer();
  }
  inline void replace(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false) {
    impl_.replace(maximum, length, data, release);
  }
  inline value_type * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  inline void swap(unbounded_string_sequence & rhs) throw() {
    impl_.swap(rhs.impl_);
  }

  static value_type * allocbuf(CORBA::ULong maximum)
  {
    return implementation_type::allocbuf(maximum);
  }
  static void freebuf(value_type * buffer)
  {
    implementation_type::freebuf(buffer);
  }


private:
  implementation_type impl_;
};


} // namespace TAO

#endif // guard_unbounded_string_sequence_hpp
