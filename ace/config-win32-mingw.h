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

// The MingW32 w32api version 1.4 and 1.5 don't define this union but we need
// it in the Win32_Asynch_IO.cpp
#if defined (ACE_MINGW_LACKS_FILE_SEGMENT_ELEMENT)

// Define stuff for PVOID64
#if !defined(_MAC) && (defined(_M_MRX000) || defined(_M_ALPHA) || defined(_M_IA64)) && (_MSC_VER >= 1100) && !(defined(MIDL_PASS) || defined(RC_INVOKED))
#define POINTER_64 __ptr64
typedef unsigned __int64 POINTER_64_INT;
#if defined(_AXP64_)
#define POINTER_32 __ptr32
#else
#define POINTER_32
#endif
#else
#if defined(_MAC) && defined(_MAC_INT_64)
#define POINTER_64 __ptr64
typedef unsigned __int64 POINTER_64_INT;
#else
#define POINTER_64
typedef unsigned long POINTER_64_INT;
#endif
#define POINTER_32
#endif

typedef void * POINTER_64 PVOID64;

//
// Define segement buffer structure for scatter/gather read/write.
//
typedef union _FILE_SEGMENT_ELEMENT {
    PVOID64 Buffer;
    ULONGLONG Alignment;
}FILE_SEGMENT_ELEMENT, *PFILE_SEGMENT_ELEMENT;
#endif

// The MingW32 w32api version 1.4 and 1.5 don't define these methods but we need
// it in the Win32_Asynch_IO.cpp
#if defined (ACE_MINGW_LACKS_READWRITE_FILESCATTER)
BOOL
WINAPI
ReadFileScatter(
    IN HANDLE hFile,
    IN FILE_SEGMENT_ELEMENT aSegmentArray[],
    IN DWORD nNumberOfBytesToRead,
    IN LPDWORD lpReserved,
    IN LPOVERLAPPED lpOverlapped
    );

BOOL
WINAPI
WriteFileGather(
    IN HANDLE hFile,
    OUT FILE_SEGMENT_ELEMENT aSegmentArray[],
    IN DWORD nNumberOfBytesToWrite,
    IN LPDWORD lpReserved,
    IN LPOVERLAPPED lpOverlapped
    );
#endif

#include "ace/post.h"
#endif /* ACE_CONFIG_WIN32_MINGW_H */
