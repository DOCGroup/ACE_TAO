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
#include "object_reference_traits.hpp"

template<typename object_t>
struct testing_object_reference_traits
  : public TAO::details::object_reference_traits_base<object_t>
  , public TAO::details::object_reference_traits_decorator<object_t, testing_object_reference_traits<object_t> >
{
  static call_counter default_initializer_calls;
  static call_counter duplicate_calls;
  static call_counter release_calls;

  typedef object_t object_type;
  typedef typename object_type::_var_type object_type_var;
  typedef TAO::details::object_reference_traits<object_t,false> real_traits;

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

template<typename object_t> call_counter
testing_object_reference_traits<object_t>::default_initializer_calls;

template<typename object_t> call_counter
testing_object_reference_traits<object_t>::duplicate_calls;

template<typename object_t> call_counter
testing_object_reference_traits<object_t>::release_calls;

namespace TAO
{
namespace details
{

template<typename object_t>
struct object_reference_traits<object_t,true>
  : public testing_object_reference_traits<object_t>
{
};

} // namespace details
} // namespace TAO

#endif // guard_testing_object_reference_traits_hpp
