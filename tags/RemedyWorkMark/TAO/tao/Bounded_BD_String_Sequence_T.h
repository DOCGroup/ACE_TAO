#ifndef guard_bounded_bd_string_sequence_hpp
#define guard_bounded_bd_string_sequence_hpp
/**
 * @file
 *
 * @brief Implement bounded sequences for bounded strings and wide-strings.
 *
 * $Id$
 *
 * @author Vladimir Zykov
 */
#include "tao/Bounded_Basic_String_Sequence_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
template<typename charT, CORBA::ULong MAX, CORBA::ULong BD_STR_MAX>
class bounded_bd_string_sequence
  : public bounded_basic_string_sequence<charT, MAX>
{
public:
  inline bounded_bd_string_sequence()
    : bounded_basic_string_sequence<charT, MAX>()
  {}

  inline bounded_bd_string_sequence(
      CORBA::ULong length,
      typename bounded_basic_string_sequence<charT, MAX>::value_type * data,
      CORBA::Boolean release)
    : bounded_basic_string_sequence<charT, MAX>(length, data, release)
  {}

  inline CORBA::ULong bd_string_maximum() const
  {
    return BD_STR_MAX;
  }
};
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_bounded_bd_string_sequence_hpp
