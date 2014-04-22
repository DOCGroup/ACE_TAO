#ifndef valuetype_traits_base_hpp
#define valuetype_traits_base_hpp
/**
 * @file
 *
 * @brief Base class for the valuetype traits.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "tao/Valuetype/Value_VarOut_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // valuetype_traits_base_hpp
