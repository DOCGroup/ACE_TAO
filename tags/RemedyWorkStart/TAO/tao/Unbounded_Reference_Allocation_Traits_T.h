#ifndef guard_unbounded_reference_allocation_traits_hpp
#define guard_unbounded_reference_allocation_traits_hpp
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

template<typename T, class ref_traits, bool dummy>
struct unbounded_reference_allocation_traits
{
  typedef T value_type;
  typedef ref_traits reference_traits;

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
    value_type * buffer = new value_type[maximum + 1];
    reinterpret_cast<value_type**>(buffer)[0] = buffer + maximum + 1;

    // no throw
    reference_traits::initialize_range(buffer + 1, buffer + maximum + 1);

    return buffer + 1;
  }

  inline static value_type * allocbuf_noinit(CORBA::ULong maximum)
  {
    value_type * buffer = new value_type[maximum + 1];
    reinterpret_cast<value_type**>(buffer)[0] = buffer + maximum + 1;

    // no throw
    reference_traits::zero_range(buffer + 1, buffer + maximum + 1);

    return buffer + 1;
  }

  inline static void freebuf(value_type * buffer)
  {
    if(buffer != 0)
    {
      value_type * begin = buffer - 1;
      value_type * end = reinterpret_cast<value_type*>(*begin);
      reference_traits::release_range(buffer, end);

      buffer = begin;
    }
    delete[] buffer;
  }
};

} // namespace details
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_unbounded_reference_allocation_traits_hpp
