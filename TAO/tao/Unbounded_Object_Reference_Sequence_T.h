#ifndef guard_unbounded_object_reference_sequence_hpp
#define guard_unbounded_object_reference_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for object references.
 *
 * @author Carlos O'Ryan
 */
#include "Unbounded_Reference_Allocation_Traits_T.h"
#include "Object_Reference_Traits_T.h"
#include "Generic_Sequence_T.h"
#include "Object_Reference_Sequence_Element_T.h"
#include "Object_Reference_Const_Sequence_Element_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
template<typename object_t, typename object_t_var>
class unbounded_object_reference_sequence
{
public:
  using object_type = object_t;
  using value_type = object_type *;
  using const_value_type = value_type const;
  using size_type = ::CORBA::ULong;

  using element_traits = details::object_reference_traits<object_type, object_t_var, true>;
  using allocation_traits = details::unbounded_reference_allocation_traits<value_type, element_traits, true>;

  using element_type = details::object_reference_sequence_element<element_traits>;
  using const_element_type = details::object_reference_const_sequence_element<element_traits>;
  using subscript_type = element_type;
  using const_subscript_type = value_type const &;

  using implementation_type = details::generic_sequence<value_type, allocation_traits, element_traits>;

  inline unbounded_object_reference_sequence()
    : impl_()
  {}
  inline explicit unbounded_object_reference_sequence(CORBA::ULong maximum)
    : impl_(maximum)
  {}
  inline unbounded_object_reference_sequence(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release)
    : impl_(maximum, length, data, release)
  {}

  // @copydoc TAO::details::generic_sequence::maximum()
  inline CORBA::ULong maximum() const {
    return impl_.maximum();
  }
  // @copydoc TAO::details::generic_sequence::release()
  inline CORBA::Boolean release() const {
    return impl_.release();
  }
  // @copydoc TAO::details::generic_sequence::length()
  inline CORBA::ULong length() const {
    return impl_.length();
  }

  // @copydoc TAO::details::generic_sequence::length()
  inline void length(CORBA::ULong length) {
    impl_.length(length);
  }
  // @copydoc TAO::details::generic_sequence::operator[]
  inline const_element_type operator[](CORBA::ULong i) const {
    return const_element_type (impl_[i], release());
  }
  // @copydoc TAO::details::generic_sequence::operator[]
  inline element_type operator[](CORBA::ULong i) {
    return element_type(impl_[i], release());
  }
  // @copydoc TAO::details::generic_sequence::get_buffer()
  inline value_type const * get_buffer() const {
    return impl_.get_buffer();
  }
  // @copydoc TAO::details::generic_sequence::replace()
  inline void replace(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false) {
    impl_.replace(maximum, length, data, release);
  }
  // @copydoc TAO::details::generic_sequence::get_buffer(CORBA::Boolean)
  inline value_type * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  // @copydoc TAO::details::generic_sequence::swap()
  inline void swap(unbounded_object_reference_sequence & rhs) noexcept {
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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_unbounded_object_reference_sequence_hpp
