#ifndef guard_value_traits_hpp
#define guard_value_traits_hpp
/**
 * @file
 *
 * @brief Implement the element manipulation traits for types with
 * value-like semantics.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include /**/ "tao/Versioned_Namespace.h"

#include <algorithm>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

template<typename T, bool dummy>
struct value_traits
{
  typedef T value_type;
  typedef T const const_value_type;

  inline static void zero_range(
      value_type * begin , value_type * end)
  {
    std::fill(begin, end, value_type ());
  }

  inline static void initialize_range(
      value_type * begin, value_type * end)
  {
    std::fill(begin, end, value_type ());
  }

  inline static void release_range(
      value_type *, value_type *)
  {
    // Noop for value sequences
  }

  // Allow MSVC++ >= 8 checked iterators to be used.
  template <typename iter>
  inline static void copy_range(
      value_type * begin, value_type * end, iter dst)
  {
    std::copy(begin, end, dst);
  }

  // Allow MSVC++ >= 8 checked iterators to be used.
  template <typename iter>
  inline static void copy_swap_range(
      value_type * begin, value_type * end, iter dst)
  {
    copy_range(begin, end, dst);
  }
};

} // namespace details
} // namespace CORBA

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_value_traits_hpp
