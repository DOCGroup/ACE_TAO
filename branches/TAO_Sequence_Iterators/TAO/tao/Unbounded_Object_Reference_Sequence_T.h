#ifndef guard_unbounded_object_reference_sequence_hpp
#define guard_unbounded_object_reference_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for object references.
 *
 * $Id$
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
  typedef object_t object_type;
  typedef object_type * value_type;
  typedef value_type const const_value_type;

  typedef details::object_reference_traits<object_type,object_t_var,true> element_traits;
  typedef details::unbounded_reference_allocation_traits<value_type,element_traits,true> allocation_traits;

  typedef details::object_reference_sequence_element<element_traits> element_type;
  typedef details::object_reference_const_sequence_element<element_traits> const_element_type;
  typedef element_type subscript_type;
  typedef value_type const & const_subscript_type;

  typedef details::generic_sequence<value_type, allocation_traits, element_traits> implementation_type;

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
  inline value_type const * get_buffer() const {
    return impl_.get_buffer();
  }
  /// @copydoc details::generic_sequence::replace
  inline void replace(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false) {
    impl_.replace(maximum, length, data, release);
  }
  /// @copydoc details::generic_sequence::get_buffer(CORBA::Boolean)
  inline value_type * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  /// @copydoc details::generic_sequence::swap
  inline void swap(unbounded_object_reference_sequence & rhs) throw() {
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

#if TAO_HAS_SEQUENCE_ITERATORS

  ///
  /// Additions to support iterator semantics for TAO unbounded object reference
  /// sequences.
  ///

  // = Traits and factory methods that create iterators.
  typedef details::Generic_Sequence_Iterator<details::generic_sequence<value_type, allocation_traits, element_traits> > iterator;
  typedef details::Const_Generic_Sequence_Iterator<details::generic_sequence<value_type, allocation_traits, element_traits> > const_iterator;
  typedef details::Generic_Sequence_Reverse_Iterator<details::generic_sequence<value_type, allocation_traits, element_traits> > reverse_iterator;
  typedef details::Const_Generic_Sequence_Reverse_Iterator<details::generic_sequence<value_type, allocation_traits, element_traits> > const_reverse_iterator;

  // Get an iterator that points to the beginning of the sequence.
  iterator begin (void)
  {
    return impl_.begin ();
  }

  // Get a const iterator that points to the beginning of the sequence.
  const_iterator begin (void) const
  {
    return impl_.begin ();
  }

  // Get an iterator that points to the end of the sequence.
  iterator end (void)
  {
    return impl_.end ();
  }

  // Get a const iterator that points to the end of the sequence.
  const_iterator end (void) const
  {
    return impl_.end ();
  }

  // Get a reverse iterator that points to the end of the sequence.
  reverse_iterator rbegin (void)
  {
    return impl_.rbegin ();
  }

  // Get a const reverse iterator that points to the end of the sequence.
  const_reverse_iterator rbegin (void) const
  {
    return impl_.rbegin ();
  }

  // Get a reverse iterator that points to one before the beginning
  // of the sequence.
  reverse_iterator rend (void)
  {
    return impl_.rend ();
  }

  // Get a const reverse iterator that points to one before the
  // beginning of the sequence.
  const_reverse_iterator rend (void) const
  {
    return impl_.rend ();
  }

#endif /* TAO_HAS_SEQUENCE_ITERATORS */

private:
  implementation_type impl_;
};

} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_unbounded_object_reference_sequence_hpp
