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
#include "ace/CDR_Stream.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
template<typename charT>
class String_Manager_T;

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
  typedef TAO::String_Manager_T <char_type> string_mgr;
  typedef ACE_InputCDR::to_string to_type;
  typedef ACE_OutputCDR::from_string from_type;

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
  typedef TAO::String_Manager_T <char_type> string_mgr;
  typedef ACE_InputCDR::to_wstring to_type;
  typedef ACE_OutputCDR::from_wstring from_type;

  inline static char_type * default_initializer()
  {
#if defined(ACE_HAS_WCHAR) || defined(ACE_HAS_XPG4_MULTIBYTE_CHAR)
    return CORBA::wstring_dup(L"");
#else
//#warning "platform not configured with native wchar_t support"
//    static CORBA::WChar empty[] = { 0 };
    CORBA::WChar empty[] = { 0 };
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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_string_traits_base_hpp
