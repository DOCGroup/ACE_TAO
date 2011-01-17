#ifndef guard_unbounded_bd_string_sequence_hpp
#define guard_unbounded_bd_string_sequence_hpp
/**
 * @file
 *
 * @brief Implement unbounded sequences for bounded strings and wide-strings.
 *
 * $Id$
 *
 * @author Vladimir Zykov
 */
#include "tao/Unbounded_Basic_String_Sequence_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
template<typename charT, CORBA::ULong BD_STR_MAX>
class unbounded_bd_string_sequence
  : public unbounded_basic_string_sequence<charT>
{
public:
  inline unbounded_bd_string_sequence()
    : unbounded_basic_string_sequence<charT>()
  {}

  inline explicit unbounded_bd_string_sequence(CORBA::ULong maximum)
    : unbounded_basic_string_sequence<charT>(maximum)
  {}
  inline unbounded_bd_string_sequence(
      CORBA::ULong maximum,
      CORBA::ULong length,
      typename unbounded_basic_string_sequence<charT>::value_type * data,
      CORBA::Boolean release)
    : unbounded_basic_string_sequence<charT>(maximum, length, data, release)
  {}

  inline CORBA::ULong bd_string_maximum() const
  {
    return BD_STR_MAX;
  }
};
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_unbounded_bd_string_sequence_hpp
