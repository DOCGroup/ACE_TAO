#ifndef guard_range_checking_hpp
#define guard_range_checking_hpp
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

/**
 * @struct
 *
 * @brief Configurable trait to check the range in operator[] access
 * to sequences.
 *
 * The sequence classes in TAO delegate on
 * range_checking<T>::check() to check the range of an operator[]
 * access.  Applications can specialize this class to detect
 * out-of-range accesses and take appropriate action (debug messages,
 * throw exceptions, etc.)
 * 
 */
template<typename T, bool dummy>
struct range_checking
{
  typedef T value_type;

  inline static void check(
      CORBA::ULong /* index */,
      CORBA::ULong /* length */,
      CORBA::ULong /* maximum */,
      char const * /* function_name */)
  {
    // Applications and tests can specialize this function to define
    // their own behavior
  }

  inline static void check_length(
      CORBA::ULong & /* new_length */,
      CORBA::ULong /* maximum */)
  {
    /*
    if (maximum < new_length)
      new_length = maximum;
    */
  }
};

} // namespace details
} // namespace TAO

#endif // guard_range_checking_hpp
