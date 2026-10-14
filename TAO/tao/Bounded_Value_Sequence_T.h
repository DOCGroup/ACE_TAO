#ifndef guard_bounded_value_sequence_hpp
#define guard_bounded_value_sequence_hpp
/**
 * @file
 *
 * @brief Implement bounded sequences for types with value-like
 * semantics.
 *
 * @author Carlos O'Ryan
 */

#include "tao/Bounded_Value_Allocation_Traits_T.h"
#include "tao/Value_Traits_T.h"
#include "tao/Generic_Sequence_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
template<class T, CORBA::ULong MAX, class Tag=int>
class bounded_value_sequence
{
public:
  using value_type = T;
  using element_type = T;
  using const_value_type = T const;
  using subscript_type = value_type &;
  using const_subscript_type = value_type const &;
  using size_type = ::CORBA::ULong;

  using allocation_traits = details::bounded_value_allocation_traits<value_type, MAX, true>;
  using element_traits = details::value_traits<value_type, true>;
  using implementation_type = details::generic_sequence<value_type, allocation_traits, element_traits>;

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
    implementation_type::range::check_length(length, MAX);
    impl_.length(length);
  }
  // @copydoc TAO::details::generic_sequence::operator[]
  inline value_type const & operator[](CORBA::ULong i) const {
    return impl_[i];
  }
  // @copydoc TAO::details::generic_sequence::operator[]
  inline value_type & operator[](CORBA::ULong i) {
    return impl_[i];
  }
  // @copydoc TAO::details::generic_sequence::replace()
  inline void replace(
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false) {
    impl_.replace(MAX, length, data, release);
  }
  // @copydoc TAO::details::generic_sequence::get_buffer()
  inline value_type const * get_buffer() const {
    return impl_.get_buffer();
  }
  // @copydoc TAO::details::generic_sequence::get_buffer(CORBA::Boolean)
  inline value_type * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  // @copydoc TAO::details::generic_sequence::swap()
  inline void swap(bounded_value_sequence & rhs) noexcept {
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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_bounded_string_sequence_hpp
