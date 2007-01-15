#ifndef guard_unbounded_valuetype_sequence_hpp
#define guard_unbounded_valuetype_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for value types.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "Unbounded_Valuetype_Allocation_Traits_T.h"
#include "Valuetype_Traits_T.h"
#include "tao/Generic_Sequence_T.h"
#include "Valuetype_Sequence_Element_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

template<typename object_t, typename object_t_var>
class unbounded_valuetype_sequence
{
public:
  typedef object_t object_type;
  typedef object_type * value_type;
  typedef value_type const const_value_type;

  typedef details::valuetype_traits<object_type,object_t_var,true> element_traits;
  typedef details::unbounded_reference_allocation_traits<value_type,element_traits,true> allocation_traits;

  typedef details::valuetype_sequence_element<element_traits> element_type;
  typedef element_type subscript_type;
  typedef value_type const & const_subscript_type;

  typedef details::generic_sequence<value_type, allocation_traits, element_traits> implementation_type;

  inline unbounded_valuetype_sequence()
    : impl_()
  {}
  inline explicit unbounded_valuetype_sequence(CORBA::ULong maximum)
    : impl_(maximum)
  {}
  inline unbounded_valuetype_sequence(
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
  inline value_type const & operator[](CORBA::ULong i) const {
    return impl_[i];
  }
  inline element_type operator[](CORBA::ULong i) {
    return element_type(impl_[i], release());
  }
  inline value_type const * get_buffer() const {
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
  inline void swap(unbounded_valuetype_sequence & rhs) throw() {
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

  template <typename stream, typename object_t, typename object_t_var>
  bool marshal_sequence(stream & strm, const TAO::unbounded_valuetype_sequence<object_t, object_t_var> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    for(CORBA::ULong i = 0; i < length; ++i) {
      if (!(strm << source[i])) {
        return false;
      }
    }
    return true;
  }

  template <typename stream, typename object_t, typename object_t_var>
  bool demarshal_sequence(stream & strm, TAO::unbounded_valuetype_sequence <object_t, object_t_var> & target) {
    typedef typename TAO::unbounded_valuetype_sequence <object_t, object_t_var> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    for(CORBA::ULong i = 0; i < new_length; ++i) {
      if (!(strm >> buffer[i])) {
        return false;
      }
    }
    tmp.swap(target);
    return true;
  }

} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL


#endif // guard_unbounded_valuetype_sequence_hpp
