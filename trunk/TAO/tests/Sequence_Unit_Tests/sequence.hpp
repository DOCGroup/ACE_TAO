#ifndef guard_TAO_sequence_hpp
#define guard_TAO_sequence_hpp
/**
 * @file
 *
 * @brief Implement both bounded and unbounded sequences of
 * self-managed types.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "generic_sequence.hpp"

namespace TAO
{

template<class T>
class unbounded_sequence
{
public:
  typedef T value_type;
  typedef details::unbounded_sequence_traits<T> traits;
  typedef details::generic_sequence<T, traits> implementation_type;

  inline unbounded_sequence()
    : impl_()
  {}
  inline /* explicit */ unbounded_sequence(CORBA::ULong maximum)
    : impl_(maximum)
  {}
  inline unbounded_sequence(
      CORBA::ULong maximum,
      CORBA::ULong length,
      T * data,
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
  inline T const & operator[](CORBA::ULong i) const {
    return impl_[i];
  }
  inline value_type & operator[](CORBA::ULong i) {
    return impl_[i];
  }
  inline void replace(
      CORBA::ULong maximum,
      CORBA::ULong length,
      T * data,
      CORBA::Boolean release = false) {
    impl_.replace(maximum, length, data, release);
  }
  inline T const * get_buffer() const {
    return impl_.get_buffer();
  }
  inline T * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  inline void swap(unbounded_sequence & rhs) throw() {
    impl_.swap(rhs.impl_);
  }
  static T * allocbuf(CORBA::ULong maximum) {
    return implementation_type::allocbuf(maximum);
  }
  static void freebuf(T * buffer)
  {
    implementation_type::freebuf(buffer);
  }

private:
  implementation_type impl_;
};

template<class T, CORBA::ULong MAX>
class bounded_sequence
{
public:
  // static const CORBA::ULong MAXIMUM = MAX;
  typedef T value_type;
  typedef details::bounded_sequence_traits<T,MAX> traits;
  typedef details::generic_sequence<T, traits> implementation_type;

  inline bounded_sequence()
    : impl_()
  {}
  inline bounded_sequence(
      CORBA::ULong length,
      T * data,
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
    impl_.length(length);
  }
  inline T const & operator[](CORBA::ULong i) const {
    return impl_[i];
  }
  inline value_type & operator[](CORBA::ULong i) {
    return impl_[i];
  }
  inline void replace(
      CORBA::ULong length,
      T * data,
      CORBA::Boolean release = false) {
    impl_.replace(MAX, length, data, release);
  }
  inline T const * get_buffer() const {
    return impl_.get_buffer();
  }
  inline T * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  inline void swap(bounded_sequence & rhs) throw() {
    impl_.swap(rhs.impl_);
  }
  static T * allocbuf(CORBA::ULong maximum) {
    return implementation_type::allocbuf(maximum);
  }
  static T * allocbuf() {
    return implementation_type::allocbuf(MAX);
  }
  static void freebuf(T * buffer)
  {
    implementation_type::freebuf(buffer);
  }

private:
  implementation_type impl_;
};

} // namespace TAO

#endif // guard_TAO_sequence_hpp
