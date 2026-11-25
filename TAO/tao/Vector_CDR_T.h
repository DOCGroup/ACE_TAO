#ifndef guard_vector_cdr
#define guard_vector_cdr
/**
 * @file
 *
 * @brief CDR (de)marshaling for std::vector
 *
 * @author Jeff Parsons
 */

#include <memory>
#include <utility>
#include <vector>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template<typename T>
  bool
  marshal_value_vector (
    TAO_OutputCDR &strm,
    std::vector<T> const &source)
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
    ::CORBA::ULong new_length {};

    if (! (strm >> new_length))
      {
        return false;
      }

    if (new_length > strm.length ())
      {
        return false;
      }

    std::vector<T> tmp;
    tmp.resize (new_length);
    T tmp_elem;

    for ( ::CORBA::ULong i {}; i < new_length; ++i)
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

  template<typename T>
  bool
  marshal_objref_vector (
    TAO_OutputCDR &strm,
    std::vector<typename T::_ptr_type> const &source)
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
    ::CORBA::ULong new_length {};

    if (! (strm >> new_length))
      {
        return false;
      }

    if (new_length > strm.length ())
      {
        return false;
      }

    std::vector<typename T::_ptr_type> tmp;
    tmp.resize (new_length, T::_nil ());
    std::vector<typename T::_var_type> owners (new_length);

    for ( ::CORBA::ULong i {}; i < new_length; ++i)
      {
        if (! (strm >> owners[i].out ()))
          {
            return false;
          }

        tmp[i] = owners[i].in ();
      }

    tmp.swap (target);
    for (typename T::_var_type &owner : owners)
      {
        owner._retn ();
      }
    return true;
  }

  template<typename T_forany>
  bool
  marshal_array_vector (
    TAO_OutputCDR &strm,
    std::vector<typename T_forany::_slice_type *> const &source)
  {
    using var_type = TAO_FixedArray_Var_T<typename T_forany::_array_type,
                                          typename T_forany::_slice_type,
                                          typename T_forany::_tag_type>;
    using vector_type = std::vector<typename T_forany::_slice_type *>;
    ::CORBA::ULong const length = source.size ();

    if (! (strm << length))
      {
        return false;
      }

    for (typename vector_type::const_iterator i =
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
    std::vector<typename T_forany::_slice_type *> &target)
  {
    using array_traits = TAO::Array_Traits<T_forany>;
    using slice_type = typename T_forany::_slice_type;
    using slice_owner = std::unique_ptr<slice_type, decltype (&array_traits::free)>;
    ::CORBA::ULong new_length {};

    if (! (strm >> new_length))
      {
        return false;
      }

    if (new_length > strm.length ())
      {
        return false;
      }

    std::vector<slice_owner> tmp_vec;
    tmp_vec.reserve (new_length);

    for ( ::CORBA::ULong i {}; i < new_length; ++i)
      {
        slice_owner tmp_array_data (array_traits::alloc (), &array_traits::free);
        if (!tmp_array_data)
          {
            return false;
          }

        T_forany tmp_array (tmp_array_data.get ());

        if (! (strm >> tmp_array))
          {
            return false;
          }

        tmp_vec.push_back (std::move (tmp_array_data));
      }

    std::vector<slice_type *> tmp_target;
    tmp_target.reserve (tmp_vec.size ());
    for (slice_owner &array : tmp_vec)
      {
        tmp_target.push_back (array.get ());
      }

    tmp_target.swap (target);
    for (slice_owner &array : tmp_vec)
      {
        array.release ();
      }

    return true;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_vector_cdr
