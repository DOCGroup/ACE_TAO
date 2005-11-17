#ifndef guard_unbounded_wstring_sequence_hpp
#define guard_unbounded_wstring_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for wide strings.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "unbounded_basic_string_sequence.hpp"

namespace TAO
{

class unbounded_wstring_sequence
  : public details::unbounded_basic_string_sequence<CORBA::WChar>
{
public:
  inline unbounded_wstring_sequence()
    : details::unbounded_basic_string_sequence<CORBA::WChar>()
  {}
  inline /* explicit */ unbounded_wstring_sequence(CORBA::ULong maximum)
    : details::unbounded_basic_string_sequence<CORBA::WChar>(maximum)
  {}
  inline unbounded_wstring_sequence(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false)
    : details::unbounded_basic_string_sequence<CORBA::WChar>(
        maximum, length, data, release)
  {}
};

} // namespace TAO

#endif // guard_unbounded_wstring_sequence_hpp
