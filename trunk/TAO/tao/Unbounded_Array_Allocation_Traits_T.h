#ifndef guard_unbounded_array_allocation_traits_hpp
#define guard_unbounded_array_allocation_traits_hpp
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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

template<typename T, bool dummy>
struct unbounded_array_allocation_traits
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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_unbounded_array_allocation_traits_hpp
