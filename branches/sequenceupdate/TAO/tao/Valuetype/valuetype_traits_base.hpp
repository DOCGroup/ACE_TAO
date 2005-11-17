#ifndef valuetype_traits_base_hpp
#define valuetype_traits_base_hpp
/**
 * @file
 *
 * @brief Base class for the object reference traits.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "tao/Objref_VarOut_T.h"

namespace TAO
{
namespace details
{

template<typename object_t, typename object_t_var>
struct valuetype_traits_base
{
  typedef object_t object_type;
  typedef object_type * value_type;
  typedef object_type const * const_value_type;
  typedef object_t_var object_type_var;

  inline static void release(object_type * object)
  {
    TAO::Value_Traits<object_type>::remove_ref(object);
  }

  inline static object_type * duplicate(object_type * object)
  {
    TAO::Value_Traits<object_type>::add_ref(object);
    return object;
  }

  inline static object_type * nil()
  {
    return 0;
  }

  inline static object_type * default_initializer()
  {
    return nil();
  }
};

} // namespace details
} // namespace TAO

#endif // valuetype_traits_base_hpp
