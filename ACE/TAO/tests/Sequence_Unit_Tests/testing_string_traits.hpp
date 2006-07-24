#ifndef guard_testing_string_traits_hpp
#define guard_testing_string_traits_hpp
/**
 * @file
 *
 * @brief Specialize the string traits so they can be used in testing.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "testing_exception.hpp"
#include "tao/String_Traits_T.h"
#include "testing_counters.hpp"

template<typename charT>
struct testing_string_traits
  : public TAO::details::string_traits_base<charT>
  , public TAO::details::string_traits_decorator<charT,testing_string_traits<charT> >
{
  static call_counter default_initializer_calls;
  static call_counter duplicate_calls;
  static call_counter release_calls;
  static call_counter not_released_from_const_calls;
  static call_counter not_released_from_managed_calls;

  typedef charT char_type;
  typedef TAO::details::string_traits<charT,false> real_traits;

  static char_type * default_initializer()
  {
    default_initializer_calls();
    return real_traits::default_initializer();
  }

  static char_type * duplicate(char_type const * s)
  {
    duplicate_calls();
    return real_traits::duplicate(s);
  }

  static void release(char_type * s)
  {
    release_calls();
    return real_traits::release(s);
  }

  static void not_released_from_const(
      char_type * & dst, char_type const * src)
  {
    not_released_from_const_calls();
    real_traits::not_released_from_const(dst, src);
  }

  static void not_released_from_managed(
      char_type * & dst, char_type const * src)
  {
    not_released_from_managed_calls();
    real_traits::not_released_from_managed(dst, src);
  }
};

template<typename charT> call_counter
testing_string_traits<charT>::default_initializer_calls;

template<typename charT> call_counter
testing_string_traits<charT>::duplicate_calls;

template<typename charT> call_counter
testing_string_traits<charT>::release_calls;

template<typename charT> call_counter
testing_string_traits<charT>::not_released_from_const_calls;

template<typename charT> call_counter
testing_string_traits<charT>::not_released_from_managed_calls;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace TAO
{
namespace details
{

template<>
struct string_traits<char,true>
  : public testing_string_traits<char>
{
};

template<>
struct string_traits<CORBA::WChar,true>
  : public testing_string_traits<CORBA::WChar>
{
};

} // namespace details
} // namespace TAO
TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_testing_string_traits_hpp
