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

namespace TAO
{
namespace details
{

template <typename T_array, typename T_slice, typename T_tag>
struct array_traits
{
  typedef T_array value_type;
  typedef T_array const const_value_type;

  typedef T_slice slice_type;
  typedef T_tag TAG;

  inline static void zero_range(
      value_type * begin, value_type * end)
  {
    std::for_each(
        begin, end, &TAO::Array_Traits<value_type,slice_type,TAG>::zero);
  }

  inline static void initialize_range(
      value_type * begin, value_type * end)
  {
    std::for_each(
        begin, end, &TAO::Array_Traits<value_type,slice_type,TAG>::zero);
  }

  inline static void copy_range(
      value_type * begin, value_type * end, value_type *dst)
  {
    for(value_type * i = begin; i != end; ++i, ++dst)
    {
      TAO::Array_Traits<value_type,slice_type,TAG>::copy(*dst, *i);
    }
  }

  inline static void release_range(
      value_type * begin, value_type * end, value_type *dst)
  {
    std::for_each(
        begin, end, &TAO::Array_Traits<value_type,slice_type,TAG>::zero);
  }
};

} // namespace details
} // namespace CORBA

#endif // guard_array_traits_hpp
