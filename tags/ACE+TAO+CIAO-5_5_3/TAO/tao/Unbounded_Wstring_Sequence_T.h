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
#include "tao/Unbounded_Basic_String_Sequence_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_unbounded_wstring_sequence_hpp
