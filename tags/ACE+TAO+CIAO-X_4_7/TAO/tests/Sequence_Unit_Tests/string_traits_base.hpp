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

#include "tao/CORBA_String.h"
#include "tao/Managed_Types.h"

namespace TAO
{
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
  typedef CORBA::String_var string_var;
  typedef TAO_String_Manager string_mgr;

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
};

template<>
struct string_traits_base<CORBA::WChar>
{
  typedef CORBA::WChar char_type;
  typedef CORBA::WString_var string_var;
  typedef TAO_WString_Manager string_mgr;

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
};

} // namespace details
} // namespace CORBA

#endif // guard_string_traits_base_hpp
