#ifndef guard_unbounded_value_allocation_traits_hpp
#define guard_unbounded_value_allocation_traits_hpp
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
struct unbounded_value_allocation_traits
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

} // namespace details
} // namespace TAO

#endif // guard_unbounded_value_allocation_traits_hpp
