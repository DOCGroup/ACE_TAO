#ifndef guard_allocation_traits_hpp
#define guard_allocation_traits_hpp
/**
 * @file
 *
 * @brief Details can be found in the documentation for
 * TAO::details::generic_sequence 
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "tao/Basic_Types.h"

namespace TAO
{
namespace details
{

template<typename T, bool dummy>
struct unbounded_allocation_traits
{
  typedef T value_type;

  inline static CORBA::ULong default_maximum()
  {
    return 0;
  }

  inline static value_type * default_buffer_allocation()
  {
    return 0;
  }

  inline static value_type * allocbuf(CORBA::ULong maximum)
  {
    return new value_type[maximum];
  }

  inline static void freebuf(value_type * buffer)
  {
    delete[] buffer;
  }
};

template<typename T, CORBA::ULong MAX, bool dummy>
struct bounded_allocation_traits
{
  typedef T value_type;

  inline static CORBA::ULong default_maximum()
  {
    return MAX;
  }

  inline static value_type * default_buffer_allocation()
  {
    return allocbuf(MAX);
  }

  inline static value_type * allocbuf(CORBA::ULong /* maximum */)
  {
    return new value_type[MAX];
  }

  inline static void freebuf(value_type * buffer)
  {
    delete[] buffer;
  }

  inline static CORBA::ULong maximum()
  {
    return MAX;
  }
  /* static CORBA::ULong const MAXIMUM = MAX; */
};

} // namespace details
} // namespace TAO

#endif // guard_allocation_traits_hpp
