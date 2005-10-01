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

#include <algorithm>
#include <functional>

namespace TAO
{
namespace details
{

template<typename object_t, bool dummy>
struct object_reference_traits
{
  typedef object_t object_type;
  typedef object_type * value_type;
  typedef object_type const * const_value_type;

  typedef typename object_type::_var_type object_type_var;

  inline static void release(object_type * object)
  {
    object_type::_tao_release(object);
  }

  inline static object_type * duplicate(object_type * object)
  {
    return object_type::_duplicate(object);
  }

  inline static object_type * nil()
  {
    return object_type::_nil();
  }

  inline static object_type * default_initializer()
  {
    return nil();
  }

  inline static void zero_range(
      object_type ** begin, object_type ** end)
  {
    std::fill(begin, end, object_type::_nil());
  }

  inline static void initialize_range(
      object_type ** begin, object_type ** end)
  {
    std::generate(begin, end, &nil);
  }

  inline static void copy_range(
      object_type ** begin, object_type ** end, object_type ** dst)
  {
    std::transform(begin, end, dst, &duplicate);
  }

  inline static void release_range(
      object_type ** begin, object_type ** end)
  {
    std::for_each(begin, end, &release);
  }

  inline static object_type const * initialize_if_zero(object_type * & element)
  {
    if (element == 0)
    {
      element = nil();
    }
    return element;
  }
};

} // namespace details
} // namespace CORBA

#endif // guard_object_reference_traits_hpp
