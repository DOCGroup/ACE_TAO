/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_wchar.h
 *
 *  $Id$
 *
 *  @author Darrell Brunsch <brunsch@uci.edu>
 */
//=============================================================================

#ifndef ACE_WCHAR_H
#define ACE_WCHAR_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// These macros have been deprecated and should be replaced by their
// ACE_TEXT_* equivalents.  These macros are just hacks and may not
// completely provide the old functionality.
#if defined (ACE_LEGACY_MODE)
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

# if defined (ACE_USES_WCHAR)
#   define ASYS_ONLY_WIDE_STRING(STRING) STRING
# else /* ACE_USES_WCHAR */
#   define ASYS_ONLY_WIDE_STRING(STRING) \
           ACE_Ascii_To_Wide (STRING).wchar_rep ()
# endif /* ACE_USES_WCHAR */

# define ACE_TEXT_STRING ACE_TString

#endif /* ACE_LEGACY_MODE */

#if defined (ACE_HAS_WCHAR)
# if defined (VXWORKS)
#   include /**/ <types/vxTypes.h>  /* For wchar_t */
#   include /**/ <stdlib.h>         /* For mbstowcs, etc. */
#   include /**/ <string.h>         /* For strlen */
#   define wint_t unsigned int      /* VxWorks has wchar_t but not wint_t */
# elif defined (ACE_HAS_STANDARD_CPP_LIBRARY) && \
    (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
#   include /**/ <cwchar>
#   include /**/ <cwctype>
# elif defined (ACE_HAS_WINCE)
#   include /**/ <wtypes.h>
# elif !defined (__BORLANDC__)
#   include /**/ <wchar.h>
# endif /* ACE_HAS_STANDARD_CPP_LIBRARY */
#elif defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)
# include /**/ <wchar.h>
#endif

#if defined (ACE_USES_STD_NAMESPACE_FOR_STDC_LIB) && \
            (ACE_USES_STD_NAMESPACE_FOR_STDC_LIB != 0)
using std::size_t;
#endif /* ACE_USES_STD_NAMESPACE_FOR_STDC_LIB */

// Since MSVC uses ushort16 = wchar_t, it is the only one
// that does not need this defined

#if defined (ACE_HAS_WCHAR) && !defined (_MSC_VER)
# if !defined (ACE_WSTRING_HAS_USHORT_SUPPORT)
#  define ACE_WSTRING_HAS_USHORT_SUPPORT
# endif /* ACE_WSTRING_HAS_USHORT_SUPPORT */
#endif /* ACE_HAS_WCHAR && !_MSC_VER */

// Set the default behaviour for ACE_TEXT_WIDE to use the L-prefix
#if !defined (ACE_USES_L_PREFIX)
# define ACE_USES_L_PREFIX 1
#endif /* ACE_USES_L_PREFIX */

// Define the unicode/wchar related macros correctly

# if !defined (ACE_TEXT_WIDE)
#  if (ACE_USES_L_PREFIX == 1)
#    define ACE_TEXT_WIDE_I(STRING) L##STRING
#  else /* ACE_USES_L_PREFIX */
#    define ACE_TEXT_WIDE_I(STRING) STRING
#  endif /* ACE_USES_L_PREFIX */
#  define ACE_TEXT_WIDE(STRING) ACE_TEXT_WIDE_I (STRING)
# endif /* ACE_TEXT_WIDE */

#if defined (ACE_USES_WCHAR)
typedef wchar_t ACE_TCHAR;
typedef char ACE_ANTI_TCHAR;
# define ACE_LIB_TEXT(STRING) ACE_TEXT_WIDE (STRING)
# define ACE_TEXT_ALWAYS_CHAR(STRING) ACE_Wide_To_Ascii (STRING).char_rep ()
# define ACE_TEXT_CHAR_TO_TCHAR(STRING) ACE_Ascii_To_Wide (STRING).wchar_rep ()
# define ACE_TEXT_WCHAR_TO_TCHAR(STRING) STRING
# define ACE_TEXT_ANTI_TO_TCHAR(STRING) ACE_Ascii_To_Wide (STRING).wchar_rep ()
#else /* ACE_USES_WCHAR */
typedef char ACE_TCHAR;
typedef wchar_t ACE_ANTI_TCHAR;
# define ACE_LIB_TEXT(STRING) STRING
# define ACE_TEXT_ALWAYS_CHAR(STRING) STRING
# define ACE_TEXT_CHAR_TO_TCHAR(STRING) STRING
# define ACE_TEXT_WCHAR_TO_TCHAR(STRING) ACE_Wide_To_Ascii (STRING).char_rep ()
# define ACE_TEXT_ANTI_TO_TCHAR(STRING) ACE_Wide_To_Ascii (STRING).char_rep ()
#endif /* ACE_USES_WCHAR */

#if defined (ACE_LEGACY_MODE)
# define ACE_TEXT TEXT
#else /* ACE_LEGACY_MODE */
# define ACE_TEXT ACE_LIB_TEXT
#endif /* ACE_LEGACY_MODE */

// The OS_String module defines some wide-char functions that are not
// universally available. In particular, they're not part of the
// XPG4 Worldwide Portability Interface wide-character string handling
// functions. So, if ACE_HAS_XPG4_MULTIBYTE_CHAR is defined, note that
// these functions are missing.
#if defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)

#  if !defined (ACE_LACKS_ITOW)
#    define ACE_LACKS_ITOW
#  endif

#  if !defined (ACE_LACKS_WCSICMP)
#    define ACE_LACKS_WCSICMP
#  endif

#  if !defined (ACE_LACKS_WCSNICMP)
#    define ACE_LACKS_WCSNICMP
#  endif

#  if !defined (ACE_LACKS_WCSDUP)
#    define ACE_LACKS_WCSDUP
#  endif

#endif /* ACE_HAS_XPG4_MULTIBYTE_CHAR */

#if defined ACE_HAS_WCHAR
/**
 * @class ACE_Wide_To_Ascii
 *
 * @brief A lightweight wchar* to char* string conversion class.
 *
 * The purpose of this class is to perform conversion from
 * wchar* to char* strings.  It is not intended for general
 * purpose use.
 */
class ACE_Wide_To_Ascii
{
public:
  /// Ctor must take a wchar string.
  ACE_Wide_To_Ascii (const wchar_t *s);

  /// Dtor will free up the memory.
  ~ACE_Wide_To_Ascii (void);

  /// Return the internal char* representation.
  char *char_rep (void);

  /// Converts an wchar_t string to ascii and returns a new string.
  static char *convert (const wchar_t *wstr);

private:
  /// Internal pointer to the converted string.
  char *s_;

  /// Disallow these operation.
  ACE_Wide_To_Ascii (void);
  ACE_Wide_To_Ascii (ACE_Wide_To_Ascii &);
  ACE_Wide_To_Ascii& operator= (ACE_Wide_To_Ascii &);
};

/**
 * @class ACE_Ascii_To_Wide
 *
 * @brief A lightweight char* to wchar* string conversion class.
 *
 * The purpose of this class is to perform conversion from
 * char* to wchar* strings.  It is not intended for general
 * purpose use.
 */
class ACE_Ascii_To_Wide
{
public:
  /// Ctor must take a wchar string.
  ACE_Ascii_To_Wide (const char *s);

  /// Dtor will free up the memory.
  ~ACE_Ascii_To_Wide (void);

  /// Return the internal wchar* representation.
  wchar_t *wchar_rep (void);

  /// Converts an char string to unicode/wide and returns a new string.
  static wchar_t *convert (const char *str);

private:
  /// Internal pointer to the converted string.
  wchar_t *s_;

  /// Disallow these operation.
  ACE_Ascii_To_Wide (void);
  ACE_Ascii_To_Wide (ACE_Ascii_To_Wide &);
  ACE_Ascii_To_Wide operator= (ACE_Ascii_To_Wide &);
};

#if defined (ACE_LEGACY_MODE)
typedef ACE_Ascii_To_Wide ACE_OS_CString;
typedef ACE_Wide_To_Ascii ACE_OS_WString;
#endif /* ACE_LEGACY_MODE */

#endif /* ACE_HAS_WCHAR */

#if defined (ACE_WIN32)
#if defined (ACE_USES_WCHAR)
#define ACE_TEXT_SERVICE_TABLE_ENTRY             SERVICE_TABLE_ENTRYW
#define ACE_TEXT_STARTUPINFO                     STARTUPINFOW
#define ACE_TEXT_WIN32_FIND_DATA                 WIN32_FIND_DATAW

#define ACE_TEXT_CreateEvent                     ::CreateEventW
#define ACE_TEXT_CreateFile                      ::CreateFileW
#define ACE_TEXT_CreateFileMapping               ::CreateFileMappingW
#define ACE_TEXT_CreateMutex                     ::CreateMutexW
#define ACE_TEXT_CreateProcess                   ::CreateProcessW
#define ACE_TEXT_CreateSemaphore                 ::CreateSemaphoreW
#define ACE_TEXT_CreateService                   ::CreateServiceW
#define ACE_TEXT_ExpandEnvironmentStrings        ::ExpandEnvironmentStringsW
#define ACE_TEXT_FindFirstChangeNotification     ::FindFirstChangeNotificationW
#define ACE_TEXT_FindFirstFile                   ::FindFirstFileW
#define ACE_TEXT_FindNextFile                    ::FindNextFileW
#define ACE_TEXT_FormatMessage                   ::FormatMessageW
#define ACE_TEXT_FreeEnvironmentStrings          ::FreeEnvironmentStringsW
#define ACE_TEXT_GetComputerName                 ::GetComputerNameW
#define ACE_TEXT_GetEnvironmentStrings           ::GetEnvironmentStringsW
#define ACE_TEXT_GetModuleFileName               ::GetModuleFileNameW
#define ACE_TEXT_GetTempPath                     ::GetTempPathW
#define ACE_TEXT_GetUserName                     ::GetUserNameW
#define ACE_TEXT_LoadLibrary                     ::LoadLibraryW
#define ACE_TEXT_MoveFileEx                      ::MoveFileExW
#define ACE_TEXT_OpenFileMapping                 ::OpenFileMappingW
#define ACE_TEXT_OpenSCManager                   ::OpenSCManagerW
#define ACE_TEXT_OpenService                     ::OpenServiceW
#define ACE_TEXT_RegisterEventSource             ::RegisterEventSourceW
#define ACE_TEXT_RegisterServiceCtrlHandler      ::RegisterServiceCtrlHandlerW
#define ACE_TEXT_RegConnectRegistry              ::RegConnectRegistryW
#define ACE_TEXT_RegCreateKeyEx                  ::RegCreateKeyExW
#define ACE_TEXT_RegDeleteKey                    ::RegDeleteKeyW
#define ACE_TEXT_RegDeleteValue                  ::RegDeleteValueW
#define ACE_TEXT_RegEnumKeyEx                    ::RegEnumKeyExW
#define ACE_TEXT_RegEnumValue                    ::RegEnumValueW
#define ACE_TEXT_RegCreateKey                    ::RegCreateKeyW
#define ACE_TEXT_RegOpenKey                      ::RegOpenKeyW
#define ACE_TEXT_RegOpenKeyEx                    ::RegOpenKeyExW
#define ACE_TEXT_RegQueryValueEx                 ::RegQueryValueExW
#define ACE_TEXT_RegSetValueEx                   ::RegSetValueExW
#define ACE_TEXT_ReportEvent                     ::ReportEventW
#define ACE_TEXT_StartService                    ::StartServiceW
#define ACE_TEXT_StartServiceCtrlDispatcher      ::StartServiceCtrlDispatcherW

#else /* ACE_USES_WCHAR */
#define ACE_TEXT_SERVICE_TABLE_ENTRY             SERVICE_TABLE_ENTRYA
#define ACE_TEXT_STARTUPINFO                     STARTUPINFOA
#define ACE_TEXT_WIN32_FIND_DATA                 WIN32_FIND_DATAA

#define ACE_TEXT_CreateEvent                     ::CreateEventA
#define ACE_TEXT_CreateFile                      ::CreateFileA
#define ACE_TEXT_CreateFileMapping               ::CreateFileMappingA
#define ACE_TEXT_CreateMutex                     ::CreateMutexA
#define ACE_TEXT_CreateProcess                   ::CreateProcessA
#define ACE_TEXT_CreateSemaphore                 ::CreateSemaphoreA
#define ACE_TEXT_CreateService                   ::CreateServiceA
#define ACE_TEXT_ExpandEnvironmentStrings        ::ExpandEnvironmentStringsA
#define ACE_TEXT_FindFirstChangeNotification     ::FindFirstChangeNotificationA
#define ACE_TEXT_FindFirstFile                   ::FindFirstFileA
#define ACE_TEXT_FindNextFile                    ::FindNextFileA
#define ACE_TEXT_FormatMessage                   ::FormatMessageA
#define ACE_TEXT_FreeEnvironmentStrings          ::FreeEnvironmentStringsA
#define ACE_TEXT_GetComputerName                 ::GetComputerNameA
#define ACE_TEXT_GetEnvironmentStrings           ::GetEnvironmentStringsA
#define ACE_TEXT_GetModuleFileName               ::GetModuleFileNameA
#define ACE_TEXT_GetTempPath                     ::GetTempPathA
#define ACE_TEXT_GetUserName                     ::GetUserNameA
#define ACE_TEXT_LoadLibrary                     ::LoadLibraryA
#define ACE_TEXT_MoveFileEx                      ::MoveFileExA
#define ACE_TEXT_OpenFileMapping                 ::OpenFileMappingA
#define ACE_TEXT_OpenSCManager                   ::OpenSCManagerA
#define ACE_TEXT_OpenService                     ::OpenServiceA
#define ACE_TEXT_RegisterEventSource             ::RegisterEventSourceA
#define ACE_TEXT_RegisterServiceCtrlHandler      ::RegisterServiceCtrlHandlerA
#define ACE_TEXT_RegConnectRegistry              ::RegConnectRegistryA
#define ACE_TEXT_RegCreateKeyEx                  ::RegCreateKeyExA
#define ACE_TEXT_RegDeleteKey                    ::RegDeleteKeyA
#define ACE_TEXT_RegDeleteValue                  ::RegDeleteValueA
#define ACE_TEXT_RegEnumKeyEx                    ::RegEnumKeyExA
#define ACE_TEXT_RegEnumValue                    ::RegEnumValueA
#define ACE_TEXT_RegCreateKey                    ::RegCreateKeyA
#define ACE_TEXT_RegOpenKey                      ::RegOpenKeyA
#define ACE_TEXT_RegOpenKeyEx                    ::RegOpenKeyExA
#define ACE_TEXT_RegQueryValueEx                 ::RegQueryValueExA
#define ACE_TEXT_RegSetValueEx                   ::RegSetValueExA
#define ACE_TEXT_ReportEvent                     ::ReportEventA
#define ACE_TEXT_StartService                    ::StartServiceA
#define ACE_TEXT_StartServiceCtrlDispatcher      ::StartServiceCtrlDispatcherA
#endif /* ACE_USES_WCHAR */
#endif /* ACE_WIN32 */

#include "ace/ace_wchar.inl"

#endif /* ACE_WCHAR_H */
