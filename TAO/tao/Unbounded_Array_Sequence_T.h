#ifndef guard_unbounded_array_sequence_hpp
#define guard_unbounded_array_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for arrays.
 *
 * @author Carlos O'Ryan
 */
#include "tao/Unbounded_Array_Allocation_Traits_T.h"
#include "tao/Generic_Sequence_T.h"
#include "tao/Array_Traits_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
template<typename T_array, typename T_slice, typename T_tag>
class unbounded_array_sequence
{
public:
  using element_type = T_array *;
  using value_type = T_array;
  using T_slice_ptr = T_slice *;
  using slice_type = T_slice *;
  using const_value_type = T_slice_ptr *;
  using subscript_type = value_type &;
  using const_subscript_type = value_type const &;
  using size_type = ::CORBA::ULong;

  using allocation_traits = details::unbounded_array_allocation_traits<value_type, true>;
  using forany = TAO_Array_Forany_T<T_array, T_slice, T_tag>;
  using element_traits = details::array_traits<forany>;
  using implementation_type = details::generic_sequence<value_type, allocation_traits, element_traits>;

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
  inline void swap(unbounded_array_sequence & rhs) noexcept {
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
  template <typename stream, typename T_array, typename T_slice, typename T_tag>
  bool demarshal_sequence (stream &strm, TAO::unbounded_array_sequence<T_array, T_slice, T_tag> &target) {
    using sequence = TAO::unbounded_array_sequence<T_array, T_slice, T_tag>;
    using forany = TAO_Array_Forany_T<T_array, T_slice, T_tag>;
    using array_traits = TAO::Array_Traits<forany>;

    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length))
      {
        return false;
      }
    if (new_length > strm.length ())
      {
        return false;
      }
    sequence tmp (new_length);
    tmp.length (new_length);
    typename sequence::value_type *const buffer = tmp.get_buffer ();
    for (CORBA::ULong i = 0; i < new_length; ++i)
      {
        forany wrapper (array_traits::alloc ());
        bool const _tao_marshal_flag = strm >> wrapper;
        if (_tao_marshal_flag)
          {
            array_traits::copy (buffer[i], wrapper.in ());
          }
        array_traits::free (wrapper.inout ());
        if (!_tao_marshal_flag)
          {
            return false;
          }
      }
    tmp.swap (target);
    return true;
  }

  template <typename stream, typename T_array, typename T_slice, typename T_tag>
  bool marshal_sequence(stream & strm, const TAO::unbounded_array_sequence<T_array, T_slice, T_tag> & source) {
    using fixed_array = TAO_FixedArray_Var_T<T_array, T_slice, T_tag>;
    using forany = TAO_Array_Forany_T<T_array, T_slice, T_tag>;
    using array_traits = TAO::Array_Traits<forany>;
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    for(CORBA::ULong i = 0; i < length; ++i) {
      fixed_array tmp_array = array_traits::dup (source[i]);
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
