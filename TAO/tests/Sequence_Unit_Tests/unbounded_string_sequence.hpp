#ifndef TAO_unbounded_string_sequence_hpp
#define TAO_unbounded_string_sequence_hpp
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

namespace TAO
{

class unbounded_string_sequence
{
public:
  typedef char * value_type;
  typedef details::unbounded_allocation_traits<char*,true> allocation_traits;
  typedef details::string_traits<char,true> element_traits;

  typedef details::generic_sequence<char*, allocation_traits, element_traits> implementation_type;

  inline unbounded_string_sequence()
    : impl_()
  {}
  inline /* explicit */ unbounded_string_sequence(CORBA::ULong maximum)
    : impl_(maximum)
  {}
#if 0
  inline unbounded_string_sequence(
      CORBA::ULong maximum,
      CORBA::ULong length,
      char * * data,
      CORBA::Boolean release = false)
    : impl_(maximum, length, data, release)
  {}
#endif /* 0 */
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
  inline char const * operator[](CORBA::ULong i) const {
    return impl_[i];
  }
#if 0
  inline value_type & operator[](CORBA::ULong i) {
    return impl_[i];
  }
  inline void replace(
      CORBA::ULong maximum,
      CORBA::ULong length,
      char * * data,
      CORBA::Boolean release = false) {
    impl_.replace(maximum, length, data, release);
  }
  inline char const * const * get_buffer() const {
    return impl_.get_buffer();
  }
  inline char * * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  inline void swap(unbounded_string_sequence & rhs) throw() {
    impl_.swap(rhs.impl_);
  }
  static char * * allocbuf(CORBA::ULong maximum) {
    return implementation_type::allocbuf(maximum);
  }
  static void freebuf(char * * buffer)
  {
    implementation_type::freebuf(buffer);
  }

#endif /* 0 */

private:
  implementation_type impl_;
};


} // namespace TAO

#endif // TAO_unbounded_string_sequence_hpp
