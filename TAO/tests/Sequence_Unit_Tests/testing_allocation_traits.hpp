#ifndef guard_testing_sequence_traits_hpp
#define guard_testing_sequence_traits_hpp
/**
 * @file
 *
 * @brief Specialize the allocation traits in a manner suitable for
 * testing.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "allocation_traits.hpp"
#include "testing_exception.hpp"

template<typename T, class base>
struct testing_allocation_traits : public base
{
  static long allocbuf_calls;
  static long freebuf_calls;
  static long calls_until_failure_in_allocbuf;

  static long default_buffer_allocation_calls;
  static long calls_until_failure_in_default_buffer_allocation;

  typedef typename base::value_type value_type;

  inline static value_type * default_buffer_allocation()
  {
    ++default_buffer_allocation_calls;
    if (--calls_until_failure_in_default_buffer_allocation == 0)
    {
      throw testing_exception();
    }
    return base::default_buffer_allocation();
  }

  inline static value_type * allocbuf(CORBA::ULong maximum)
  {
    ++allocbuf_calls;
    if (--calls_until_failure_in_allocbuf == 0)
    {
      throw testing_exception();
    }
    return base::allocbuf(maximum);
  }

  inline static void freebuf(value_type * buffer)
  {
    ++freebuf_calls;
    base::freebuf(buffer);
  }
};

template<typename T, class base>
long testing_allocation_traits<T,base>::allocbuf_calls = 0;

template<typename T, class base>
long testing_allocation_traits<T,base>::freebuf_calls = 0;

template<typename T, class base>
long testing_allocation_traits<T,base>::calls_until_failure_in_allocbuf = 0;

template<typename T, class base>
long testing_allocation_traits<T,base>::default_buffer_allocation_calls = 0;

template<typename T, class base>
long testing_allocation_traits<T,base>::calls_until_failure_in_default_buffer_allocation = 0;

namespace TAO
{
namespace details
{

template<typename T>
struct unbounded_allocation_traits<T,true>
  : public testing_allocation_traits<T, unbounded_allocation_traits<T,false> >
{
};

template<typename T, CORBA::ULong MAX>
struct bounded_allocation_traits<T,MAX,true>
  : public testing_allocation_traits<T, bounded_allocation_traits<T,MAX,false> >
{
};

} // namespace details
} // namespace TAO

#endif // guard_testing_sequence_traits_hpp
