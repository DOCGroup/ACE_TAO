#ifndef guard_mock_array_hpp
#define guard_mock_array_hpp
#include /**/ "ace/pre.h"
/**
 * @file
 *
 * @brief Mock an IDL-generated array
 *
 * @author Carlos O'Ryan
 */
#include "testing_counters.hpp"
#include "tao/orbconf.h"
#include "tao/Array_VarOut_T.h"

#include <algorithm>

using my_array = unsigned long[5];
using my_array_slice = unsigned long;
struct my_array_tag {};

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace TAO
{

using my_array_forany = TAO_Array_Forany_T<my_array, my_array_slice, my_array_tag>;

template<>
struct Array_Traits<my_array_forany>
{
  static my_array_slice * alloc();
  static void free(my_array_slice * _tao_slice);
  static my_array_slice * dup(my_array_slice const * _tao_source);
  static void copy(
      my_array_slice * _tao_destination,
      my_array_slice const * _tao_source);

  // TODO This is a new function
  static void zero(
      my_array_slice * _tao_slice);

};

}
TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif // guard_mock_array_hpp
