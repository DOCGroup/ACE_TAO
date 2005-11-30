#ifndef guard_string_traits_base_hpp
#define guard_string_traits_base_hpp
/**
 * @file
 *
 * @brief Isolate the string_traits from the accidental differences
 * between wstring and string.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "tao/String_Alloc.h"

namespace TAO
{
template<typename charT>
class String_Manager;

template<typename charT>
class String_var;

template<typename charT>
class String_out;

namespace details
{

template<typename charT>
struct string_traits_base
{
};

template<>
struct string_traits_base<char>
{
  typedef char char_type;
  typedef TAO::String_var <char_type> string_var;
  typedef TAO::String_out <char_type> string_out;
  typedef TAO::String_Manager <char_type> string_mgr;

  inline static char_type * default_initializer()
  {
    return CORBA::string_dup("");
  }

  inline static char_type * duplicate(char_type const * s)
  {
    return CORBA::string_dup(s);
  }

  inline static void release(char_type * s)
  {
    CORBA::string_free(s);
  }

  inline static char_type * allocate (CORBA::ULong len)
  {
    return CORBA::string_alloc (len);
  }
};

template<>
struct string_traits_base<CORBA::WChar>
{
  typedef CORBA::WChar char_type;
  typedef TAO::String_var <char_type> string_var;
  typedef TAO::String_out <char_type> string_out;
  typedef TAO::String_Manager <char_type> string_mgr;

  inline static char_type * default_initializer()
  {
#if defined(ACE_HAS_WCHAR) || defined(ACE_HAS_XPG4_MULTIBYTE_CHAR)
    return CORBA::wstring_dup(L"");
#else
#warning "platform not configured with native wchar_t support"
    static CORBA::WChar empty[] = { 0 };
    return CORBA::wstring_dup(empty);
#endif /* 0 */
  }

  inline static char_type * duplicate(char_type const * s)
  {
    return CORBA::wstring_dup(s);
  }

  inline static void release(char_type * s)
  {
    CORBA::wstring_free(s);
  }

  inline static char_type * allocate (CORBA::ULong len)
  {
    return CORBA::wstring_alloc (len);
  }
};

} // namespace details
} // namespace TAO

#endif // guard_string_traits_base_hpp
