#ifndef guard_mock_array_hpp
#define guard_mock_array_hpp
#include /**/ "ace/pre.h"
/**
 * @file
 *
 * @brief Mock an IDL-generated array
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "testing_counters.hpp"

#include <algorithm>

typedef unsigned long my_array[5];
typedef unsigned long my_array_slice;
struct my_array_tag {};

namespace TAO
{

template<typename T, typename T_slice, typename TAG>
struct Array_Traits;

template<>
struct Array_Traits<my_array,my_array_slice,my_array_tag>
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

#include /**/ "ace/post.h"
#endif // guard_mock_array_hpp
