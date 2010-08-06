#ifndef guard_testing_object_reference_traits_hpp
#define guard_testing_object_reference_traits_hpp
/**
 * @file
 *
 * @brief Specialize the object reference traits so they can be used
 *        in testing.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "testing_exception.hpp"
#include "testing_counters.hpp"
#include "tao/Object_Reference_Traits_T.h"

template<typename object_t, typename object_t_var>
struct testing_object_reference_traits
  : public TAO::details::object_reference_traits_base<object_t, object_t_var>
  , public TAO::details::object_reference_traits_decorator<object_t, object_t_var, testing_object_reference_traits<object_t, object_t_var> >
{
  static call_counter default_initializer_calls;
  static call_counter duplicate_calls;
  static call_counter release_calls;

  typedef object_t object_type;
  typedef object_t_var object_type_var;
  typedef TAO::details::object_reference_traits<object_t,object_t_var,false> real_traits;

  static object_type * default_initializer()
  {
    default_initializer_calls();
    return real_traits::default_initializer();
  }

  static object_type * duplicate(object_type * s)
  {
    duplicate_calls();
    return real_traits::duplicate(s);
  }

  static void release(object_type * s)
  {
    release_calls();
    return real_traits::release(s);
  }
};

template<typename object_t,typename object_t_var> call_counter
testing_object_reference_traits<object_t,object_t_var>::default_initializer_calls;

template<typename object_t,typename object_t_var> call_counter
testing_object_reference_traits<object_t,object_t_var>::duplicate_calls;

template<typename object_t,typename object_t_var> call_counter
testing_object_reference_traits<object_t,object_t_var>::release_calls;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace TAO
{
namespace details
{

template<typename object_t, typename object_t_var>
struct object_reference_traits<object_t,object_t_var,true>
  : public testing_object_reference_traits<object_t,object_t_var>
{
};

} // namespace details
} // namespace TAO
TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_testing_object_reference_traits_hpp
