// $Id$

#ifndef ACE_ADAPTER_OS_STRING_H
#define ACE_ADAPTER_OS_STRING_H
#include "ace/pre.h"

#include "../../ace_wchar.h"
#include "../../OS_String.h"

#if defined (ACE_HAS_WCHAR)

#if defined (ACE_USES_WCHAR)

class ACE_OS_String_C : public ACE_OS_String
{
public:
  static int ace_isspace (const wchar_t s);
  static int ace_isprint (const wchar_t s);

};  

typedef ACE_OS_String_W ACE_OS_String;

#else /* ACE_USES_WCHAR */

typedef ACE_OS_String ACE_OS_String_C;

class ACE_OS_String_W : public ACE_OS_String
{
public:
  static int ace_isspace (const wchar_t s);
  static int ace_isprint (const wchar_t s);

};  

#endif /* ACE_USES_WCHAR */
#else /* ACE_HAS_WCHAR */

typedef ACE_OS_String_C ACE_OS_String;

#endif /* ACE_HAS_WCHAR */

#include "ace/post.h"
#endif /* ACE_ADAPTER_OS_STRING_H */
