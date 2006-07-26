#ifndef guard_unbounded_basic_string_sequence_hpp
#define guard_unbounded_basic_string_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for strings and wide-strings.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Unbounded_Reference_Allocation_Traits_T.h"
#include "tao/String_Traits_T.h"
#include "tao/Generic_Sequence_T.h"
#include "tao/String_Sequence_Element_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

template<typename charT>
class unbounded_basic_string_sequence
{
public:
  typedef charT character_type;
  typedef character_type * value_type;
  typedef character_type const * const_value_type;

  typedef details::string_traits<character_type,true> element_traits;
  typedef details::unbounded_reference_allocation_traits<value_type,element_traits,true> allocation_traits;

  typedef details::string_sequence_element<element_traits> element_type;

  typedef details::generic_sequence<value_type, allocation_traits, element_traits> implementation_type;

  typedef element_type subscript_type;
  typedef const_value_type const_subscript_type;

  inline unbounded_basic_string_sequence()
    : impl_()
  {}
  inline explicit unbounded_basic_string_sequence(CORBA::ULong maximum)
    : impl_(maximum)
  {}
  inline unbounded_basic_string_sequence(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release)
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
  inline void swap(unbounded_basic_string_sequence & rhs) throw() {
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

} // namespace details
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_unbounded_basic_string_sequence_hpp
