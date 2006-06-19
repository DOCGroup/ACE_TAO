#ifndef guard_array_traits_hpp
#define guard_array_traits_hpp
/**
 * @file
 *
 * @brief Implement the element manipulation traits for types with
 * array-like semantics.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include <algorithm>
#include "tao/Array_VarOut_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

template <typename T_forany>
struct array_traits
{
  typedef T_forany forany_type;
  typedef typename T_forany::_array_type value_type;
  typedef typename T_forany::_array_type const const_value_type;

  typedef typename T_forany::_slice_type slice_type;

  inline static void zero_range(
      value_type * begin, value_type * end)
  {
    std::for_each(
        begin, end, &TAO::Array_Traits<forany_type>::zero);
  }

  inline static void initialize_range(
      value_type * begin, value_type * end)
  {
    std::for_each(
        begin, end, &TAO::Array_Traits<forany_type>::zero);
  }

  inline static void copy_range(
      value_type * begin, value_type * end, value_type *dst)
  {
    for(value_type * i = begin; i != end; ++i, ++dst)
    {
      TAO::Array_Traits<forany_type>::copy(*dst, *i);
    }
  }

  inline static void release_range(
      value_type * begin, value_type * end, value_type *dst)
  {
    std::for_each(
        begin, end, &TAO::Array_Traits<forany_type>::zero);
  }
};

} // namespace details
} // namespace CORBA

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_array_traits_hpp
