#ifndef guard_testing_range_checking_hpp
#define guard_testing_range_checking_hpp
/**
 * @file
 *
 * @brief Specialize the range_checking traits in a manner suitable
 * for testing.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "range_checking.hpp"

#include <sstream>
#include <stdexcept>

template<typename T>
struct testing_range_checking
{
  typedef T value_type;

  inline static void check(
      CORBA::ULong index,
      CORBA::ULong length,
      CORBA::ULong maximum,
      char const * function_name)
  {
    if (index < length) {
      return;
    }
    std::ostringstream error;
    error << "Out of range access in " << function_name
          << ", index=" << index
          << ", length=" << length
          << ", maximum=" << maximum
          << ", class=unbounded_sequence<int>";
    throw std::range_error(error.str());
  }
};

namespace TAO
{
namespace details
{

template<typename T>
struct range_checking<T,true>
  : public testing_range_checking<T>
{
};

} // namespace details
} // namespace TAO

#endif // guard_testing_range_checking_hpp
