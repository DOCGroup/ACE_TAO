#ifndef valuetype_traits_base_hpp
#define valuetype_traits_base_hpp
/**
 * @file
 *
 * @brief Base class for the valuetype traits.
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
  using object_type = object_t;
  using value_type = object_type *;
  using const_value_type = object_type const *;
  using object_type_var = object_t_var;

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
    return nullptr;
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
