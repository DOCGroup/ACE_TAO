#ifndef guard_TAO_sequence_traits_hpp
#define guard_TAO_sequence_traits_hpp
/**
 * @file
 *
 * @brief Please see the documentation for generic_sequence.cpp for
 * details.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "tao/corbafwd.h"

namespace TAO
{
namespace details
{

template<class T>
struct sequence_traits
{
  typedef T value_type;

  inline static void check_index(
      CORBA::ULong /* index */,
      CORBA::ULong /* length */,
      CORBA::ULong /* maximum */,
      char const * /* function_name */)
  {
    // Applications and tests can specialize this function to define
    // their own behavior
  }
};

template<class T>
struct unbounded_sequence_traits : public sequence_traits<T>
{
  typedef typename sequence_traits<T>::value_type value_type;

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

template<class T, CORBA::ULong MAX>
struct bounded_sequence_traits : public sequence_traits<T>
{
  typedef typename sequence_traits<T>::value_type value_type;

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
} // namespace CORBA

#endif // guard_TAO_sequence_traits_hpp
