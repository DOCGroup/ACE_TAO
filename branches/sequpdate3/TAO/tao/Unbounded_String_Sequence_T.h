#ifndef guard_unbounded_string_sequence_hpp
#define guard_unbounded_string_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for strings.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Unbounded_Basic_String_Sequence_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

class unbounded_string_sequence
  : public details::unbounded_basic_string_sequence<char>
{
public:
  inline unbounded_string_sequence()
    : details::unbounded_basic_string_sequence<char>()
  {}
  inline explicit unbounded_string_sequence(CORBA::ULong maximum)
    : details::unbounded_basic_string_sequence<char>(maximum)
  {}
  inline unbounded_string_sequence(
      CORBA::ULong maximum,
      CORBA::ULong length,
      value_type * data,
      CORBA::Boolean release = false)
    : details::unbounded_basic_string_sequence<char>(
        maximum, length, data, release)
  {}
};

} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_unbounded_string_sequence_hpp
