#ifndef guard_unbounded_value_sequence_hpp
#define guard_unbounded_value_sequence_hpp
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

#include "tao/Unbounded_Value_Allocation_Traits_T.h"
#include "tao/Value_Traits_T.h"
#include "tao/Generic_Sequence_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

template<class T>
class unbounded_value_sequence
{
public:
  typedef T value_type;
  typedef T element_type;
  typedef T const const_value_type;
  typedef value_type & subscript_type;
  typedef value_type const & const_subscript_type;
  typedef ::CORBA::ULong size_type;

  typedef details::unbounded_value_allocation_traits<value_type,true> allocation_traits;
  typedef details::value_traits<value_type,true> element_traits;
  typedef details::generic_sequence<value_type, allocation_traits, element_traits> implementation_type;

  inline unbounded_value_sequence()
    : impl_()
  {}
  inline explicit unbounded_value_sequence(CORBA::ULong maximum)
    : impl_(maximum)
  {}
  inline unbounded_value_sequence(
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
  inline value_type const & operator[](CORBA::ULong i) const {
    return impl_[i];
  }
  inline value_type & operator[](CORBA::ULong i) {
    return impl_[i];
  }
  inline void replace(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false) {
    impl_.replace(maximum, length, data, release);
  }
  inline value_type const * get_buffer() const {
    return impl_.get_buffer();
  }
  inline value_type * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  inline void swap(unbounded_value_sequence & rhs) throw() {
    impl_.swap(rhs.impl_);
  }
  static value_type * allocbuf(CORBA::ULong maximum) {
    return implementation_type::allocbuf(maximum);
  }
  static void freebuf(value_type * buffer) {
    implementation_type::freebuf(buffer);
  }

#if defined TAO_HAS_SEQUENCE_ITERATORS && TAO_HAS_SEQUENCE_ITERATORS == 1

  ///
  /// Additions to support iterator semantics for TAO unbounded value
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

#endif /* TAO_HAS_SEQUENCE_ITERATORS==1 */

private:
  implementation_type impl_;
};

} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_unbounded_string_sequence_hpp

