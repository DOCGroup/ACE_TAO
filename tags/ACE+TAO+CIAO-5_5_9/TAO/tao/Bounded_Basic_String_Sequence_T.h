#ifndef guard_bounded_basic_string_sequence_hpp
#define guard_bounded_basic_string_sequence_hpp
/**
 * @file
 *
 * @brief Implement bounded sequences for strings and wide-strings.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Bounded_Reference_Allocation_Traits_T.h"
#include "tao/String_Traits_T.h"
#include "tao/Generic_Sequence_T.h"
#include "tao/String_Sequence_Element_T.h"
#include "tao/String_Const_Sequence_Element_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
template<typename charT, CORBA::ULong MAX>
class bounded_basic_string_sequence
{
public:
  typedef charT character_type;
  typedef charT * value_type;
  typedef charT const * const_value_type;

  typedef details::string_traits<charT,true> element_traits;
  typedef details::bounded_reference_allocation_traits<value_type,element_traits,MAX,true> allocation_traits;

  typedef details::string_sequence_element<element_traits> element_type;
  typedef details::string_const_sequence_element<element_traits> const_element_type;
  typedef element_type subscript_type;
  typedef const_value_type const_subscript_type;

  typedef details::generic_sequence<value_type, allocation_traits, element_traits> implementation_type;

  /// @copydoc details::generic_sequence::generic_sequence
  inline bounded_basic_string_sequence()
    : impl_()
  {}
  /// @copydoc details::generic_sequence::generic_sequence
  inline bounded_basic_string_sequence(
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release)
    : impl_(MAX, length, data, release)
  {}
  /// @copydoc details::generic_sequence::maximum
  inline CORBA::ULong maximum() const {
    return impl_.maximum();
  }
  /// @copydoc details::generic_sequence::release
  inline CORBA::Boolean release() const {
    return impl_.release();
  }
  /// @copydoc details::generic_sequence::length
  inline CORBA::ULong length() const {
    return impl_.length();
  }

  /// @copydoc details::generic_sequence::length
  inline void length(CORBA::ULong length) {
    implementation_type::range::check_length(length, MAX);
    impl_.length(length);
  }
  /// @copydoc details::generic_sequence::operator[]
  inline const_element_type operator[](CORBA::ULong i) const {
    return const_element_type (impl_[i], release());
  }
  /// @copydoc details::generic_sequence::operator[]
  inline element_type operator[](CORBA::ULong i) {
    return element_type(impl_[i], release());
  }
  /// @copydoc details::generic_sequence::get_buffer
  inline const_value_type const * get_buffer() const {
    return impl_.get_buffer();
  }
  /// @copydoc details::generic_sequence::replace
  inline void replace(
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false) {
    impl_.replace(MAX, length, data, release);
  }
  /// @copydoc TAO::details::generic_sequence::get_buffer(CORBA::Boolean)
  inline value_type * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  inline void swap(bounded_basic_string_sequence & rhs) throw() {
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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_bounded_basic_string_sequence_hpp
