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

#include "tao/Range_Checking_T.h"

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
          << ", maximum=" << maximum;
    throw std::range_error(error.str());
  }

  inline static void check_length(
      CORBA::ULong & new_length,
      CORBA::ULong maximum)
  {
    if (maximum < new_length)
    {
      std::ostringstream error;
      error << "Invalid bounded sequence length "
            << ", length=" << new_length
            << ", maximum=" << maximum;
      throw std::runtime_error(error.str());
    }
  }
};

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
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

TAO_END_VERSIONED_NAMESPACE_DECL
#endif // guard_testing_range_checking_hpp
