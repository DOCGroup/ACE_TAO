// -*- C++ -*-
// $Id$

//
// The following configuration file is designed to work for win32
// platforms using gcc/g++ with mingw32 (http://www.mingw.org).
//

#ifndef ACE_CONFIG_WIN32_MINGW_H
#define ACE_CONFIG_WIN32_MINGW_H
#include "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
#error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

#define ACE_CC_NAME ACE_LIB_TEXT ("g++")
#define ACE_CC_PREPROCESSOR "cpp"
#define ACE_CC_PREPROCESOR_ARGS ""

#define ACE_HAS_BROKEN_SAP_ANY

// Why all this is not in config-g++-common.h?
#define ACE_CC_MAJOR_VERSION __GNUC__
#define ACE_CC_MINOR_VERSION __GNUC_MINOR__
#define ACE_CC_BETA_VERSION (0)

#if !defined(__MINGW32__)
#  error You do not seem to be using mingw32
#endif

#include "ace/config-g++-common.h"

#include /**/ <_mingw.h>
#include /**/ <w32api.h>

#define ACE_LACKS_MODE_MASKS
#define ACE_HAS_USER_MODE_MASKS

#if (__MINGW32_MAJOR_VERSION == 0) && (__MINGW32_MINOR_VERSION < 5)
#error You need a newer version (>= 0.5) of mingw32/w32api
#endif

#define ACE_LACKS_STRRECVFD
#define ACE_HAS_STRERROR

// We trust the user: He must have used -mpentiumpro or -mpentium
// if that is what he wants.
#if defined(pentiumpro) || defined(pentium)
# define ACE_HAS_PENTIUM
#endif

#if !defined (ACE_HAS_WINNT4)
# if (defined (WINNT) && WINNT == 1) \
     || (defined (__WINNT__) && __WINNT__ == 1)
#  define ACE_HAS_WINNT4 1
# else
#  define ACE_HAS_WINNT4 0
# endif
#endif

#define ACE_ENDTHREADEX(STATUS)  ::_endthreadex ((DWORD) (STATUS))

#if ( __W32API_MAJOR_VERSION < 1) || ((__W32API_MAJOR_VERSION == 1) && (__W32API_MINOR_VERSION <= 5))

// The MingW32 w32api version 1.50 and lower don't define these types and methods
// but we need it in the Win32_Asynch_IO.cpp

typedef void *PVOID,*LPVOID;

/* FIXME for __WIN64 */
#ifndef  __ptr64
#define __ptr64
#endif
typedef void* __ptr64 PVOID64;

//
// Define segement buffer structure for scatter/gather read/write.
//
typedef union _FILE_SEGMENT_ELEMENT {
           PVOID64 Buffer;
           ULONGLONG Alignment;
}FILE_SEGMENT_ELEMENT, *PFILE_SEGMENT_ELEMENT;


BOOL WINAPI ReadFileScatter(HANDLE,FILE_SEGMENT_ELEMENT[],DWORD,LPDWORD,LPOVERLAPPED);

BOOL WINAPI WriteFileGather(HANDLE,FILE_SEGMENT_ELEMENT[],DWORD,LPDWORD,LPOVERLAPPED);
#endif

#include "ace/post.h"
#endif /* ACE_CONFIG_WIN32_MINGW_H */
