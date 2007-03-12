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

# ifndef ACE_LACKS_MEMBER_TEMPLATES
  // Allow MSVC++ >= 8 checked iterators to be used.
  template <typename iter>
  inline static void copy_range(
      value_type * begin, value_type * end, iter dst)
  {
    std::copy(begin, end, dst);
  }
# else
  inline static void copy_range(
      value_type * begin, value_type * end, value_type * dst)
  {
    std::copy(begin, end, dst);
  }
# endif  /* !ACE_LACKS_MEMBER_TEMPLATES */
};

} // namespace details
} // namespace CORBA

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_value_traits_hpp
