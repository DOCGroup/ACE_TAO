#ifndef guard_bounded_wstring_sequence_hpp
#define guard_bounded_wstring_sequence_hpp
/**
 * @file
 *
 * @brief Implement bounded sequences for wstrings.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "tao/Bounded_Basic_String_Sequence_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

template<CORBA::ULong MAX>
class bounded_wstring_sequence
  : public details::bounded_basic_string_sequence<CORBA::WChar,MAX>
{
public:
  inline bounded_wstring_sequence()
    : details::bounded_basic_string_sequence<CORBA::WChar,MAX>()
  {}
  inline bounded_wstring_sequence(
      CORBA::ULong length,
      CORBA::WChar * * data,
      CORBA::Boolean release = false)
    : details::bounded_basic_string_sequence<CORBA::WChar,MAX>(
        length, data, release)
  {}
};


} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_bounded_wstring_sequence_hpp
