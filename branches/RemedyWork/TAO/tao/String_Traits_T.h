#ifndef guard_string_traits_hpp
#define guard_string_traits_hpp
/**
 * @file
 *
 * @brief Implement the element manipulation traits for string types.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "String_Traits_Base_T.h"
#include "ace/OS_NS_string.h"

#include <algorithm>
#include <functional>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

template<typename char_type, class derived>
struct string_traits_decorator
{
  typedef char_type * value_type;
  typedef char_type const * const_value_type;

  inline static void zero_range(
      char_type ** begin, char_type ** end)
  {
    ACE_OS::memset (begin, 0, (end - begin) * sizeof (char_type*));
  }

  inline static void initialize_range(
      char_type ** begin, char_type ** end)
  {
    std::generate(begin, end, &derived::default_initializer);
  }

  // Allow MSVC++ >= 8 checked iterators to be used.
  template <typename iter>
  inline static void copy_range(
      char_type ** begin, char_type ** end, iter dst)
  {
    std::transform(begin, end, dst, &derived::duplicate);
  }

  // Allow MSVC++ >= 8 checked iterators to be used.
  template <typename iter>
  inline static bool copy_swap_range(
      char_type ** begin, char_type ** end, iter dst)
  {
    std::swap_ranges(begin, end, dst);
    return true;
  }

  inline static void release_range(
      char_type ** begin, char_type ** end)
  {
    std::for_each(begin, end, &derived::release);
  }

  inline static char_type const * initialize_if_zero(char_type * & element)
  {
    if (element == 0)
    {
      element = derived::default_initializer();
    }
    return element;
  }
};

template<class charT, bool dummy>
struct string_traits
  : public string_traits_base<charT>
  , public string_traits_decorator<charT,string_traits<charT,dummy> >
{
};

} // namespace details
} // namespace CORBA

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_string_traits_hpp
