#ifndef guard_unbounded_array_sequence_hpp
#define guard_unbounded_array_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for arrays.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Unbounded_Array_Allocation_Traits_T.h"
#include "tao/Generic_Sequence_T.h"
#include "tao/Array_Traits_T.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

template<typename array_traits>
class unbounded_array_sequence
{
public:
  typedef typename array_traits::value_type value_type;
  typedef typename array_traits::slice_type slice_type;
  typedef slice_type * T_slice_ptr;
  typedef value_type * element_type;
  typedef T_slice_ptr * const_value_type;
  typedef value_type & subscript_type;
  typedef value_type const & const_subscript_type;

  typedef details::unbounded_array_allocation_traits<value_type,true> allocation_traits;
  typedef details::array_traits <array_traits> element_traits;
  typedef details::generic_sequence<value_type, allocation_traits, element_traits> implementation_type;

  inline unbounded_array_sequence()
    : impl_()
  {}
  inline explicit unbounded_array_sequence(CORBA::ULong maximum)
    : impl_(maximum)
  {}
  inline unbounded_array_sequence(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false)
    : impl_(maximum, length, data, release)
  {}
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
  inline void swap(unbounded_array_sequence & rhs) throw() {
    impl_.swap(rhs.impl_);
  }
  static value_type * allocbuf(CORBA::ULong maximum) {
    return implementation_type::allocbuf(maximum);
  }
  static void freebuf(value_type * buffer) {
    implementation_type::freebuf(buffer);
  }

private:
  implementation_type impl_;
};

  template <typename stream, typename array_traits>
  bool demarshal_sequence(stream & strm, TAO::unbounded_array_sequence<array_traits> & target) {
    typedef TAO::unbounded_array_sequence<array_traits> sequence;
    typedef TAO_Array_Forany_T <array_traits> forany;

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
      forany tmp (TAO::details::array_traits<array_traits>::alloc ());
      bool const _tao_marshal_flag = (strm >> tmp);
      if (_tao_marshal_flag) {
        TAO::details::array_traits<array_traits>::copy (buffer[i], tmp.in ());
      }
      TAO::details::array_traits<array_traits>::free (tmp.inout ());
      if (!_tao_marshal_flag) {
        return false;
      }
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, typename array_traits>
  bool marshal_sequence(stream & strm, const TAO::unbounded_array_sequence<array_traits> & source) {
    if (0 == &source)
      ACE_THROW_RETURN (::CORBA::BAD_PARAM(0, CORBA::COMPLETED_MAYBE), false);
    typedef typename ::TAO_FixedArray_Var_T <array_traits> fixed_array;
    typedef typename ::TAO_Array_Forany_T <array_traits> forany;
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    for(CORBA::ULong i = 0; i < length; ++i) {
      fixed_array tmp_array = TAO::details::array_traits<array_traits>::dup (source[i]);
      forany const tmp (tmp_array.inout ());
      if (!(strm << tmp)) {
        return false;
      }
    }
    return true;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_unbounded_array_sequence_hpp
