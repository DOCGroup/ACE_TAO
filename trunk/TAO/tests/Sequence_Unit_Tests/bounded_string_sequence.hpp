#ifndef guard_bounded_string_sequence_hpp
#define guard_bounded_string_sequence_hpp
/**
 * @file
 *
 * @brief Implement bounded sequences for strings.
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

template<CORBA::ULong MAX>
class bounded_string_sequence
{
public:
  typedef char * value_type;
  typedef char const * const_value_type;

  typedef details::string_traits<char,true> element_traits;
  typedef details::bounded_reference_allocation_traits<value_type,element_traits,MAX,true> allocation_traits;

  typedef details::string_sequence_element<element_traits> element_type;

  typedef details::generic_sequence<char*, allocation_traits, element_traits> implementation_type;

  inline bounded_string_sequence()
    : impl_()
  {}
  inline bounded_string_sequence(
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
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false) {
    impl_.replace(MAX, length, data, release);
  }
  inline value_type * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  inline void swap(bounded_string_sequence & rhs) throw() {
    impl_.swap(rhs.impl_);
  }

  static value_type * allocbuf(CORBA::ULong maximum)
  {
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
