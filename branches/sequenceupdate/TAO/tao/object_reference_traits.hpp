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
#include "tao/object_reference_traits_base.hpp"

#include <algorithm>
#include <functional>

namespace TAO
{
namespace details
{

template<class object_t, class derived>
struct object_reference_traits_decorator
{
  typedef object_t object_type;
  typedef object_type * value_type;
  typedef object_type const * const_value_type;

//  typedef typename object_type::_var_type object_type_var;

  inline static void zero_range(
      object_type ** begin, object_type ** end)
  {
    std::fill(begin, end, derived::nil());
  }

  inline static void initialize_range(
      object_type ** begin, object_type ** end)
  {
    std::generate(begin, end, &derived::default_initializer);
  }

  inline static void copy_range(
      object_type ** begin, object_type ** end, object_type ** dst)
  {
    std::transform(begin, end, dst, &derived::duplicate);
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

template<typename object_t, bool dummy>
struct object_reference_traits
  : public object_reference_traits_base<object_t>
  , public object_reference_traits_decorator<object_t, object_reference_traits<object_t,dummy> >
{
  typedef object_t object_type;
  typedef object_type * value_type;
  typedef object_type const * const_value_type;
};

} // namespace details
} // namespace CORBA

#endif // guard_object_reference_traits_hpp
