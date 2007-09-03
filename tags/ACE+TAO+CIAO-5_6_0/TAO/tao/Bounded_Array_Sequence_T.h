#ifndef guard_bounded_array_sequence_hpp
#define guard_bounded_array_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for arrays.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Bounded_Array_Allocation_Traits_T.h"
#include "tao/Generic_Sequence_T.h"
#include "tao/Array_Traits_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

template<typename T_array, typename T_slice, typename T_tag, CORBA::ULong MAX>
class bounded_array_sequence
{
public:
  typedef T_array * element_type;
  typedef T_array value_type;
  typedef T_array * T_slice_ptr;
  typedef T_slice_ptr * const_value_type;
  typedef value_type & subscript_type;
  typedef value_type const & const_subscript_type;

  typedef details::bounded_array_allocation_traits<value_type,MAX,true> allocation_traits;
  typedef TAO_Array_Forany_T<T_array, T_slice, T_tag> forany;
  typedef details::array_traits <forany> element_traits;
  typedef details::generic_sequence<value_type, allocation_traits, element_traits> implementation_type;

  inline bounded_array_sequence()
    : impl_()
  {}
  inline bounded_array_sequence(
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false)
    : impl_(MAX, length, data, release)
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
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false) {
    impl_.replace(MAX, length, data, release);
  }
  inline value_type const * get_buffer() const {
    return impl_.get_buffer();
  }
  inline value_type * get_buffer(CORBA::Boolean orphan = false) {
    return impl_.get_buffer(orphan);
  }
  inline void swap(bounded_array_sequence & rhs) throw() {
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
}

namespace TAO
{
  template <typename stream, typename T_array, typename T_slice, typename T_tag, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_array_sequence<T_array, T_slice, T_tag, MAX> & target) {
    typedef typename TAO::bounded_array_sequence<T_array, T_slice, T_tag, MAX> sequence;
    typedef TAO_Array_Forany_T<T_array, T_slice, T_tag> forany;
    typedef TAO::Array_Traits<forany> array_traits;

    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    for(CORBA::ULong i = 0; i < new_length; ++i) {
      forany tmp (array_traits::alloc ());
      bool const _tao_marshal_flag = (strm >> tmp);
      if (_tao_marshal_flag) {
        array_traits::copy (buffer[i], tmp.in ());
      }
      array_traits::free (tmp.inout ());
      if (!_tao_marshal_flag) {
        return false;
      }
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, typename T_array, typename T_slice, typename T_tag, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_array_sequence<T_array, T_slice, T_tag, MAX> & source) {
    typedef TAO_Array_Forany_T<T_array, T_slice, T_tag> forany;
    typedef TAO_FixedArray_Var_T <T_array, T_slice, T_tag> fixed_array;
    typedef TAO::Array_Traits<forany> array_traits;
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    for(CORBA::ULong i = 0; i < length; ++i) {
      fixed_array tmp_array = array_traits::dup (source[i]);
      forany tmp (tmp_array.inout ());
      if (!(strm << tmp)) {
        return false;
      }
    }
    return true;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_bounded_array_sequence_hpp
