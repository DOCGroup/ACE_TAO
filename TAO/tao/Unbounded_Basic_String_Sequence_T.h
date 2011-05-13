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
#include "tao/String_Const_Sequence_Element_T.h"
#include "tao/MM_Sequence_Iterator_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
template<typename charT>
class unbounded_basic_string_sequence
{
public:
  typedef charT character_type;
  typedef character_type * value_type;
  typedef character_type const * const_value_type;
  typedef ::CORBA::ULong size_type;

  typedef details::string_traits<character_type,true> element_traits;
  typedef details::unbounded_reference_allocation_traits<value_type,element_traits,true> allocation_traits;

  typedef details::string_sequence_element<element_traits> element_type;
  typedef details::string_const_sequence_element<element_traits> const_element_type;

  typedef details::generic_sequence<value_type, allocation_traits, element_traits> implementation_type;

  typedef element_type subscript_type;
  typedef const_value_type const_subscript_type;


  /// @copydoc TAO::details::generic_sequence::generic_sequence()
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

  /// @copydoc TAO::details::generic_sequence::maximum()
  inline CORBA::ULong maximum() const {
    return impl_.maximum();
  }
  /// @copydoc TAO::details::generic_sequence::release()
  inline CORBA::Boolean release() const {
    return impl_.release();
  }
  /// @copydoc TAO::details::generic_sequence::length()
  inline CORBA::ULong length() const {
    return impl_.length();
  }
  /// @copydoc TAO::details::generic_sequence::length()
  inline void length(CORBA::ULong length) {
    impl_.length(length);
  }
  /// @copydoc TAO::details::generic_sequence::operator[]
  inline const_element_type operator[](CORBA::ULong i) const {
    return const_element_type (impl_[i], release());
  }
  /// @copydoc TAO::details::generic_sequence::operator[]
  inline element_type operator[](CORBA::ULong i) {
    return element_type(impl_[i], release());
  }
  /// @copydoc TAO::details::generic_sequence::get_buffer()
  inline const_value_type const * get_buffer() const {
    return impl_.get_buffer();
  }
  /// @copydoc TAO::details::generic_sequence::replace()
  inline void replace(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false) {
    impl_.replace(maximum, length, data, release);
  }
  /// @copydoc TAO::details::generic_sequence::get_buffer(CORBA::Boolean)
  inline value_type * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  /// @copydoc TAO::details::generic_sequence::swap
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


#if defined TAO_HAS_SEQUENCE_ITERATORS && TAO_HAS_SEQUENCE_ITERATORS == 1

  ///
  /// Additions to support iterator semantics for TAO unbounded basic
  /// string sequences.
  ///

  // = Traits and factory methods that create iterators.
  typedef MM_Sequence_Iterator<unbounded_basic_string_sequence<charT> > iterator;
  typedef Const_MM_Sequence_Iterator<unbounded_basic_string_sequence<charT> > const_iterator;
  typedef MM_Sequence_Reverse_Iterator<unbounded_basic_string_sequence<charT> > reverse_iterator;
  typedef Const_MM_Sequence_Reverse_Iterator<unbounded_basic_string_sequence<charT> > const_reverse_iterator;

  /// Get an iterator that points to the beginning of the sequence.
  inline iterator begin (void)
  {
    return iterator (&this->impl_);
  }

  /// Get a const iterator that points to the beginning of the sequence.
  inline const_iterator begin (void) const
  {
    return const_iterator (&this->impl_);
  }

  /// Get an iterator that points to the end of the sequence.
  inline iterator end (void)
  {
    return iterator (&this->impl_,
                     this->impl_.length ());
  }

  /// Get a const iterator that points to the end of the sequence.
  inline const_iterator end (void) const
  {
    return const_iterator (&this->impl_,
                           this->impl_.length ());
  }

  /// Get a reverse iterator that points to the end of the sequence.
  inline reverse_iterator rbegin (void)
  {
    return reverse_iterator (&this->impl_,
                             this->impl_.length () - 1);
  }

  /// Get a const reverse iterator that points to the end of the sequence.
  inline const_reverse_iterator rbegin (void) const
  {
    return const_reverse_iterator (&this->impl_,
                                   this->impl_.length () - 1);
  }

  /// Get a reverse iterator that points to one before the beginning
  /// of the sequence.
  inline reverse_iterator rend (void)
  {
    return reverse_iterator (&this->impl_,
                             -1);
  }

  /// Get a const reverse iterator that points to one before the
  /// beginning of the sequence.
  inline const_reverse_iterator rend (void) const
  {
    return const_reverse_iterator (&this->impl_,
                                   -1);
  }

#endif /* TAO_HAS_SEQUENCE_ITERATORS==1 */

private:
  implementation_type impl_;
};

#if defined TAO_HAS_SEQUENCE_ITERATORS && TAO_HAS_SEQUENCE_ITERATORS == 1
/*
// Below is an attempt at template specialization that would
// not compile. It might be useful later.
// Generic_Sequence_Iterator template specializations for
// Unbounded_Basic_String_Sequence. These are needed since
// Unbounded_Basic_String_Sequence does some memory management with
// the strings. Without these specialization we leak memory.

/// Dereference operator returns a reference to the item contained
/// at the current position. This dereference implies string
/// memory management.
// template<typename charT>
   unbounded_basic_string_sequence<char>::element_type&
     details::Generic_Sequence_Iterator<unbounded_basic_string_sequence<char>, unbounded_basic_string_sequence::allocation_traits, unbounded_basic_string_sequence::element_traits >::operator* (void)
     {
       // Access the underlying element in the sequence.
       //return element_type (impl_[i], release());
       return element_type ((*(this->sequence_))[this->pos_],
                            this->sequence_->release());
     }

 /// Returns a const reference to the item contained at the current position
   // template<typename charT>
   unbounded_basic_string_sequence<char>::const_element_type&
   details::Generic_Sequence_Iterator<unbounded_basic_string_sequence<char> >::operator* (void) const
 {
   // Access the underlying element in the sequence.
   //return const_element_type (impl_[i], release());
   return const_element_type ((*(this->sequence_))[this->pos_],
                              this->sequence_->release ());
 }
*/
#endif /* TAO_HAS_SEQUENCE_ITERATORS==1 */

} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_unbounded_basic_string_sequence_hpp
