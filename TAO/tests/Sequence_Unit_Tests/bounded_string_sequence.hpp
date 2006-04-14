#ifndef guard_bounded_string_sequence_hpp
#define guard_bounded_string_sequence_hpp
/**
 * @file
 *
 * @brief Implement bounded sequences for strings.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "bounded_basic_string_sequence.hpp"

namespace TAO
{

template<CORBA::ULong MAX>
class bounded_string_sequence
  : public details::bounded_basic_string_sequence<char,MAX>
{
public:
  inline bounded_string_sequence()
    : details::bounded_basic_string_sequence<char,MAX>()
  {}
  inline bounded_string_sequence(
      CORBA::ULong length,
      char * * data,
      CORBA::Boolean release = false)
    : details::bounded_basic_string_sequence<char,MAX>(
        length, data, release)
  {}
};


} // namespace TAO

#endif // guard_bounded_string_sequence_hpp
