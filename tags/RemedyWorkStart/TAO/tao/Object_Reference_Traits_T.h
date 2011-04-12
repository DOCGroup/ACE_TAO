#ifndef guard_object_reference_traits_hpp
#define guard_object_reference_traits_hpp
/**
 * @file
 *
 * @brief Implement the element manipulation traits for object
 *        reference types.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "Object_Reference_Traits_Base_T.h"
#include "ace/OS_NS_string.h"

#include <algorithm>
#include <functional>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

template<class object_t, class object_t_var, class derived>
struct object_reference_traits_decorator
{
  typedef object_t object_type;
  typedef object_type * value_type;
  typedef object_type const * const_value_type;
  typedef object_t_var object_type_var;

  inline static void zero_range(
      object_type ** begin, object_type ** end)
  {
    ACE_OS::memset (begin, 0, (end - begin) * sizeof (object_type*));
  }

  inline static void initialize_range(
      object_type ** begin, object_type ** end)
  {
    std::generate(begin, end, &derived::default_initializer);
  }

  // Allow MSVC++ >= 8 checked iterators to be used.
  template <typename iter>
  inline static void copy_range(
      object_type ** begin, object_type ** end, iter dst)
  {
    std::transform(begin, end, dst, &derived::duplicate);
  }

  // Allow MSVC++ >= 8 checked iterators to be used.
  template <typename iter>
  inline static bool copy_swap_range(
      object_type ** begin, object_type ** end, iter dst)
  {
    std::copy(begin, end, dst);
    return true;
  }

  inline static void release_range(
      object_type ** begin, object_type ** end)
  {
    std::for_each(begin, end, &derived::release);
  }

  inline static object_type const * initialize_if_zero(object_type * & element)
  {
    if (element == 0)
    {
      element = derived::nil();
    }
    return element;
  }
};

template<typename object_t, typename object_t_var, bool dummy>
struct object_reference_traits
  : public object_reference_traits_base<object_t, object_t_var>
  , public object_reference_traits_decorator<object_t, object_t_var, object_reference_traits<object_t,object_t_var,dummy> >
{
  typedef object_t object_type;
  typedef object_type * value_type;
  typedef object_type const * const_value_type;
  typedef object_t_var object_type_var;
};

} // namespace details
} // namespace CORBA

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_object_reference_traits_hpp
