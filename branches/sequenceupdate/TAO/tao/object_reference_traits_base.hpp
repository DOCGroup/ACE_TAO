#ifndef object_reference_traits_base_hpp
#define object_reference_traits_base_hpp
/**
 * @file
 *
 * @brief Base class for the object reference traits.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

namespace TAO
{
namespace details
{

template<typename object_t>
struct object_reference_traits_base
{
  typedef object_t object_type;
  typedef object_type * value_type;
  typedef object_type const * const_value_type;

//  typedef typename object_type::_var_type object_type_var;

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
};

} // namespace details
} // namespace TAO

#endif // object_reference_traits_base_hpp
