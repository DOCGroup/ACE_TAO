/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    unicode.h
//
// = AUTHORcd 
//    Darrell Brunsch
//
// ============================================================================

#ifndef ACE_WCHAR_H
#define ACE_WCHAR_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// These macros have been deprecated and should be replaced by their
// ACE_TEXT_* equivalents.  These macros are just hacks and may not
// completely provide the old functionality.
#if !defined (ACE_ONLY_LATEST_AND_GREATEST)
// Convert the old unicode indicators
# if defined (ACE_HAS_MOSTLY_UNICODE_APIS)
#   define ACE_USES_WCHAR
# endif /* ACE_HAS_MOSTLY_UNICODE_APIS */
# if defined (ACE_HAS_UNICODE)
#   define ACE_HAS_WCHAR
# endif /* ACE_HAS_UNICODE */

// These are defined to get older stuff to compile
// FUZZ: disable check_for_tchar
# define ASYS_TCHAR ACE_TCHAR
# define ASYS_TEXT ACE_TEXT
# define ASYS_ONLY_MULTIBYTE_STRING ACE_TEXT_ALWAYS_CHAR
# define ASYS_MULTIBYTE_STRING ACE_TEXT_CHAR_TO_TCHAR
# define ASYS_WIDE_STRING ACE_TEXT_CHAR_TO_TCHAR
# define ACE_WIDE_STRING ACE_TEXT_CHAR_TO_TCHAR
# define ACE_DIRECTORY_SEPARATOR_CHAR_A ACE_DIRECTORY_SEPARATOR_CHAR

# if defined (ACE_USES_WCHAR)
#   define ASYS_ONLY_WIDE_STRING(STRING) STRING
# else /* ACE_USES_WCHAR */
#   define ASYS_ONLY_WIDE_STRING(STRING) ACE_Ascii_To_Wide (STRING).wchar_rep ()
# endif /* ACE_USES_WCHAR */

# define ACE_TEXT_STRING ACE_TString

# if !defined (ACE_WIN32)
#   if (defined (ACE_HAS_UNICODE) && (defined (UNICODE)))
typedef const wchar_t *LPCTSTR;
typedef wchar_t *LPTSTR;
typedef wchar_t TCHAR;
#   else
typedef const char *LPCTSTR;
typedef char *LPTSTR;
typedef char TCHAR;
#   endif /* ACE_HAS_UNICODE && UNICODE */
# endif /* ACE_WIN32 */

#endif /* ACE_ONLY_LATEST_AND_GREATEST */

#if defined (ACE_HAS_WCHAR)
# if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && \
    (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
#   include /**/ <cwchar>
# elif !defined (__BORLANDC__) && !defined (ACE_HAS_WINCE)
#   include /**/ <wchar.h>
# endif /* ACE_HAS_STANDARD_CPP_LIBRARY */
#elif defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)
# include /**/ <wchar.h>
#endif


// Define the unicode/wchar related macros correctly
#if defined ACE_HAS_WCHAR
# define ACE_TEXT_CHAR_TO_WCHAR(STRING) ACE_Ascii_To_Wide (STRING).wchar_rep ()
# define ACE_TEXT_WCHAR_TO_CHAR(STRING) ACE_Wide_To_Ascii (STRING).char_rep ()
#endif /* ACE_HAS_WCHAR */

#if defined (ACE_USES_WCHAR)
typedef wchar_t ACE_TCHAR;
# define ACE_TEXT(STRING) L##STRING
# define ACE_TEXT_ALWAYS_CHAR(STRING) ACE_Wide_To_Ascii (STRING).char_rep ()
# define ACE_TEXT_CHAR_TO_TCHAR(STRING) ACE_Ascii_To_Wide (STRING).wchar_rep ()
#else /* ACE_USES_WCHAR */
typedef char ACE_TCHAR;
# define ACE_TEXT(STRING) STRING
# define ACE_TEXT_ALWAYS_CHAR(STRING) STRING
# define ACE_TEXT_CHAR_TO_TCHAR(STRING) STRING
#endif /* ACE_USES_WCHAR */

#if defined ACE_HAS_WCHAR
class ACE_Wide_To_Ascii
{
  // = TITLE
  //     A lightweight wchar* to char* string conversion class.
  //
  // = DESCRIPTION
  //     The purpose of this class is to perform conversion from
  //     wchar* to char* strings.  It is not intended for general
  //     purpose use.
public:
  ACE_Wide_To_Ascii (const wchar_t *s) : s_ (ACE_Wide_To_Ascii::convert (s)) {}
  // Ctor must take a wchar string.

  ~ACE_Wide_To_Ascii (void) { delete [] this->s_; }
  // Dtor will free up the memory.

  char *char_rep (void) { return this->s_; }
  // Return the internal char* representation.

  static char *convert (const wchar_t *wstr)
  // Converts an wchar_t string to ascii and returns a new string.
  {
# if defined (ACE_WIN32)
    size_t len = ::WideCharToMultiByte (CP_OEMCP, 0, wstr, -1, 
                                        NULL, 0, NULL, NULL);
# else
    size_t len = ::wcslen (wstr) + 1;
# endif

    char *str = new char[len];

# if defined (ACE_WIN32)
    ::WideCharToMultiByte (CP_OEMCP, 0, wstr, -1, str, len, NULL, NULL);
# else /* ACE_WIN32 */
    for (size_t i = 0; i < len; i++)
      {
        wchar_t *t = ACE_const_cast (wchar_t *, wstr);
        str[i] = ACE_static_cast (char, *(t + i));
      }
# endif /* ACE_WIN32 */
    return str;
  }

private:
  char *s_;
  // Internal pointer to the converted string.

  ACE_Wide_To_Ascii (void);
  ACE_Wide_To_Ascii (ACE_Wide_To_Ascii &);
  ACE_Wide_To_Ascii& operator= (ACE_Wide_To_Ascii &);
  // Disallow these operation.
};

class ACE_Ascii_To_Wide
{
  // = TITLE
  //     A lightweight char* to wchar* string conversion class.
  //
  // = DESCRIPTION
  //     The purpose of this class is to perform conversion from
  //     char* to wchar* strings.  It is not intended for general
  //     purpose use.
public:
  ACE_Ascii_To_Wide (const char *s) : s_ (ACE_Ascii_To_Wide::convert (s)) {}
  // Ctor must take a wchar string.

  ~ACE_Ascii_To_Wide (void) { delete [] this->s_; }
  // Dtor will free up the memory.

  wchar_t *wchar_rep (void) { return this->s_; }
  // Return the internal wchar* representation.

  static wchar_t *convert (const char *str)
  // Converts an char string to unicode/wide and returns a new string.
  {
# if defined (ACE_WIN32)
    size_t len = ::MultiByteToWideChar (CP_OEMCP, 0, str, -1, NULL, 0);
# else /* ACE_WIN32 */
    size_t len = strlen (str) + 1;
# endif /* ACE_WIN32 */
    
    wchar_t *wstr = new wchar_t[len];

# if defined (ACE_WIN32)
    ::MultiByteToWideChar (CP_OEMCP, 0, str, -1, wstr, len);
# else /* ACE_WIN32 */
    for (size_t i = 0; i < len; i++)
      {
        char *t = ACE_const_cast (char *, str);
        wstr[i] = ACE_static_cast (wchar_t, *(t + i));
      }
# endif /* ACE_WIN32 */
    return wstr;
  }
private:
  wchar_t *s_;
  // Internal pointer to the converted string.

  ACE_Ascii_To_Wide (void);
  ACE_Ascii_To_Wide (ACE_Ascii_To_Wide &);
  ACE_Ascii_To_Wide operator= (ACE_Ascii_To_Wide &);
  // Disallow these operation.
};

#endif /* ACE_HAS_WCHAR */

#if defined (ACE_WIN32)
#if defined (ACE_USES_WCHAR)
#define ACE_TEXT_STARTUPINFO              STARTUPINFOW
#define ACE_TEXT_WIN32_FIND_DATA          WIN32_FIND_DATAW

#define ACE_TEXT_CreateEvent              ::CreateEventW
#define ACE_TEXT_CreateFile               ::CreateFileW
#define ACE_TEXT_CreateFileMapping        ::CreateFileMappingW
#define ACE_TEXT_CreateMutex              ::CreateMutexW
#define ACE_TEXT_CreateProcess            ::CreateProcessW
#define ACE_TEXT_CreateSemaphore          ::CreateSemaphoreW
#define ACE_TEXT_CreateService            ::CreateServiceW
#define ACE_TEXT_ExpandEnvironmentStrings ::ExpandEnvironmentStringsW
#define ACE_TEXT_FindFirstFile            ::FindFirstFileW
#define ACE_TEXT_FindNextFile             ::FindNextFileW
#define ACE_TEXT_FormatMessage            ::FormatMessageW
#define ACE_TEXT_FreeEnvironmentStrings   ::FreeEnvironmentStringsW
#define ACE_TEXT_GetComputerName          ::GetComputerNameW
#define ACE_TEXT_GetModuleFileName        ::GetModuleFileNameW
#define ACE_TEXT_GetTempPath              ::GetTempPathW
#define ACE_TEXT_GetUserName              ::GetUserNameW
#define ACE_TEXT_LoadLibrary              ::LoadLibraryW
#define ACE_TEXT_OpenSCManager            ::OpenSCManagerW
#define ACE_TEXT_OpenService              ::OpenServiceW
#define ACE_TEXT_RegConnectRegistry       ::RegConnectRegistryW
#define ACE_TEXT_RegCreateKeyEx           ::RegCreateKeyExW
#define ACE_TEXT_RegDeleteKey             ::RegDeleteKeyW
#define ACE_TEXT_RegDeleteValue           ::RegDeleteValueW
#define ACE_TEXT_RegEnumKeyEx             ::RegEnumKeyExW
#define ACE_TEXT_RegEnumValue             ::RegEnumValueW
#define ACE_TEXT_RegOpenKey               ::RegOpenKeyW
#define ACE_TEXT_RegOpenKeyEx             ::RegOpenKeyExW
#define ACE_TEXT_RegQueryValueEx          ::RegQueryValueExW
#define ACE_TEXT_RegSetValueEx            ::RegSetValueExW
#define ACE_TEXT_StartService             ::StartServiceW

#else /* ACE_USES_WCHAR */
#define ACE_TEXT_STARTUPINFO              STARTUPINFOA
#define ACE_TEXT_WIN32_FIND_DATA          WIN32_FIND_DATAA

#define ACE_TEXT_CreateEvent              ::CreateEventA
#define ACE_TEXT_CreateFile               ::CreateFileA
#define ACE_TEXT_CreateFileMapping        ::CreateFileMappingA
#define ACE_TEXT_CreateMutex              ::CreateMutexA
#define ACE_TEXT_CreateProcess            ::CreateProcessA
#define ACE_TEXT_CreateSemaphore          ::CreateSemaphoreA
#define ACE_TEXT_CreateService            ::CreateServiceA
#define ACE_TEXT_ExpandEnvironmentStrings ::ExpandEnvironmentStringsA
#define ACE_TEXT_FindFirstFile            ::FindFirstFileA
#define ACE_TEXT_FindNextFile             ::FindNextFileA
#define ACE_TEXT_FormatMessage            ::FormatMessageA
#define ACE_TEXT_FreeEnvironmentStrings   ::FreeEnvironmentStringsA
#define ACE_TEXT_GetComputerName          ::GetComputerNameA
#define ACE_TEXT_GetModuleFileName        ::GetModuleFileNameA
#define ACE_TEXT_GetTempPath              ::GetTempPathA
#define ACE_TEXT_GetUserName              ::GetUserNameA
#define ACE_TEXT_LoadLibrary              ::LoadLibraryA
#define ACE_TEXT_OpenSCManager            ::OpenSCManagerA
#define ACE_TEXT_OpenService              ::OpenServiceA
#define ACE_TEXT_RegConnectRegistry       ::RegConnectRegistryA
#define ACE_TEXT_RegCreateKeyEx           ::RegCreateKeyExA
#define ACE_TEXT_RegDeleteKey             ::RegDeleteKeyA
#define ACE_TEXT_RegDeleteValue           ::RegDeleteValueA
#define ACE_TEXT_RegEnumKeyEx             ::RegEnumKeyExA
#define ACE_TEXT_RegEnumValue             ::RegEnumValueA
#define ACE_TEXT_RegOpenKey               ::RegOpenKeyA
#define ACE_TEXT_RegOpenKeyEx             ::RegOpenKeyExA
#define ACE_TEXT_RegQueryValueEx          ::RegQueryValueExA
#define ACE_TEXT_RegSetValueEx            ::RegSetValueExA
#define ACE_TEXT_StartService             ::StartServiceA
#endif /* ACE_USES_WCHAR */
#endif /* ACE_WIN32 */

#endif /* ACE_WCHAR_H */
