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

#include <algorithm>

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

  inline static void copy_range(
      value_type * begin, value_type * end, value_type *dst)
  {
    std::copy(begin, end, dst);
  }
};

} // namespace details
} // namespace CORBA

#endif // guard_value_traits_hpp
