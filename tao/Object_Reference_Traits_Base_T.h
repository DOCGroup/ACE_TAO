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

#include "tao/Objref_VarOut_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

template<typename object_t, typename object_t_var>
struct object_reference_traits_base
{
  typedef object_t object_type;
  typedef object_type * value_type;
  typedef object_type const * const_value_type;
  typedef object_t_var object_type_var;

  inline static void release(object_type * object)
  {
    TAO::Objref_Traits<object_type>::release(object);
  }

  inline static object_type * duplicate(object_type * object)
  {
    return TAO::Objref_Traits<object_type>::duplicate(object);
  }

  inline static object_type * nil()
  {
    return TAO::Objref_Traits<object_type>::nil();
  }

  inline static object_type * default_initializer()
  {
    return nil();
  }
};

} // namespace details
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // object_reference_traits_base_hpp
