#ifndef guard_vector_cdr
#define guard_vector_cdr
/**
 * @file
 *
 * @brief CDR (de)marshaling for std::vector
 *
 * $Id$
 *
 * @author Jeff Parsons
 */

#include <vector>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template<typename T>
  bool
  marshal_value_vector (
    TAO_OutputCDR &strm,
    const std::vector<T> &source)
  {
    ::CORBA::ULong const length = source.size ();

    if (! (strm << length))
      {
        return false;
      }

    for (typename std::vector<T>::const_iterator iter =
           source.begin ();
         iter != source.end ();
         ++iter)
      {
        if (! (strm << *iter))
          {
            return false;
          }
      }

    return true;
  }

  template<typename T>
  bool
  demarshal_value_vector (
    TAO_InputCDR &strm,
    std::vector<T> &target)
  {
    ::CORBA::ULong new_length = 0;

    if (! (strm >> new_length))
      {
        return false;
      }

    if (new_length > strm.length ())
      {
        return false;
      }

    std::vector<T> tmp;
    tmp.reserve (new_length);
    T tmp_elem;

    for ( ::CORBA::ULong i = 0; i < new_length; ++i)
      {
        if (! (strm >> tmp_elem))
          {
            return false;
          }

        tmp[i] = tmp_elem;
      }

    tmp.swap(target);
    return true;
  }

  template<typename T>
  bool
  marshal_objref_vector (
    TAO_OutputCDR &strm,
    const std::vector<typename T::_ptr_type> &source)
  {
    ::CORBA::ULong const length = source.size ();

    if (! (strm << length))
      {
        return false;
      }

    for (typename std::vector<typename T::_ptr_type>::const_iterator i =
           source.begin ();
         i != source.end ();
         ++i)
      {
        if (! (TAO::Objref_Traits<T>::marshal (*i, strm)))
          {
            return false;
          }
      }

    return true;
  }

  template<typename T>
  bool
  demarshal_objref_vector (
    TAO_InputCDR &strm,
    std::vector<typename T::_ptr_type> &target)
  {
    ::CORBA::ULong new_length = 0;

    if (! (strm >> new_length))
      {
        return false;
      }

    if (new_length > strm.length ())
      {
        return false;
      }

    std::vector<typename T::_ptr_type> tmp;
    tmp.reserve (new_length);
    typename T::_ptr_type tmp_elem = T::_nil ();

    for ( ::CORBA::ULong i = 0; i < new_length; ++i)
      {
        if (! (strm >> tmp_elem))
          {
            return false;
          }

        tmp[i] = tmp_elem;
      }

    tmp.swap (target);
    return true;
  }

  template<typename T_forany>
  bool
  marshal_array_vector (
    TAO_OutputCDR &strm,
    const std::vector<typename T_forany::_slice_type *> &source)
  {
    typedef TAO_FixedArray_Var_T <typename T_forany::_array_type,
                                  typename T_forany::_slice_type,
                                  typename T_forany::_tag_type> var_type;
    ::CORBA::ULong const length = source.size ();

    if (! (strm << length))
      {
        return false;
      }

    for (std::vector<typename T_forany::_slice_type *> i =
           source.begin ();
         i != source.end ();
         ++i)
      {
        var_type tmp_array =
          TAO::Array_Traits<T_forany>::dup (*i);
        T_forany const tmp (tmp_array.inout ());

        if (! (strm << tmp))
          {
            return false;
          }
      }

    return true;
  }

  template<typename T_forany>
  bool
  demarshal_array_vector (
    TAO_InputCDR &strm,
    const std::vector<typename T_forany::_slice_type *> &source)
  {
    typedef TAO::Array_Traits<T_forany> array_traits;
    ::CORBA::ULong new_length = 0;

    if (! (strm >> new_length))
      {
        return false;
      }

    if (new_length > strm.length ())
      {
        return false;
      }

    std::vector<typename T_forany::_slice_type *> tmp_vec;
    tmp_vec.reserve (new_length);

    for ( ::CORBA::ULong i = 0; i < new_length; ++i)
      {
        T_forany tmp_array (array_traits::alloc ());
        bool const _tao_marshal_flag = (strm >> tmp_array);

        if (_tao_marshal_flag)
          {
            array_traits::copy (tmp_vec[i], tmp_array.in ());
          }

        array_traits::free (tmp_array.inout ());

        if (!_tao_marshal_flag)
          {
            return false;
          }
      }

    tmp_vec.swap (source);
    return true;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_vector_cdr
