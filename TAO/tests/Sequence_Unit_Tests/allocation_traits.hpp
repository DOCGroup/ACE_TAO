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

template<typename value_type, class reference_traits, bool dummy>
struct unbounded_reference_allocation_traits
  : public unbounded_allocation_traits<value_type,dummy>
{
  typedef unbounded_allocation_traits<value_type,dummy> base_allocation_traits;

  inline static value_type * allocbuf(CORBA::ULong maximum)
  {
    value_type * buffer =
      base_allocation_traits::allocbuf(maximum + 1);
    reinterpret_cast<value_type*>(buffer[0]) = buffer + maximum + 1;

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
    base_allocation_traits::freebuf(buffer);
  }
};

template<typename value_type, class reference_traits, CORBA::ULong MAX, bool dummy>
struct bounded_reference_allocation_traits
  : public bounded_allocation_traits<value_type,MAX,dummy>
{
  typedef bounded_allocation_traits<value_type,MAX,dummy> base_allocation_traits;

  inline static value_type * allocbuf(CORBA::ULong /* maximum */)
  {
    value_type * buffer = base_allocation_traits::allocbuf(MAX);
    reference_traits::zero_range(buffer, buffer + MAX);
    return buffer;
  }

  inline static void freebuf(value_type * buffer)
  {
    reference_traits::release_range(buffer, buffer + MAX);
    base_allocation_traits::freebuf(buffer);
  }
};

} // namespace details
} // namespace TAO

#endif // guard_allocation_traits_hpp
