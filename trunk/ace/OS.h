/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//   ace
//
// = FILENAME
//   OS.h
//
// = AUTHOR
//   Doug Schmidt <schmidt@cs.wustl.edu>, Jesper S. M|ller
//   <stophph@diku.dk>, and a cast of thousands...
//
// ============================================================================

#ifndef ACE_OS_H
# define ACE_OS_H

// This file should be a link to the platform/compiler-specific
// configuration file (e.g., config-sunos5-sunc++-4.x.h).
# include "ace/inc_user_config.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Get OS.h to compile on some of the platforms without DIR info yet.
# if !defined (ACE_HAS_DIRENT)
    typedef int DIR;
    struct dirent {};
# endif /* ACE_HAS_DIRENT */

# if defined (ACE_PSOS_TM)
    typedef long long longlong_t;
    typedef long      id_t;
# endif /* ACE_PSOS_TM */

# if defined (ACE_HAS_MOSTLY_UNICODE_APIS) && !defined (UNICODE)
#   error UNICODE must be defined when using ACE_HAS_MOSTLY_UNICODE_APIS, check your compiler document on how to enable UNICODE.
# endif /* ACE_HAS_MOSTLY_UNICODE_APIS && !UNICODE */

# if defined (ACE_LACKS_INLINE_FUNCTIONS) && !defined (ACE_NO_INLINE)
#   define ACE_NO_INLINE
# endif /* defined (ACE_LACKS_INLINE_FUNCTIONS) && !defined (ACE_NO_INLINE) */

# if defined (ACE_HAS_ANSI_CASTS)
#   define ACE_sap_any_cast(TYPE)                                      reinterpret_cast<TYPE> (const_cast<ACE_Addr &> (ACE_Addr::sap_any))
#   define ACE_static_cast(TYPE, EXPR)                                 static_cast<TYPE> (EXPR)
#   define ACE_const_cast(TYPE, EXPR)                                  const_cast<TYPE> (EXPR)
#   define ACE_reinterpret_cast(TYPE, EXPR)                            reinterpret_cast<TYPE> (EXPR)
#   if defined (ACE_LACKS_RTTI)
#     define ACE_dynamic_cast(TYPE, EXPR)                              static_cast< TYPE > (EXPR)
#     define ACE_dynamic_cast_1_ptr(TYPE, T1, EXPR)                    static_cast< TYPE<T1> *> (EXPR)
#     define ACE_dynamic_cast_2_ptr(TYPE, T1, T2, EXPR)                static_cast< TYPE<T1, T2> *> (EXPR)
#     define ACE_dynamic_cast_3_ptr(TYPE, T1, T2, T3, EXPR)            static_cast< TYPE<T1, T2, T3> *> (EXPR)
#     define ACE_dynamic_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)        static_cast< TYPE<T1, T2, T3, T4> *> (EXPR)
#     define ACE_dynamic_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)    static_cast< TYPE<T1, T2, T3, T4, T5> *> (EXPR)
#     define ACE_dynamic_cast_1_ref(TYPE, T1, EXPR)                    static_cast< TYPE<T1> &> (EXPR)
#     define ACE_dynamic_cast_2_ref(TYPE, T1, T2, EXPR)                static_cast< TYPE<T1, T2> &> (EXPR)
#     define ACE_dynamic_cast_3_ref(TYPE, T1, T2, T3, EXPR)            static_cast< TYPE<T1, T2, T3> &> (EXPR)
#     define ACE_dynamic_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)        static_cast< TYPE<T1, T2, T3, T4> &> (EXPR)
#     define ACE_dynamic_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)    static_cast< TYPE<T1, T2, T3, T4, T5> &> (EXPR)
#   else  /* ! ACE_LACKS_RTTI */
#     define ACE_dynamic_cast(TYPE, EXPR)                              dynamic_cast< TYPE > (EXPR)
#     define ACE_dynamic_cast_1_ptr(TYPE, T1, EXPR)                    dynamic_cast< TYPE<T1> *> (EXPR)
#     define ACE_dynamic_cast_2_ptr(TYPE, T1, T2, EXPR)                dynamic_cast< TYPE<T1, T2> *> (EXPR)
#     define ACE_dynamic_cast_3_ptr(TYPE, T1, T2, T3, EXPR)            dynamic_cast< TYPE<T1, T2, T3> *> (EXPR)
#     define ACE_dynamic_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)        dynamic_cast< TYPE<T1, T2, T3, T4> *> (EXPR)
#     define ACE_dynamic_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)    dynamic_cast< TYPE<T1, T2, T3, T4, T5> *> (EXPR)
#     define ACE_dynamic_cast_1_ref(TYPE, T1, EXPR)                    dynamic_cast< TYPE<T1> &> (EXPR)
#     define ACE_dynamic_cast_2_ref(TYPE, T1, T2, EXPR)                dynamic_cast< TYPE<T1, T2> &> (EXPR)
#     define ACE_dynamic_cast_3_ref(TYPE, T1, T2, T3, EXPR)            dynamic_cast< TYPE<T1, T2, T3> &> (EXPR)
#     define ACE_dynamic_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)        dynamic_cast< TYPE<T1, T2, T3, T4> &> (EXPR)
#     define ACE_dynamic_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)    dynamic_cast< TYPE<T1, T2, T3, T4, T5> &> (EXPR)
#   endif /* ! ACE_LACKS_RTTI */
# else
#   define ACE_sap_any_cast(TYPE)                                      ((TYPE) (ACE_Addr::sap_any))
#   define ACE_static_cast(TYPE, EXPR)                                 ((TYPE) (EXPR))
#   define ACE_const_cast(TYPE, EXPR)                                  ((TYPE) (EXPR))
#   define ACE_reinterpret_cast(TYPE, EXPR)                            ((TYPE) (EXPR))
#   define ACE_dynamic_cast(TYPE, EXPR)                                ((TYPE) (EXPR))
#   define ACE_dynamic_cast_1_ptr(TYPE, T1, EXPR)                      ((TYPE<T1> *) (EXPR))
#   define ACE_dynamic_cast_2_ptr(TYPE, T1, T2, EXPR)                  ((TYPE<T1, T2> *) (EXPR))
#   define ACE_dynamic_cast_3_ptr(TYPE, T1, T2, T3, EXPR)              ((TYPE<T1, T2, T3> *) (EXPR))
#   define ACE_dynamic_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)          ((TYPE<T1, T2, T3, T4> *) (EXPR))
#   define ACE_dynamic_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)      ((TYPE<T1, T2, T3, T4, T5> *) (EXPR))
#   define ACE_dynamic_cast_1_ref(TYPE, T1, EXPR)                      ((TYPE<T1> &) (EXPR))
#   define ACE_dynamic_cast_2_ref(TYPE, T1, T2, EXPR)                  ((TYPE<T1, T2> &) (EXPR))
#   define ACE_dynamic_cast_3_ref(TYPE, T1, T2, T3, EXPR)              ((TYPE<T1, T2, T3> &) (EXPR))
#   define ACE_dynamic_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)          ((TYPE<T1, T2, T3, T4> &) (EXPR))
#   define ACE_dynamic_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)      ((TYPE<T1, T2, T3, T4, T5> &) (EXPR))
# endif /* ACE_HAS_ANSI_CASTS */

# if !defined (ACE_CAST_CONST)
    // Sun CC 4.2, for example, requires const in reinterpret casts of
    // data members in const member functions.  But, other compilers
    // complain about the useless const.  This keeps everyone happy.
#   if defined (__SUNPRO_CC)
#     define ACE_CAST_CONST const
#   else  /* ! __SUNPRO_CC */
#     define ACE_CAST_CONST
#   endif /* ! __SUNPRO_CC */
# endif /* ! ACE_CAST_CONST */

// Deal with MSVC++ insanity for CORBA...
# if defined (ACE_HAS_BROKEN_NAMESPACES)
#   define ACE_CORBA_1(NAME) CORBA_##NAME
#   define ACE_CORBA_2(TYPE, NAME) CORBA_##TYPE##_##NAME
#   define ACE_CORBA_3(TYPE, NAME) CORBA_##TYPE::NAME
#   define ACE_NESTED_CLASS(TYPE, NAME) NAME
# else  /* ! ACE_HAS_BROKEN_NAMESPACES */
#   define ACE_CORBA_1(NAME) CORBA::NAME
#   define ACE_CORBA_2(TYPE, NAME) CORBA::TYPE::NAME
#   define ACE_CORBA_3(TYPE, NAME) CORBA::TYPE::NAME
#   define ACE_NESTED_CLASS(TYPE, NAME) TYPE::NAME
# endif /* ! ACE_HAS_BROKEN_NAMESPACES */

// Define some helpful macros.
# define ACE_ONE_SECOND_IN_MSECS 1000L
# define ACE_ONE_SECOND_IN_USECS 1000000L
# define ACE_ONE_SECOND_IN_NSECS 1000000000L

# if defined (ACE_NO_INLINE)
  // ACE inlining has been explicitly disabled.  Implement
  // internally within ACE by undefining __ACE_INLINE__.
#   undef __ACE_INLINE__
# endif /* ! ACE_NO_INLINE */

# if !defined (ACE_DEFAULT_CLOSE_ALL_HANDLES)
#   define ACE_DEFAULT_CLOSE_ALL_HANDLES 1
# endif /* ACE_DEFAULT_CLOSE_ALL_HANDLES */

// The maximum length for a fully qualified Internet name.
# if !defined(ACE_MAX_FULLY_QUALIFIED_NAME_LEN)
#   define ACE_MAX_FULLY_QUALIFIED_NAME_LEN 256
# endif /* ACE_MAX_FULLY_QUALIFIED_NAME_LEN */

# if defined (ACE_HAS_4_4BSD_SENDMSG_RECVMSG)
    // Control message size to pass a file descriptor.
#   define ACE_BSD_CONTROL_MSG_LEN sizeof (struct cmsghdr) + sizeof (ACE_HANDLE)
#   if defined (ACE_LACKS_CMSG_DATA_MACRO)
#     if defined (ACE_LACKS_CMSG_DATA_MEMBER)
#       define CMSG_DATA(cmsg) ((unsigned char *) ((struct cmsghdr *) (cmsg) + 1))
#     else
#       define CMSG_DATA(cmsg) ((cmsg)->cmsg_data)
#     endif /* ACE_LACKS_CMSG_DATA_MEMBER */
#   endif /* ACE_LACKS_CMSG_DATA_MACRO */
# endif /* ACE_HAS_4_4BSD_SENDMSG_RECVMSG */

// Define the default constants for ACE.  Many of these are used for
// the ACE tests and applications.  You can change these values by
// defining the macros in your config.h file.

# if !defined (ACE_DEFAULT_TIMEOUT)
#   define ACE_DEFAULT_TIMEOUT 5
# endif /* ACE_DEFAULT_TIMEOUT */

# if !defined (ACE_DEFAULT_THREADS)
#   define ACE_DEFAULT_THREADS 1
# endif /* ACE_DEFAULT_THREADS */

// The following 3 defines are used in the IP multicast and broadcast tests.
# if !defined (ACE_DEFAULT_BROADCAST_PORT)
#   define ACE_DEFAULT_BROADCAST_PORT 10000
# endif /* ACE_DEFAULT_BROADCAST_PORT */

# if !defined (ACE_DEFAULT_MULTICAST_PORT)
#   define ACE_DEFAULT_MULTICAST_PORT 10001
# endif /* ACE_DEFAULT_MULTICAST_PORT */

# if !defined (ACE_DEFAULT_MULTICAST_ADDR)
// This address MUST be within the range for host group addresses:
// 224.0.0.0 to 239.255.255.255.
#   define ACE_DEFAULT_MULTICAST_ADDR "224.9.9.2"
# endif /* ACE_DEFAULT_MULTICAST_ADDR */

// Default port number for HTTP.
# if !defined (ACE_DEFAULT_HTTP_SERVER_PORT)
#   define ACE_DEFAULT_HTTP_SERVER_PORT 80
# endif /* ACE_DEFAULT_HTTP_SERVER_PORT */

// Used in many IPC_SAP tests
# if !defined (ACE_DEFAULT_SERVER_PORT)
#   define ACE_DEFAULT_SERVER_PORT 10002
# endif /* ACE_DEFAULT_SERVER_PORT */

# if !defined (ACE_DEFAULT_HTTP_PORT)
#   define ACE_DEFAULT_HTTP_PORT 80
# endif /* ACE_DEFAULT_HTTP_PORT */

# if !defined (ACE_DEFAULT_MAX_SOCKET_BUFSIZ)
#   define ACE_DEFAULT_MAX_SOCKET_BUFSIZ 65536
# endif /* ACE_DEFAULT_MAX_SOCKET_BUFSIZ */

# if !defined (ACE_DEFAULT_SERVER_PORT_STR)
#   define ACE_DEFAULT_SERVER_PORT_STR "10002"
# endif /* ACE_DEFAULT_SERVER_PORT_STR */

// Used for the Service_Directory test
# if !defined (ACE_DEFAULT_SERVICE_PORT)
#   define ACE_DEFAULT_SERVICE_PORT 10003
# endif /* ACE_DEFAULT_SERVICE_PORT */

// Used for the ACE_Thread_Spawn test
# if !defined (ACE_DEFAULT_THR_PORT    )
#   define ACE_DEFAULT_THR_PORT 10004
# endif /* ACE_DEFAULT_THR_PORT */

// Used for SOCK_Connect::connect() tests
# if !defined (ACE_DEFAULT_LOCAL_PORT)
#   define ACE_DEFAULT_LOCAL_PORT 10005
# endif /* ACE_DEFAULT_LOCAL_PORT */

// Used for Connector tests
# if !defined (ACE_DEFAULT_LOCAL_PORT_STR)
#   define ACE_DEFAULT_LOCAL_PORT_STR "10005"
# endif /* ACE_DEFAULT_LOCAL_PORT_STR */

// Used for the name server.
# if !defined (ACE_DEFAULT_NAME_SERVER_PORT)
#   define ACE_DEFAULT_NAME_SERVER_PORT 10006
# endif /* ACE_DEFAULT_NAME_SERVER_PORT */

# if !defined (ACE_DEFAULT_NAME_SERVER_PORT_STR)
#   define ACE_DEFAULT_NAME_SERVER_PORT_STR "10006"
# endif /* ACE_DEFAULT_NAME_SERVER_PORT_STR */

// Used for the token server.
# if !defined (ACE_DEFAULT_TOKEN_SERVER_PORT)
#   define ACE_DEFAULT_TOKEN_SERVER_PORT 10007
# endif /* ACE_DEFAULT_TOKEN_SERVER_PORT */

# if !defined (ACE_DEFAULT_TOKEN_SERVER_PORT_STR)
#   define ACE_DEFAULT_TOKEN_SERVER_PORT_STR "10007"
# endif /* ACE_DEFAULT_TOKEN_SERVER_PORT_STR */

// Used for the logging server.
# if !defined (ACE_DEFAULT_LOGGING_SERVER_PORT)
#   define ACE_DEFAULT_LOGGING_SERVER_PORT 10008
# endif /* ACE_DEFAULT_LOGGING_SERVER_PORT */

# if !defined (ACE_DEFAULT_LOGGING_SERVER_PORT_STR)
#   define ACE_DEFAULT_LOGGING_SERVER_PORT_STR "10008"
# endif /* ACE_DEFAULT_LOGGING_SERVER_PORT_STR */

// Used for the logging server.
# if !defined (ACE_DEFAULT_THR_LOGGING_SERVER_PORT)
#   define ACE_DEFAULT_THR_LOGGING_SERVER_PORT 10008
# endif /* ACE_DEFAULT_THR_LOGGING_SERVER_PORT */

# if !defined (ACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR)
#   define ACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR "10008"
# endif /* ACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR */

// Used for the time server.
# if !defined (ACE_DEFAULT_TIME_SERVER_PORT)
#   define ACE_DEFAULT_TIME_SERVER_PORT 10009
# endif /* ACE_DEFAULT_TIME_SERVER_PORT */

# if !defined (ACE_DEFAULT_TIME_SERVER_PORT_STR)
#   define ACE_DEFAULT_TIME_SERVER_PORT_STR "10009"
# endif /* ACE_DEFAULT_TIME_SERVER_PORT_STR */

# if !defined (ACE_DEFAULT_TIME_SERVER_STR)
#   define ACE_DEFAULT_TIME_SERVER_STR "ACE_TS_TIME"
# endif /* ACE_DEFAULT_TIME_SERVER_STR */

// Used by the FIFO tests and the Client_Logging_Handler netsvc.
# if !defined (ACE_DEFAULT_RENDEZVOUS)
#   if defined (ACE_HAS_STREAM_PIPES)
#     define ACE_DEFAULT_RENDEZVOUS "/tmp/fifo.ace"
#   else
#     define ACE_DEFAULT_RENDEZVOUS "localhost:10010"
#   endif /* ACE_HAS_STREAM_PIPES */
# endif /* ACE_DEFAULT_RENDEZVOUS */

# if !defined (ACE_DEFAULT_LOGGER_KEY)
#   if defined (ACE_HAS_UNICODE) && defined (UNICODE)
#     if defined (ACE_HAS_STREAM_PIPES)
#       define ACE_DEFAULT_LOGGER_KEY L"/tmp/server_daemon"
#     else
#       define ACE_DEFAULT_LOGGER_KEY L"localhost:10012"
#     endif /* ACE_HAS_STREAM_PIPES */
#   else /* ACE_HAS_UNICODE */
#     if defined (ACE_HAS_STREAM_PIPES)
#       define ACE_DEFAULT_LOGGER_KEY "/tmp/server_daemon"
#     else
#       define ACE_DEFAULT_LOGGER_KEY "localhost:10012"
#     endif /* ACE_HAS_STREAM_PIPES */
#   endif /* ACE_HAS_UNICODE && UNICODE */
# endif /* ACE_DEFAULT_LOGGER_KEY */

# if !defined (ACE_DEFAULT_SERVER_HOST)
#   define ACE_DEFAULT_SERVER_HOST "localhost"
# endif /* ACE_DEFAULT_SERVER_HOST */

// Default shared memory key
# if !defined (ACE_DEFAULT_SHM_KEY)
#   define ACE_DEFAULT_SHM_KEY 1234
# endif /* ACE_DEFAULT_SHM_KEY */

// Default segment size used by SYSV shared memory (128 K)
# if !defined (ACE_DEFAULT_SEGMENT_SIZE)
#   define ACE_DEFAULT_SEGMENT_SIZE 1024 * 128
# endif /* ACE_DEFAULT_SEGMENT_SIZE */

// Maximum number of SYSV shared memory segments
// (does anyone know how to figure out the right values?!)
# if !defined (ACE_DEFAULT_MAX_SEGMENTS)
#   define ACE_DEFAULT_MAX_SEGMENTS 6
# endif /* ACE_DEFAULT_MAX_SEGMENTS */

// Name of the map that's stored in shared memory.
# if !defined (ACE_NAME_SERVER_MAP)
#   define ACE_NAME_SERVER_MAP "Name Server Map"
# endif /* ACE_NAME_SERVER_MAP */

// Default file permissions.
# if !defined (ACE_DEFAULT_FILE_PERMS)
#   define ACE_DEFAULT_FILE_PERMS 0666
# endif /* ACE_DEFAULT_FILE_PERMS */

// Default directory permissions.
# if !defined (ACE_DEFAULT_DIR_PERMS)
#   define ACE_DEFAULT_DIR_PERMS 0777
# endif /* ACE_DEFAULT_DIR_PERMS */

// Default size of the ACE Reactor.
# if !defined (ACE_DEFAULT_SELECT_REACTOR_SIZE)
#   define ACE_DEFAULT_SELECT_REACTOR_SIZE FD_SETSIZE
# endif /* ACE_DEFAULT_SELECT_REACTOR_SIZE */

// Default size of the ACE Map_Manager.
# if !defined (ACE_DEFAULT_MAP_SIZE)
#   define ACE_DEFAULT_MAP_SIZE 1024
# endif /* ACE_DEFAULT_MAP_SIZE */

// Defaults for ACE Timer Wheel
# if !defined (ACE_DEFAULT_TIMER_WHEEL_SIZE)
#   define ACE_DEFAULT_TIMER_WHEEL_SIZE 1024
# endif /* ACE_DEFAULT_TIMER_WHEEL_SIZE */

# if !defined (ACE_DEFAULT_TIMER_WHEEL_RESOLUTION)
#   define ACE_DEFAULT_TIMER_WHEEL_RESOLUTION 100
# endif /* ACE_DEFAULT_TIMER_WHEEL_RESOLUTION */

// Default size for ACE Timer Hash table
# if !defined (ACE_DEFAULT_TIMER_HASH_TABLE_SIZE)
#   define ACE_DEFAULT_TIMER_HASH_TABLE_SIZE 1024
# endif /* ACE_DEFAULT_TIMER_HASH_TABLE_SIZE */

// Defaults for the ACE Free List
# if !defined (ACE_DEFAULT_FREE_LIST_PREALLOC)
#   define ACE_DEFAULT_FREE_LIST_PREALLOC 0
# endif /* ACE_DEFAULT_FREE_LIST_PREALLOC */

# if !defined (ACE_DEFAULT_FREE_LIST_LWM)
#   define ACE_DEFAULT_FREE_LIST_LWM 0
# endif /* ACE_DEFAULT_FREE_LIST_LWM */

# if !defined (ACE_DEFAULT_FREE_LIST_HWM)
#   define ACE_DEFAULT_FREE_LIST_HWM 25000
# endif /* ACE_DEFAULT_FREE_LIST_HWM */

# if !defined (ACE_DEFAULT_FREE_LIST_INC)
#   define ACE_DEFAULT_FREE_LIST_INC 100
# endif /* ACE_DEFAULT_FREE_LIST_INC */

# if !defined (ACE_UNIQUE_NAME_LEN)
#   define ACE_UNIQUE_NAME_LEN 100
# endif /* ACE_UNIQUE_NAME_LEN */

# if !defined (ACE_MAX_DGRAM_SIZE)
   // This is just a guess.  8k is the normal limit on
   // most machines because that's what NFS expects.
#   define ACE_MAX_DGRAM_SIZE 8192
# endif /* ACE_MAX_DGRAM_SIZE */

# if !defined (ACE_DEFAULT_ARGV_BUFSIZ)
#   define ACE_DEFAULT_ARGV_BUFSIZ 1024 * 4
# endif /* ACE_DEFAULT_ARGV_BUFSIZ */

// Because most WinCE APIs use wchar strings, many of ACE functions
// must adapt to this change also.  For backward compatibility (most
// platforms still use ASCII char APIs even if they support UNICODE,)
// these new macros are introduced to avoid code bloating.
//
// ASYS stands for ACE SYSTEM something.

# if defined (ACE_HAS_MOSTLY_UNICODE_APIS)
#   define ASYS_TCHAR wchar_t
#   define ASYS_TEXT(STRING)   _TEXT(STRING)
# else
#   define ASYS_TCHAR char
#   define ASYS_TEXT(STRING)   STRING
# endif /* ACE_HAS_MOSTLY_UNICODE_APIS */

// Macro to replace __TEXT
# if !defined (ACE_TEXT)
#   if (defined (ACE_HAS_UNICODE) && (defined (UNICODE)))
#     define ACE_TEXT(STRING) L##STRING
#   else
#     define ACE_TEXT(STRING) STRING
#   endif /* UNICODE && ACE_HAS_UNICODE */
# endif /* !defined ACE_TEXT */

# if !defined (ACE_HAS_TEXT_MACRO_CONFLICT)
#   define __TEXT(STRING) ACE_TEXT(STRING)
# endif /* ACE_HAS_TEXT_MACRO_CONFLICT */

// Here are all ACE-specific global declarations needed throughout
// ACE.

// Helpful dump macros.
# define ACE_BEGIN_DUMP ASYS_TEXT ("\n====\n(%P|%t|%x)")
# define ACE_END_DUMP ASYS_TEXT ("====\n")

// A free list which create more elements when there aren't enough
// elements.
# define ACE_FREE_LIST_WITH_POOL 1

// A simple free list which doen't allocate/deallocate elements.
# define ACE_PURE_FREE_LIST 2

# if defined (ACE_NDEBUG)
#   define ACE_DB(X)
# else
#   define ACE_DB(X) X
# endif /* ACE_NDEBUG */

// Nasty macro stuff to account for Microsoft Win32 DLL nonsense.  We
// use these macros so that we don't end up with ACE software
// hard-coded to Microsoft proprietary extensions to C++.

// First, we define how to properly export/import objects.
# if defined (ACE_WIN32) /* Only Win32 needs special treatment. */
#   if defined(_MSC_VER) || defined(__BORLANDC__)
/*  Microsoft, Borland: */
#     define ACE_Proper_Export_Flag __declspec (dllexport)
#     define ACE_Proper_Import_Flag __declspec (dllimport)
#     define ACE_EXPORT_SINGLETON_DECLARATION(T)  template class __declspec (dllexport) T
#     define ACE_IMPORT_SINGLETON_DECLARATION(T)  extern template class T
#   else /* defined(_MSC_VER) || defined(__BORLANDC__) */
/* Non-Microsoft, non-Borland: */
#     define ACE_Proper_Export_Flag _export
#     define ACE_Proper_Import_Flag _import
#     define ACE_EXPORT_SINGLETON_DECLARATION(T)
#     define ACE_IMPORT_SINGLETON_DECLARATION(T)
#   endif /* defined(_MSC_VER) || defined(__BORLANDC__) */
# else  /* ! ACE_WIN32 */
#   define ACE_Proper_Export_Flag
#   define ACE_Proper_Import_Flag
#   define ACE_EXPORT_SINGLETON_DECLARATION(T)
#   define ACE_IMPORT_SINGLETON_DECLARATION(T)
# endif /* ACE_WIN32 */

// Here are definition for ACE library.
# if defined (ACE_HAS_DLL) && (ACE_HAS_DLL == 1)
#   if defined (ACE_BUILD_DLL)
#     define ACE_Export ACE_Proper_Export_Flag
#     define ACE_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#   else
#     define ACE_Export ACE_Proper_Import_Flag
#     define ACE_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#   endif /* ACE_BUILD_DLL */
# else /* ! ACE_HAS_DLL */
#   define ACE_Export
#   define ACE_SINGLETON_DECLARATION(T)
# endif /* ACE_HAS_DLL */

// Here are definition for ACE_Svc library.
# if defined (ACE_HAS_SVC_DLL) && (ACE_HAS_SVC_DLL == 1)
#   if defined (ACE_BUILD_SVC_DLL)
#     define ACE_Svc_Export ACE_Proper_Export_Flag
#     define ACE_SVC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#   else
#     define ACE_Svc_Export ACE_Proper_Import_Flag
#     define ACE_SVC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#   endif /* ACE_BUILD_SVC_DLL */
# else /* ACE_HAS_SVC_DLL */
#   define ACE_Svc_Export
#   define ACE_SVC_SINGLETON_DECLARATION(T)
# endif /* ACE_HAS_SVC_DLL */

// This is a whim of mine -- that instead of annotating a class with
// ACE_Export in its declaration, we make the declaration near the TOP
// of the file with ACE_DECLARE_EXPORT.
# define ACE_DECLARE_EXPORT(TS,ID) TS ACE_Export ID
// TS = type specifier (e.g., class, struct, int, etc.)
// ID = identifier
// So, how do you use it?  Most of the time, just use ...
// ACE_DECLARE_EXPORT(class, someobject);
// If there are global functions to be exported, then use ...
// ACE_DECLARE_EXPORT(void, globalfunction) (int, ...);
// Someday, when template libraries are supported, we made need ...
// ACE_DECLARE_EXPORT(template class, sometemplate) <class TYPE, class LOCK>;

// ACE_NO_HEAP_CHECK macro can be used to suppress false report of
// memory leaks. It turns off the built-in heap checking until the
// block is left. The old state will then be restored Only used for
// Win32 (in the moment).
# if defined (ACE_WIN32)

// This is necessary to work around bugs with Win32 non-blocking
// connects...
#   if !defined (ACE_NON_BLOCKING_BUG_DELAY)
#     define ACE_NON_BLOCKING_BUG_DELAY 35000
#   endif /* ACE_NON_BLOCKING_BUG_DELAY */

#   if defined (_DEBUG) && !defined (ACE_HAS_WINCE)
class ACE_Export ACE_No_Heap_Check
{
public:
  ACE_No_Heap_Check (void)
    : old_state (_CrtSetDbgFlag (_CRTDBG_REPORT_FLAG))
  { _CrtSetDbgFlag (old_state & ~_CRTDBG_ALLOC_MEM_DF);}
  ~ACE_No_Heap_Check (void) { _CrtSetDbgFlag (old_state);}
private:
  int old_state;
};
#     define ACE_NO_HEAP_CHECK ACE_No_Heap_Check ____no_heap;
#   else /* !_DEBUG */
#     define ACE_NO_HEAP_CHECK
#   endif /* _DEBUG */
# else /* !ACE_WIN32 */
#   define ACE_NO_HEAP_CHECK
# endif /* ACE_WIN32 */

// Turn a number into a string.
# define ACE_ITOA(X) #X

// Create a string of a server address with a "host:port" format.
# define ACE_SERVER_ADDRESS(H,P) H":"P

// A couple useful inline functions for checking whether bits are
// enabled or disabled.

// Efficiently returns the least power of two >= X...
# define ACE_POW(X) (((X) == 0)?1:(X-=1,X|=X>>1,X|=X>>2,X|=X>>4,X|=X>>8,X|=X>>16,(++X)))
# define ACE_EVEN(NUM) (((NUM) & 1) == 0)
# define ACE_ODD(NUM) (((NUM) & 1) == 1)
# define ACE_BIT_ENABLED(WORD, BIT) (((WORD) & (BIT)) != 0)
# define ACE_BIT_DISABLED(WORD, BIT) (((WORD) & (BIT)) == 0)
# define ACE_BIT_CMP_MASK(WORD, BIT, MASK) (((WORD) & (BIT)) == MASK)
# define ACE_SET_BITS(WORD, BITS) (WORD |= (BITS))
# define ACE_CLR_BITS(WORD, BITS) (WORD &= ~(BITS))
# define ACE_MAX(x,y) (((x)>(y))?(x):(y))
# define ACE_MIN(x,y) (((x)<(y))?(x):(y))

// Keep the compiler from complaining about parameters which are not used.
# if defined (ghs) || defined (__GNUC__) || defined (__hpux) || defined (__sgi) || defined (DEC_CXX) || defined (__KCC) || defined (__rational__)
// Some compilers complain about "statement with no effect" with (a).
// This eliminates the warnings, and no code is generated for the null
// conditional statement.  NOTE: that may only be true if -O is enabled,
// such as with GreenHills (ghs) 1.8.8.
#   define ACE_UNUSED_ARG(a) {if (&a) /* null */ ;}
# else
#   define ACE_UNUSED_ARG(a) (a)
# endif /* ghs */

# if defined (__sgi) || defined (ghs) || defined (DEC_CXX) || defined(__BORLANDC__)
#   define ACE_NOTREACHED(a)
# else
#   define ACE_NOTREACHED(a) a
# endif /* defined (__sgi) || defined (ghs) || defined (DEC_CXX) || defined(__BORLANDC__) */

# if defined (ACE_NEEDS_FUNC_DEFINITIONS)
// It just evaporated ;-) Not pleasant.
#   define ACE_UNIMPLEMENTED_FUNC(f)
# else
#   define ACE_UNIMPLEMENTED_FUNC(f) f;
# endif /* ACE_NEEDS_FUNC_DEFINITIONS */

// Easy way to designate that a class is used as a pseudo-namespace.
// Insures that g++ "friendship" anamolies are properly handled.
# define ACE_CLASS_IS_NAMESPACE(CLASSNAME) \
private: \
CLASSNAME (void); \
CLASSNAME (const CLASSNAME&); \
friend class ace_dewarn_gplusplus

// These hooks enable ACE to have all dynamic memory management
// automatically handled on a per-object basis.

# if defined (ACE_HAS_ALLOC_HOOKS)
#   define ACE_ALLOC_HOOK_DECLARE \
  void *operator new (size_t bytes); \
  void operator delete (void *ptr);

  // Note that these are just place holders for now.  They'll
  // be replaced by the ACE_Malloc stuff shortly...
#   define ACE_ALLOC_HOOK_DEFINE(CLASS) \
  void *CLASS::operator new (size_t bytes) { return ::new char[bytes]; } \
  void CLASS::operator delete (void *ptr) { delete (ptr); }
# else
#   define ACE_ALLOC_HOOK_DECLARE struct __Ace {} /* Just need a dummy... */
#   define ACE_ALLOC_HOOK_DEFINE(CLASS)
# endif /* ACE_HAS_ALLOC_HOOKS */

# if defined (ACE_LACKS_KEY_T)
#   if defined (ACE_WIN32)
   // Win32 doesn't use numeric values to name its semaphores, it uses
   // strings!
typedef char *key_t;
#   else
typedef int key_t;
#   endif /* ACE_WIN32 */
# endif /* ACE_LACKS_KEY_T */

# if defined (VXWORKS)
#   if defined (ghs)
    // GreenHills 1.8.8 needs the stdarg.h #include before the #include of
    // vxWorks.h.
    // Also, be sure that these #includes come _after_ the key_t typedef, and
    // before the #include of time.h.
#     include /**/ <stdarg.h>
#   endif /* ghs */

#   include /**/ <vxWorks.h>
# endif /* VXWORKS */


///////////////////////////////////////////
//                                       //
// NOTE: Please do not add any #includes //
//       before this point.  On VxWorks, //
//       vxWorks.h must be #included     //
//       first!                          //
//                                       //
///////////////////////////////////////////

# if defined (ACE_PSOS)

    // remap missing error numbers for system functions
#   define EPERM        1        /* Not super-user                        */
#   define ENOENT       2        /* No such file or directory             */
#   define ESRCH        3        /* No such process                       */
#   if ! defined (ACE_PSOS_PROVIDES_ERROR_SYMBOLS_TM)
#     define EINTR        4        /* interrupted system call               */
#   endif /* ! defined (ACE_PSOS_PROVIDES_ERROR_SYMBOLS_TM) */
#   define EBADF        9        /* Bad file number                       */
#   define EAGAIN       11       /* Resource temporarily unavailable      */
#   if ! defined (ACE_PSOS_PROVIDES_ERROR_SYMBOLS_TM)
#     define EWOULDBLOCK  EAGAIN   /* Blocking resource request would block */
#     define ENOMEM       12       /* Not enough core                       */
#   endif /* ! defined (ACE_PSOS_PROVIDES_ERROR_SYMBOLS_TM) */
#   define EACCES       13       /* Permission denied                     */
#   define EFAULT       14       /* Bad access                            */
#   if ! defined (ACE_PSOS_PROVIDES_ERROR_SYMBOLS_TM)
#     define EEXIST       17       /* File exists                           */
#   endif /* ! defined (ACE_PSOS_PROVIDES_ERROR_SYMBOLS_TM) */
#   define ENOSPC       28       /* No space left on device               */
#   if ! defined (ACE_PSOS_PROVIDES_ERROR_SYMBOLS_TM)
#     define EPIPE        32       /* Broken pipe                           */
#   endif /* ! defined (ACE_PSOS_PROVIDES_ERROR_SYMBOLS_TM) */
#   define ETIME        62       /* timer expired                         */
#   define ENAMETOOLONG 78       /* path name is too long                 */
#   define ENOSYS       89       /* Unsupported file system operation     */
#   if ! defined (ACE_PSOS_PROVIDES_ERROR_SYMBOLS_TM)
#     define EADDRINUSE   125      /* Address already in use                */
#     define ENETUNREACH  128      /* Network is unreachable                */
#     define EISCONN      133      /* Socket is already connected           */
#     define ESHUTDOWN    143      /* Can't send after socket shutdown      */
#     define ECONNREFUSED 146      /* Connection refused                    */
#     define EINPROGRESS  150      /* operation now in progress             */
#   endif /* ! defined (ACE_PSOS_PROVIDES_ERROR_SYMBOLS_TM) */
#   define ERRMAX       151      /* Last error number                     */

#   if defined (ACE_PSOSIM)

#     include /**/ <ace/sys_conf.h> /* system configuration file */
#     include /**/ <psos.h>         /* pSOS+ system calls                */
#     include /**/ <pna.h>          /* pNA+ TCP/IP Network Manager calls */

    /* In the *simulator* environment, use unsigned int for size_t */
#     define size_t  unsigned int


    /*   include <rpc.h>       pRPC+ Remote Procedure Call Library calls   */
    /*                         are not supported by pSOSim                 */
    /*                                                                     */
    /*   include <phile.h>     pHILE+ file system calls are not supported  */
    /*                         by pSOSim *so*, for the time being, we make */
    /*                         use of UNIX file system headers and then    */
    /*                         when we have time, we wrap UNIX file system */
    /*                         calls w/ pHILE+ wrappers, and modify ACE to */
    /*                         use the wrappers under pSOSim               */

    /* put includes for necessary UNIX file system calls here */
#     include /**/ <sys/stat.h>
#     include /**/ <sys/ioctl.h>
#     include /**/ <sys/sockio.h>
#     include /**/ <netinet/tcp.h>

#     define TCP_
#     if ! defined (BUFSIZ)
#       define BUFSIZ 1024
#     endif  /* ! defined (BUFSIZ) */


#   else

#     if defined (ACE_PSOS_CANT_USE_SYS_TYPES)
      // these are missing from the pSOS types.h file, and the compiler
      // supplied types.h file collides with the pSOS version
      typedef unsigned char u_char;
      typedef unsigned short        u_short;
      typedef unsigned int  u_int;
      typedef unsigned long u_long;
      typedef unsigned char uchar_t;
      typedef unsigned short        ushort_t;
      typedef unsigned int  uint_t;
      typedef unsigned long ulong_t;
      typedef char *caddr_t;
      typedef long pid_t;
//      typedef unsigned char wchar_t;
#     endif

#     include /**/ <ace/sys_conf.h> /* system configuration file */
#     include /**/ <configs.h>   /* includes all pSOS headers */
//    #include /**/ <psos.h>    /* pSOS system calls */
#     include /**/ <pna.h>      /* pNA+ TCP/IP Network Manager calls */
#     include /**/ <phile.h>     /* pHILE+ file system calls */
//    #include /**/ <prepccfg.h>     /* pREPC+ file system calls */

// These are colliding with the pSOS libraries
// # include /**/ <unistd.h>    /* Diab Data supplied file system calls */
// # include /**/ <sys/wait.h>    /* Diab Data supplied header file */

// This collides with phile.h
//    #include /**/ <sys/stat.h>    /* Diab Data supplied header file */

  // missing preprocessor definitions
#     define AF_UNIX 0x1
#     define PF_UNIX AF_UNIX
#     define PF_INET AF_INET
#     define AF_MAX AF_INET
#     define IFF_LOOPBACK IFF_EXTLOOPBACK

  typedef long fd_mask;
#     define IPPORT_RESERVED       1024
#     define IPPORT_USERRESERVED   5000

#     define howmany(x, y) (((x)+((y)-1))/(y))

  extern "C"
  {
    typedef void (* ACE_SignalHandler) (void);
    typedef void (* ACE_SignalHandlerV) (void);
  }

#     if !defined(SIG_DFL)
#       define SIG_DFL (ACE_SignalHandler) 0
#     endif  // philabs

#   endif /* defined (ACE_PSOSIM) */

// For general purpose portability

#   define ACE_BITS_PER_ULONG (8 * sizeof (u_long))

typedef u_long ACE_idtype_t;
typedef u_long ACE_id_t;
#   define ACE_SELF (0)
typedef u_long ACE_pri_t;

// pHILE+ calls the DIR struct XDIR instead
typedef XDIR DIR;

// Use pSOS semaphores, wrapped . . .
typedef struct
{
  u_long sema_;
  // Semaphore handle.  This is allocated by pSOS.

  char name_[4];
  // Name of the semaphore: really a 32 bit number to pSOS
} ACE_sema_t;

// Used for ACE_MMAP_Memory_Pool
#   if !defined (ACE_DEFAULT_BACKING_STORE)
#     define ACE_DEFAULT_BACKING_STORE "/tmp/ace-malloc-XXXXXX"
#   endif /* ACE_DEFAULT_BACKING_STORE */

// Used for ACE_FILE_Connector
#   if !defined (ACE_DEFAULT_TEMP_FILE)
#     define ACE_DEFAULT_TEMP_FILE "/tmp/ace-file-XXXXXX"
#   endif /* ACE_DEFAULT_TEMP_FILE */

// Used for logging
#   if !defined (ACE_DEFAULT_LOGFILE)
#     define ACE_DEFAULT_LOGFILE "/tmp/logfile"
#   endif /* ACE_DEFAULT_LOGFILE */

// Used for dynamic linking.
#   if !defined (ACE_DEFAULT_SVC_CONF)
#     define ACE_DEFAULT_SVC_CONF "./svc.conf"
#   endif /* ACE_DEFAULT_SVC_CONF */

#   if !defined (ACE_DEFAULT_SEM_KEY)
#     define ACE_DEFAULT_SEM_KEY 1234
#   endif /* ACE_DEFAULT_SEM_KEY */

#   define ACE_STDIN 0
#   define ACE_STDOUT 1
#   define ACE_STDERR 2

#   define ACE_DIRECTORY_SEPARATOR_STR_A "/"
#   define ACE_DIRECTORY_SEPARATOR_CHAR_A '/'

#   define ACE_DLL_SUFFIX ".so"
#   define ACE_DLL_PREFIX "lib"
#   define ACE_LD_SEARCH_PATH "LD_LIBRARY_PATH"
#   define ACE_LD_SEARCH_PATH_SEPARATOR_STR ":"
#   define ACE_LOGGER_KEY "/tmp/server_daemon"

#   define ACE_PLATFORM_A "pSOS"
#   define ACE_PLATFORM_EXE_SUFFIX_A ""

# if defined (ACE_HAS_MOSTLY_UNICODE_APIS)
#   define ASYS_WIDE_STRING(ASCII_STRING) ACE_WString (ASCII_STRING).fast_rep ()
# else
#   define ASYS_WIDE_STRING(ASCII_STRING) ASCII_STRING
# endif /* ACE_HAS_MOSTLY_UNICODE_APIS */

#   if defined (ACE_HAS_UNICODE)
#     define ACE_DIRECTORY_SEPARATOR_STR_W L"/"
#     define ACE_DIRECTORY_SEPARATOR_CHAR_W L'/'
#     define ACE_PLATFORM_W L"pSOS"
#     define ACE_PLATFORM_EXE_SUFFIX_W L""
#   else
#     define ACE_DIRECTORY_SEPARATOR_STR_W "/"
#     define ACE_DIRECTORY_SEPARATOR_CHAR_W '/'
#     define ACE_PLATFORM_W "pSOS"
#     define ACE_PLATFORM_EXE_SUFFIX_W ""
#   endif /* ACE_HAS_UNICODE */

#   define ACE_MAX_DEFAULT_PORT 65535

#   if ! defined(MAXPATHLEN)
#     define MAXPATHLEN  1024
#   endif /* MAXPATHLEN */

#   if ! defined(MAXNAMLEN)
#     define MAXNAMLEN   255
#   endif /* MAXNAMLEN */

#   if defined (ACE_LACKS_MMAP)
#     define PROT_READ 0
#     define PROT_WRITE 0
#     define PROT_EXEC 0
#     define PROT_NONE 0
#     define PROT_RDWR 0
#     define MAP_PRIVATE 0
#     define MAP_SHARED 0
#     define MAP_FIXED 0
#   endif /* ACE_LACKS_MMAP */

// The following 3 defines are used by the ACE Name Server...
#   if !defined (ACE_DEFAULT_NAMESPACE_DIR_A)
#     define ACE_DEFAULT_NAMESPACE_DIR_A "/tmp"
#   endif /* ACE_DEFAULT_NAMESPACE_DIR_A */

#   if !defined (ACE_DEFAULT_LOCALNAME_A)
#     define ACE_DEFAULT_LOCALNAME_A "localnames"
#   endif /* ACE_DEFAULT_LOCALNAME_A */

#   if !defined (ACE_DEFAULT_GLOBALNAME_A)
#     define ACE_DEFAULT_GLOBALNAME_A "globalnames"
#   endif /* ACE_DEFAULT_GLOBALNAME_A */

#   if defined (ACE_HAS_UNICODE)
#     if !defined (ACE_DEFAULT_NAMESPACE_DIR_W)
#       define ACE_DEFAULT_NAMESPACE_DIR_W L"/tmp"
#     endif /* ACE_DEFAULT_NAMESPACE_DIR_W */
#     if !defined (ACE_DEFAULT_LOCALNAME_W)
#       define ACE_DEFAULT_LOCALNAME_W L"localnames"
#     endif /* ACE_DEFAULT_LOCALNAME_W */
#     if !defined (ACE_DEFAULT_GLOBALNAME_W)
#       define ACE_DEFAULT_GLOBALNAME_W L"globalnames"
#     endif /* ACE_DEFAULT_GLOBALNAME_W */
#   else
#     if !defined (ACE_DEFAULT_NAMESPACE_DIR_W)
#       define ACE_DEFAULT_NAMESPACE_DIR_W "/tmp"
#     endif /* ACE_DEFAULT_NAMESPACE_DIR_W */
#     if !defined (ACE_DEFAULT_LOCALNAME_W)
#       define ACE_DEFAULT_LOCALNAME_W "localnames"
#     endif /* ACE_DEFAULT_LOCALNAME_W */
#     if !defined (ACE_DEFAULT_GLOBALNAME_W)
#       define ACE_DEFAULT_GLOBALNAME_W "globalnames"
#     endif /* ACE_DEFAULT_GLOBALNAME_W */
#   endif /* ACE_HAS_UNICODE */


typedef int ACE_HANDLE;
typedef ACE_HANDLE ACE_SOCKET;
#   define ACE_INVALID_HANDLE -1

typedef ACE_HANDLE ACE_SHLIB_HANDLE;
const ACE_SHLIB_HANDLE ACE_SHLIB_INVALID_HANDLE = ACE_INVALID_HANDLE;
const int ACE_DEFAULT_SHLIB_MODE = 0;

#   define ACE_INVALID_SEM_KEY -1

struct  hostent {
  char    *h_name;        /* official name of host */
  char    **h_aliases;    /* alias list */
  int     h_addrtype;     /* host address type */
  int     h_length;       /* address length */
  char    **h_addr_list;  /* (first, only) address from name server */
#   define h_addr h_addr_list[0]   /* the first address */
};

struct  servent {
  char     *s_name;    /* official service name */
  char    **s_aliases; /* alias list */
  int       s_port;    /* port # */
  char     *s_proto;   /* protocol to use */
};

// For Win32 compatibility.

typedef const char *LPCTSTR;
typedef char *LPTSTR;
typedef char TCHAR;

#   define ACE_SEH_TRY if (1)
#   define ACE_SEH_EXCEPT(X) while (0)
#   define ACE_SEH_FINALLY if (1)

#   if !defined (LPSECURITY_ATTRIBUTES)
#     define LPSECURITY_ATTRIBUTES int
#   endif /* !defined LPSECURITY_ATTRIBUTES */
#   if !defined (GENERIC_READ)
#     define GENERIC_READ 0
#   endif /* !defined GENERIC_READ */
#   if !defined (FILE_SHARE_READ)
#     define FILE_SHARE_READ 0
#   endif /* !defined FILE_SHARE_READ */
#   if !defined (OPEN_EXISTING)
#     define OPEN_EXISTING 0
#   endif /* !defined OPEN_EXISTING */
#   if !defined (FILE_ATTRIBUTE_NORMAL)
#     define FILE_ATTRIBUTE_NORMAL 0
#   endif /* !defined FILE_ATTRIBUTE_NORMAL */
#   if !defined (MAXIMUM_WAIT_OBJECTS)
#     define MAXIMUM_WAIT_OBJECTS 0
#   endif /* !defined MAXIMUM_WAIT_OBJECTS */
#   if !defined (FILE_FLAG_OVERLAPPED)
#     define FILE_FLAG_OVERLAPPED 0
#   endif /* !defined FILE_FLAG_OVERLAPPED */
#   if !defined (FILE_FLAG_SEQUENTIAL_SCAN)
#     define FILE_FLAG_SEQUENTIAL_SCAN 0
#   endif /* !defined FILE_FLAG_SEQUENTIAL_SCAN */

struct ACE_OVERLAPPED
{
  u_long Internal;
  u_long InternalHigh;
  u_long Offset;
  u_long OffsetHigh;
  ACE_HANDLE hEvent;
};

#   if !defined(USER_INCLUDE_SYS_TIME_TM)
typedef struct timespec
{
  time_t tv_sec; // Seconds
  long tv_nsec; // Nanoseconds
} timespec_t;
#   endif

// Use pSOS time, wrapped . . .
class ACE_Export ACE_PSOS_Time_t
{
public:

  ACE_PSOS_Time_t ();
    // default ctor: date, time, and ticks all zeroed

  ACE_PSOS_Time_t (const timespec_t& t);
    // ctor from a timespec_t

  operator timespec_t ();
    // type cast operator (to a timespec_t)

  static u_long get_system_time (ACE_PSOS_Time_t& t);
    // static member function to get current system time

  static u_long set_system_time (const ACE_PSOS_Time_t& t);
    // static member function to set current system time

#   if defined (ACE_PSOSIM)

  static u_long init_simulator_time ();
    // static member function to initialize system time, using UNIX calls

#   endif /* ACE_PSOSIM */

  static const u_long max_ticks;
    // max number of ticks supported in a single system call

private:

  // constants for prying info out of the pSOS time encoding
  static const u_long year_mask;
  static const u_long month_mask;
  static const u_long day_mask;
  static const u_long hour_mask;
  static const u_long minute_mask;
  static const u_long second_mask;
  static const int year_shift;
  static const int month_shift;
  static const int hour_shift;
  static const int minute_shift;
  static const int year_origin;
  static const int month_origin;

  // error codes
  static const u_long err_notime;   // system time not set
  static const u_long err_illdate;  // date out of range
  static const u_long err_illtime;  // time out of range
  static const u_long err_illticks; // ticks out of range

   u_long date_;
  // date : year in bits 31-16, month in bits 15-8, day in bits 7-0

  u_long time_;
  // time : hour in bits 31-16, minutes in bits 15-8, seconds in bits 7-0

  u_long ticks_;
  // ticks: number of system clock ticks (KC_TICKS2SEC-1 max)
} ;

# endif /* defined (ACE_PSOS) */

# if defined (ACE_HAS_CHARPTR_SPRINTF)
#   define ACE_SPRINTF_ADAPTER(X) ::strlen (X)
# else
#   define ACE_SPRINTF_ADAPTER(X) X
# endif /* ACE_HAS_CHARPTR_SPRINTF */

# if defined (__ACE_INLINE__)
#   define ACE_INLINE inline
#   if !defined (ACE_HAS_INLINED_OSCALLS)
#     define ACE_HAS_INLINED_OSCALLS
#   endif /* !ACE_HAS_INLINED_OSCALLS */
# else
#   define ACE_INLINE
# endif /* __ACE_INLINE__ */

// Default address for shared memory mapped files and SYSV shared memory
// (defaults to 64 M).
# if !defined (ACE_DEFAULT_BASE_ADDR)
#   define ACE_DEFAULT_BASE_ADDR ((char *) (64 * 1024 * 1024))
# endif /* ACE_DEFAULT_BASE_ADDR */

// This fudge factor can be overriden for timers that need it, such as on
// Solaris, by defining the ACE_TIMER_SKEW symbol in the appropriate config
// header.
# if !defined (ACE_TIMER_SKEW)
#   define ACE_TIMER_SKEW 0
# endif /* ACE_TIMER_SKEW */

// This needs to go here *first* to avoid problems with AIX.
# if defined (ACE_HAS_PTHREADS)
extern "C" {
#   include /**/ <pthread.h>
#   if defined (DIGITAL_UNIX)
#     define pthread_self __pthread_self
extern "C" pthread_t pthread_self (void);
#   endif /* DIGITAL_UNIX */
}
# endif /* ACE_HAS_PTHREADS */

// There are a lot of threads-related macro definitions in the config files.
// They came in at different times and from different places and platform
// requirements as threads evolved.  They are probably not all needed - some
// overlap or are otherwise confused.  This is an attempt to start
// straightening them out.
# if defined (ACE_HAS_PTHREADS_STD)    /* POSIX.1c threads (pthreads) */
  // POSIX.1c threads implies pthread_sigmask()
#   if !defined (ACE_HAS_PTHREAD_SIGMASK)
#     define ACE_HAS_PTHREAD_SIGMASK
#   endif /* ! ACE_HAS_PTHREAD_SIGMASK */
  // ... and 2-parameter asctime_r and ctime_r
#   if !defined (ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R) && !defined (ACE_HAS_STHREADS)
#     define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
#   endif
# endif /* ACE_HAS_PTHREADS_STD */

# if (ACE_NTRACE == 1)
#   define ACE_TRACE(X)
# else
#   define ACE_TRACE(X) ACE_Trace ____ (ASYS_TEXT (X), __LINE__, ASYS_TEXT (__FILE__))
# endif /* ACE_NTRACE */

# if !defined (ACE_HAS_WINCE)
#   include /**/ <time.h>
#   if defined (__Lynx__)
#     include /**/ <st.h>
#     include /**/ <sem.h>
#   endif /* __Lynx__ */
# endif /* ACE_HAS_WINCE */

# if defined (ACE_LACKS_SYSTIME_H)
// Some platforms may need to include this, but I suspect that most
// will get it from <time.h>
#   if defined (VXWORKS)
#     include /**/ <sys/times.h>
#   else
#     include /**/ <sys/time.h>
#   endif /* VXWORKS */
# endif /* ACE_LACKS_SYSTIME_H */

# if !defined (ACE_HAS_POSIX_TIME) && !defined (ACE_PSOS)
// Definition per POSIX.
typedef struct timespec
{
  time_t tv_sec; // Seconds
  long tv_nsec; // Nanoseconds
} timespec_t;
# elif defined (ACE_HAS_BROKEN_POSIX_TIME)
// OSF/1 defines struct timespec in <sys/timers.h> - Tom Marrs
#   include /**/ <sys/timers.h>
# endif /* !ACE_HAS_POSIX_TIME */

# if defined(ACE_LACKS_TIMESPEC_T)
typedef struct timespec timespec_t;
# endif /* ACE_LACKS_TIMESPEC_T */

# if !defined (ACE_HAS_CLOCK_GETTIME) && !defined (_CLOCKID_T)
typedef int clockid_t;
#   if !defined (CLOCK_REALTIME)
#     define CLOCK_REALTIME 0
#   endif /* CLOCK_REALTIME */
# endif /* ! ACE_HAS_CLOCK_GETTIME && ! _CLOCKID_T */

// -------------------------------------------------------------------
// These forward declarations are only used to circumvent a bug in
// MSVC 6.0 compiler.  They shouldn't cause any problem for other
// compilers and they can be removed once MS release a SP that contains
// the fix.
class ACE_Time_Value;
ACE_Export ACE_Time_Value operator + (const ACE_Time_Value &tv1,
                                      const ACE_Time_Value &tv2);

ACE_Export ACE_Time_Value operator - (const ACE_Time_Value &tv1,
                                      const ACE_Time_Value &tv2);
// -------------------------------------------------------------------

class ACE_Export ACE_Time_Value
{
  // = TITLE
  //     Operations on "timeval" structures.
  //
  // = DESCRIPTION
  //     This class centralizes all the time related processing in
  //     ACE.  These timers are typically used in conjunction with OS
  //     mechanisms like <select>, <poll>, or <cond_timedwait>.
  //     <ACE_Time_Value> makes the use of these mechanisms portable
  //     across OS platforms,
public:
  // = Useful constants.

  static const ACE_Time_Value zero;
  // Constant "0".

  static const ACE_Time_Value max_time;
  // Constant for maximum time representable.

  // = Initialization methods.

  ACE_Time_Value (void);
  // Default Constructor.

  ACE_Time_Value (long sec, long usec = 0);
  // Constructor.

  // = Methods for converting to/from various time formats.
  ACE_Time_Value (const struct timeval &t);
  // Construct the <ACE_Time_Value> from a <timeval>.

  ACE_Time_Value (const timespec_t &t);
  //  Initializes the <ACE_Time_Value> object from a <timespec_t>.

  ACE_Time_Value (const ACE_Time_Value &tv);
  // Copy constructor.

# if defined (ACE_WIN32)
  ACE_Time_Value (const FILETIME &ft);
  //  Initializes the ACE_Time_Value object from a Win32 FILETIME
# endif /* ACE_WIN32 */

  void set (long sec, long usec);
  // Construct a <Time_Value> from two <long>s.

  void set (double d);
  // Construct a <Time_Value> from a <double>, which is assumed to be
  // in second format, with any remainder treated as microseconds.

  void set (const timeval &t);
  // Construct a <Time_Value> from a <timeval>.

  void set (const timespec_t &t);
  // Initializes the <Time_Value> object from a <timespec_t>.

# if defined (ACE_WIN32)
  void set (const FILETIME &ft);
  //  Initializes the <Time_Value> object from a <timespec_t>.
# endif /* ACE_WIN32 */

  long msec (void) const;
  // Converts from <Time_Value> format into milli-seconds format.

  void msec (long);
  // Converts from milli-seconds format into <Time_Value> format.

  operator timespec_t () const;
  // Returns the value of the object as a <timespec_t>.

  operator timeval () const;
  // Returns the value of the object as a <timeval>.

  operator const timeval *() const;
  // Returns a pointer to the object as a <timeval>.

# if defined (ACE_WIN32)
  operator FILETIME () const;
  // Returns the value of the object as a Win32 FILETIME.
# endif /* ACE_WIN32 */

  // = The following are accessor/mutator methods.

  long sec (void) const;
  // Get seconds.

  void sec (long sec);
  // Set seconds.

  long usec (void) const;
  // Get microseconds.

  void usec (long usec);
  // Set microseconds.

  // = The following are arithmetic methods for operating on
  // Time_Values.

  void operator += (const ACE_Time_Value &tv);
  // Add <tv> to this.

  void operator -= (const ACE_Time_Value &tv);
  // Subtract <tv> to this.

  friend ACE_Export ACE_Time_Value operator + (const ACE_Time_Value &tv1,
                                               const ACE_Time_Value &tv2);
  // Adds two ACE_Time_Value objects together, returns the sum.

  friend ACE_Export ACE_Time_Value operator - (const ACE_Time_Value &tv1,
                                               const ACE_Time_Value &tv2);
  // Subtracts two ACE_Time_Value objects, returns the difference.

  friend ACE_Export int operator < (const ACE_Time_Value &tv1,
                                    const ACE_Time_Value &tv2);
  // True if tv1 < tv2.

  friend ACE_Export int operator > (const ACE_Time_Value &tv1,
                                    const ACE_Time_Value &tv2);
  // True if tv1 > tv2.

  friend ACE_Export int operator <= (const ACE_Time_Value &tv1,
                                     const ACE_Time_Value &tv2);
  // True if tv1 <= tv2.

  friend ACE_Export int operator >= (const ACE_Time_Value &tv1,
                                     const ACE_Time_Value &tv2);
  // True if tv1 >= tv2.

  friend ACE_Export int operator == (const ACE_Time_Value &tv1,
                                     const ACE_Time_Value &tv2);
  // True if tv1 == tv2.

  friend ACE_Export int operator != (const ACE_Time_Value &tv1,
                                     const ACE_Time_Value &tv2);
  // True if tv1 != tv2.

  void dump (void) const;
  // Dump the state of an object.

# if defined (ACE_WIN32)
  static const DWORDLONG FILETIME_to_timval_skew;
  // Const time difference between FILETIME and POSIX time.
# endif /* ACE_WIN32 */

private:
  void normalize (void);
  // Put the timevalue into a canonical form.

  timeval tv_;
  // Store the values as a <timeval>.
};

class ACE_Export ACE_Countdown_Time
{
  // = TITLE
  //     Keeps track of the amount of elapsed time.
  //
  // = DESCRIPTION
  //     This class has a side-effect on the <max_wait_time> -- every
  //     time the <stop> method is called the <max_wait_time> is
  //     updated.
public:
  // = Initialization and termination methods.
  ACE_Countdown_Time (ACE_Time_Value *max_wait_time);
  // Cache the <max_wait_time> and call <start>.

  ~ACE_Countdown_Time (void);
  // Call <stop>.

  int start (void);
  // Cache the current time and enter a start state.

  int stop (void);
  // Subtract the elapsed time from max_wait_time_ and enter a stopped
  // state.

  int update (void);
  // Calls stop and then start.  max_wait_time_ is modified by the
  // call to stop.

private:
  ACE_Time_Value *max_wait_time_;
  // Maximum time we were willing to wait.

  ACE_Time_Value start_time_;
  // Beginning of the start time.

  int stopped_;
  // Keeps track of whether we've already been stopped.
};

# if defined (ACE_HAS_USING_KEYWORD)
#   define ACE_USING using
# else
#   define ACE_USING
# endif /* ACE_HAS_USING_KEYWORD */

# if defined (ACE_HAS_TYPENAME_KEYWORD)
#   define ACE_TYPENAME typename
# else
#   define ACE_TYPENAME
# endif /* ACE_HAS_TYPENAME_KEYWORD */

// The following is necessary since many C++ compilers don't support
// typedef'd types inside of classes used as formal template
// arguments... ;-(.  Luckily, using the C++ preprocessor I can hide
// most of this nastiness!

# if defined (ACE_HAS_TEMPLATE_TYPEDEFS)

// Handle ACE_Message_Queue.
#   define ACE_SYNCH_DECL class _ACE_SYNCH
#   define ACE_SYNCH_USE _ACE_SYNCH
#   define ACE_SYNCH_MUTEX_T ACE_TYPENAME _ACE_SYNCH::MUTEX
#   define ACE_SYNCH_CONDITION_T ACE_TYPENAME _ACE_SYNCH::CONDITION
#   define ACE_SYNCH_SEMAPHORE_T ACE_TYPENAME _ACE_SYNCH::SEMAPHORE

// Handle ACE_Malloc*
#   define ACE_MEM_POOL_1 class _ACE_MEM_POOL
#   define ACE_MEM_POOL_2 _ACE_MEM_POOL
#   define ACE_MEM_POOL _ACE_MEM_POOL
#   define ACE_MEM_POOL_OPTIONS ACE_TYPENAME _ACE_MEM_POOL::OPTIONS

// Handle ACE_Svc_Handler
#   define ACE_PEER_STREAM_1 class _ACE_PEER_STREAM
#   define ACE_PEER_STREAM_2 _ACE_PEER_STREAM
#   define ACE_PEER_STREAM _ACE_PEER_STREAM
#   define ACE_PEER_STREAM_ADDR ACE_TYPENAME _ACE_PEER_STREAM::PEER_ADDR

// Handle ACE_Acceptor
#   define ACE_PEER_ACCEPTOR_1 class _ACE_PEER_ACCEPTOR
#   define ACE_PEER_ACCEPTOR_2 _ACE_PEER_ACCEPTOR
#   define ACE_PEER_ACCEPTOR _ACE_PEER_ACCEPTOR
#   define ACE_PEER_ACCEPTOR_ADDR ACE_TYPENAME _ACE_PEER_ACCEPTOR::PEER_ADDR

// Handle ACE_Connector
#   define ACE_PEER_CONNECTOR_1 class _ACE_PEER_CONNECTOR
#   define ACE_PEER_CONNECTOR_2 _ACE_PEER_CONNECTOR
#   define ACE_PEER_CONNECTOR _ACE_PEER_CONNECTOR
#   define ACE_PEER_CONNECTOR_ADDR ACE_TYPENAME _ACE_PEER_CONNECTOR::PEER_ADDR
#   if !defined(ACE_HAS_TYPENAME_KEYWORD)
#     define ACE_PEER_CONNECTOR_ADDR_ANY ACE_PEER_CONNECTOR_ADDR::sap_any
#   else
    //
    // If the compiler supports 'typename' we cannot use
    //
    // PEER_CONNECTOR::PEER_ADDR::sap_any
    //
    // because PEER_CONNECTOR::PEER_ADDR is not considered a type. But:
    //
    // typename PEER_CONNECTOR::PEER_ADDR::sap_any
    //
    // will not work either, because now we are declaring sap_any a
    // type, further:
    //
    // (typename PEER_CONNECTOR::PEER_ADDR)::sap_any
    //
    // is considered a casting expression. All I can think of is using a
    // typedef, I tried PEER_ADDR but that was a source of trouble on
    // some platforms. I will try:
    //
#     define ACE_PEER_CONNECTOR_ADDR_ANY ACE_PEER_ADDR_TYPEDEF::sap_any
#   endif /* ACE_HAS_TYPENAME_KEYWORD */

// Handle ACE_SOCK_*
#   define ACE_SOCK_ACCEPTOR ACE_SOCK_Acceptor
#   define ACE_SOCK_CONNECTOR ACE_SOCK_Connector
#   define ACE_SOCK_STREAM ACE_SOCK_Stream

// Handle ACE_LSOCK_*
#   define ACE_LSOCK_ACCEPTOR ACE_LSOCK_Acceptor
#   define ACE_LSOCK_CONNECTOR ACE_LSOCK_Connector
#   define ACE_LSOCK_STREAM ACE_LSOCK_Stream

// Handle ACE_TLI_*
#   define ACE_TLI_ACCEPTOR ACE_TLI_Acceptor
#   define ACE_TLI_CONNECTOR ACE_TLI_Connector
#   define ACE_TLI_STREAM ACE_TLI_Stream

// Handle ACE_SPIPE_*
#   define ACE_SPIPE_ACCEPTOR ACE_SPIPE_Acceptor
#   define ACE_SPIPE_CONNECTOR ACE_SPIPE_Connector
#   define ACE_SPIPE_STREAM ACE_SPIPE_Stream

// Handle ACE_UPIPE_*
#   define ACE_UPIPE_ACCEPTOR ACE_UPIPE_Acceptor
#   define ACE_UPIPE_CONNECTOR ACE_UPIPE_Connector
#   define ACE_UPIPE_STREAM ACE_UPIPE_Stream

// Handle ACE_FILE_*
#   define ACE_FILE_CONNECTOR ACE_FILE_Connector
#   define ACE_FILE_STREAM ACE_FILE_IO

// Handle ACE_*_Memory_Pool.
#   define ACE_MMAP_MEMORY_POOL ACE_MMAP_Memory_Pool
#   define ACE_LITE_MMAP_MEMORY_POOL ACE_Lite_MMAP_Memory_Pool
#   define ACE_SBRK_MEMORY_POOL ACE_Sbrk_Memory_Pool
#   define ACE_SHARED_MEMORY_POOL ACE_Shared_Memory_Pool
#   define ACE_LOCAL_MEMORY_POOL ACE_Local_Memory_Pool

# else /* TEMPLATES are broken in some form or another (i.e., most C++ compilers) */

// Handle ACE_Message_Queue.
#   if defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
#     define ACE_SYNCH_DECL class _ACE_SYNCH_MUTEX_T, class _ACE_SYNCH_CONDITION_T, class _ACE_SYNCH_SEMAPHORE_T
#     define ACE_SYNCH_USE _ACE_SYNCH_MUTEX_T, _ACE_SYNCH_CONDITION_T, _ACE_SYNCH_SEMAPHORE_T
#   else
#     define ACE_SYNCH_DECL class _ACE_SYNCH_MUTEX_T, class _ACE_SYNCH_CONDITION_T
#     define ACE_SYNCH_USE _ACE_SYNCH_MUTEX_T, _ACE_SYNCH_CONDITION_T
#   endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */
#   define ACE_SYNCH_MUTEX_T _ACE_SYNCH_MUTEX_T
#   define ACE_SYNCH_CONDITION_T _ACE_SYNCH_CONDITION_T
#   define ACE_SYNCH_SEMAPHORE_T _ACE_SYNCH_SEMAPHORE_T

// Handle ACE_Malloc*
#   define ACE_MEM_POOL_1 class _ACE_MEM_POOL, class _ACE_MEM_POOL_OPTIONS
#   define ACE_MEM_POOL_2 _ACE_MEM_POOL, _ACE_MEM_POOL_OPTIONS
#   define ACE_MEM_POOL _ACE_MEM_POOL
#   define ACE_MEM_POOL_OPTIONS _ACE_MEM_POOL_OPTIONS

// Handle ACE_Svc_Handler
#   define ACE_PEER_STREAM_1 class _ACE_PEER_STREAM, class _ACE_PEER_ADDR
#   define ACE_PEER_STREAM_2 _ACE_PEER_STREAM, _ACE_PEER_ADDR
#   define ACE_PEER_STREAM _ACE_PEER_STREAM
#   define ACE_PEER_STREAM_ADDR _ACE_PEER_ADDR

// Handle ACE_Acceptor
#   define ACE_PEER_ACCEPTOR_1 class _ACE_PEER_ACCEPTOR, class _ACE_PEER_ADDR
#   define ACE_PEER_ACCEPTOR_2 _ACE_PEER_ACCEPTOR, _ACE_PEER_ADDR
#   define ACE_PEER_ACCEPTOR _ACE_PEER_ACCEPTOR
#   define ACE_PEER_ACCEPTOR_ADDR _ACE_PEER_ADDR

// Handle ACE_Connector
#   define ACE_PEER_CONNECTOR_1 class _ACE_PEER_CONNECTOR, class _ACE_PEER_ADDR
#   define ACE_PEER_CONNECTOR_2 _ACE_PEER_CONNECTOR, _ACE_PEER_ADDR
#   define ACE_PEER_CONNECTOR _ACE_PEER_CONNECTOR
#   define ACE_PEER_CONNECTOR_ADDR _ACE_PEER_ADDR
#   define ACE_PEER_CONNECTOR_ADDR_ANY ACE_PEER_CONNECTOR_ADDR::sap_any

// Handle ACE_SOCK_*
#   define ACE_SOCK_ACCEPTOR ACE_SOCK_Acceptor, ACE_INET_Addr
#   define ACE_SOCK_CONNECTOR ACE_SOCK_Connector, ACE_INET_Addr
#   define ACE_SOCK_STREAM ACE_SOCK_Stream, ACE_INET_Addr

// Handle ACE_LSOCK_*
#   define ACE_LSOCK_ACCEPTOR ACE_LSOCK_Acceptor, ACE_UNIX_Addr
#   define ACE_LSOCK_CONNECTOR ACE_LSOCK_Connector, ACE_UNIX_Addr
#   define ACE_LSOCK_STREAM ACE_LSOCK_Stream, ACE_UNIX_Addr

// Handle ACE_TLI_*
#   define ACE_TLI_ACCEPTOR ACE_TLI_Acceptor, ACE_INET_Addr
#   define ACE_TLI_CONNECTOR ACE_TLI_Connector, ACE_INET_Addr
#   define ACE_TLI_STREAM ACE_TLI_Stream, ACE_INET_Addr

// Handle ACE_SPIPE_*
#   define ACE_SPIPE_ACCEPTOR ACE_SPIPE_Acceptor, ACE_SPIPE_Addr
#   define ACE_SPIPE_CONNECTOR ACE_SPIPE_Connector, ACE_SPIPE_Addr
#   define ACE_SPIPE_STREAM ACE_SPIPE_Stream, ACE_SPIPE_Addr

// Handle ACE_UPIPE_*
#   define ACE_UPIPE_ACCEPTOR ACE_UPIPE_Acceptor, ACE_SPIPE_Addr
#   define ACE_UPIPE_CONNECTOR ACE_UPIPE_Connector, ACE_SPIPE_Addr
#   define ACE_UPIPE_STREAM ACE_UPIPE_Stream, ACE_SPIPE_Addr

// Handle ACE_FILE_*
#   define ACE_FILE_CONNECTOR ACE_FILE_Connector, ACE_FILE_Addr
#   define ACE_FILE_STREAM ACE_FILE_IO, ACE_FILE_Addr

// Handle ACE_*_Memory_Pool.
#   define ACE_MMAP_MEMORY_POOL ACE_MMAP_Memory_Pool, ACE_MMAP_Memory_Pool_Options
#   define ACE_LITE_MMAP_MEMORY_POOL ACE_Lite_MMAP_Memory_Pool, ACE_MMAP_Memory_Pool_Options
#   define ACE_SBRK_MEMORY_POOL ACE_Sbrk_Memory_Pool, ACE_Sbrk_Memory_Pool_Options
#   define ACE_SHARED_MEMORY_POOL ACE_Shared_Memory_Pool, ACE_Shared_Memory_Pool_Options
#   define ACE_LOCAL_MEMORY_POOL ACE_Local_Memory_Pool, ACE_Local_Memory_Pool_Options

# endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

// These two are only for backward compatibility. You should avoid
// using them if not necessary.
# define ACE_SYNCH_1 ACE_SYNCH_DECL
# define ACE_SYNCH_2 ACE_SYNCH_USE

// For Win32 compatibility...
# if !defined (ACE_WSOCK_VERSION)
#   define ACE_WSOCK_VERSION 0, 0
# endif /* ACE_WSOCK_VERSION */

# if defined (ACE_HAS_BROKEN_CTIME)
#   undef ctime
# endif /* ACE_HAS_BROKEN_CTIME */

extern "C" {
typedef void (*ACE_Service_Object_Exterminator)(void *);
}

// Static service macros
# define ACE_STATIC_SVC_DECLARE(X) extern ACE_Static_Svc_Descriptor ace_svc_desc_##X ;
# define ACE_STATIC_SVC_DEFINE(X, NAME, TYPE, FN, FLAGS, ACTIVE) \
ACE_Static_Svc_Descriptor ace_svc_desc_##X = { NAME, TYPE, FN, FLAGS, ACTIVE };
# define ACE_STATIC_SVC_REQUIRE(X)\
class ACE_Static_Svc_##X {\
public:\
        ACE_Static_Svc_##X() { ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_##X); }\
};\
static ACE_Static_Svc_##X ace_static_svc_##X;


// More generic dynamic/static service macros.
# define ACE_FACTORY_DECLARE(CLS,X) extern "C" CLS##_Export ACE_Service_Object *_make_##X (ACE_Service_Object_Exterminator *);
# define ACE_FACTORY_DEFINE(CLS,X) \
extern "C" void _gobble_##X (void *p) { X *_p = (X *) p; delete _p; } \
extern "C" ACE_Service_Object *_make_##X (ACE_Service_Object_Exterminator *gobbler) \
{ ACE_TRACE (#X); \
if (gobbler != 0) *gobbler = (ACE_Service_Object_Exterminator) _gobble_##X; return new X; }

// Dynamic/static service macros.
# define ACE_SVC_FACTORY_DECLARE(X) ACE_FACTORY_DECLARE (ACE_Svc, X)
# define ACE_SVC_INVOKE(X) _make_##X (0)
# define ACE_SVC_NAME(X) _make_##X
# define ACE_SVC_FACTORY_DEFINE(X) ACE_FACTORY_DEFINE (ACE_Svc, X)

# if defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION))
#   define ACE_TSS_TYPE(T) ACE_TSS< T >
#   if defined (ACE_HAS_BROKEN_CONVERSIONS)
#     define ACE_TSS_GET(I, T) (*(I))
#   else
#     define ACE_TSS_GET(I, T) ((I)->operator T * ())
#   endif /* ACE_HAS_BROKEN_CONVERSIONS */
# else
#   define ACE_TSS_TYPE(T) T
#   define ACE_TSS_GET(I, T) (I)
# endif /* ACE_HAS_THREADS && (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATIOND) */

# if defined (ACE_LACKS_MODE_MASKS)
// MODE MASKS

// the following macros are for POSIX conformance.

#   define S_IRWXU 00700           /* read, write, execute: owner. */
#   define S_IRUSR 00400           /* read permission: owner. */
#   define S_IWUSR 00200           /* write permission: owner. */
#   define S_IXUSR 00100           /* execute permission: owner. */
#   define S_IRWXG 00070           /* read, write, execute: group. */
#   define S_IRGRP 00040           /* read permission: group. */
#   define S_IWGRP 00020           /* write permission: group. */
#   define S_IXGRP 00010           /* execute permission: group. */
#   define S_IRWXO 00007           /* read, write, execute: other. */
#   define S_IROTH 00004           /* read permission: other. */
#   define S_IWOTH 00002           /* write permission: other. */
#   define S_IXOTH 00001           /* execute permission: other. */

# endif /* ACE_LACKS_MODE_MASKS */

# if defined (ACE_LACKS_SEMBUF_T)
struct sembuf
{
  unsigned short sem_num; // semaphore #
  short sem_op; // semaphore operation
  short sem_flg; // operation flags
};
# endif /* ACE_LACKS_SEMBUF_T */

# if defined (ACE_HAS_H_ERRNO)
void herror (const char *str);
# endif /* ACE_HAS_H_ERRNO */

# if defined (ACE_LACKS_UALARM_PROTOTYPE)
extern "C" u_int ualarm (u_int usecs, u_int interval);
# endif /* ACE_LACKS_UALARM_PROTOTYPE */

# if defined (ACE_LACKS_MSGBUF_T)
struct msgbuf {};
# endif /* ACE_LACKS_MSGBUF_T */

# if defined (ACE_LACKS_STRRECVFD)
struct strrecvfd {};
# endif /* ACE_LACKS_STRRECVFD */

# if defined (ACE_HAS_PROC_FS)
#   include /**/ <sys/procfs.h>
# endif /* ACE_HAS_PROC_FS */

# if defined (ACE_HAS_UNICODE)
#   if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
#     include /**/ <cwchar>
#   elif !defined (__BORLANDC__) && !defined (ACE_HAS_WINCE) /* ACE_HAS_STANDARD_CPP_LIBRARY */
#     include /**/ <wchar.h>
#   endif /* ACE_HAS_STANDARD_CPP_LIBRARY */
# elif defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)
#   include /**/ <wchar.h>
# elif defined (ACE_LACKS_WCHAR_T)
typedef ACE_UINT32 wchar_t;
# endif /* ACE_HAS_UNICODE */

# if defined (ACE_HAS_BROKEN_WRITEV)
typedef struct iovec ACE_WRITEV_TYPE;
# else
typedef const struct iovec ACE_WRITEV_TYPE;
# endif /* ACE_HAS_BROKEN_WRITEV */

# if defined (ACE_HAS_BROKEN_READV)
typedef const struct iovec ACE_READV_TYPE;
# else
typedef struct iovec ACE_READV_TYPE;
# endif /* ACE_HAS_BROKEN_READV */

# if defined (ACE_HAS_BROKEN_SETRLIMIT)
typedef struct rlimit ACE_SETRLIMIT_TYPE;
# else
typedef const struct rlimit ACE_SETRLIMIT_TYPE;
# endif /* ACE_HAS_BROKEN_SETRLIMIT */

# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#   define ACE_MT(X) X
#   if !defined (_REENTRANT)
#     define _REENTRANT
#   endif /* _REENTRANT */
# else
#   define ACE_MT(X)
# endif /* ACE_MT_SAFE */

# if !defined (ACE_DEFAULT_THREAD_PRIORITY)
#   define ACE_DEFAULT_THREAD_PRIORITY (-0x7fffffffL - 1L)
# endif /* ACE_DEFAULT_THREAD_PRIORITY */

// Convenient macro for testing for deadlock, as well as for detecting
// when mutexes fail.
# define ACE_GUARD(MUTEX,OBJ,LOCK) \
  ACE_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) return;
# define ACE_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN) \
  ACE_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) return RETURN;
# define ACE_GUARD_THROW(MUTEX,OBJ,LOCK,EXCEPTION) \
  ACE_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) TAO_THROW (EXCEPTION);
# define ACE_GUARD_THROW_RETURN(MUTEX,OBJ,LOCK,EXCEPTION,RETURN) \
  ACE_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) TAO_THROW_RETURN (EXCEPTION, RETURN);
# define ACE_WRITE_GUARD(MUTEX,OBJ,LOCK) \
  ACE_Write_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) return;
# define ACE_WRITE_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN) \
  ACE_Write_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) return RETURN;
# define ACE_READ_GUARD(MUTEX,OBJ,LOCK) \
  ACE_Read_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) return;
# define ACE_READ_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN) \
  ACE_Read_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) return RETURN;
# define ACE_THREAD_GUARD(OBJ,LOCK) \
  ACE_Thread_Mutex_Guard OBJ (LOCK); \
    if (OBJ.locked () == 0) return;
# define ACE_THREAD_GUARD_RETURN(OBJ,LOCK,RETURN) \
  ACE_Thread_Mutex_Guard OBJ (LOCK); \
    if (OBJ.locked () == 0) return RETURN;
# define ACE_THREAD_GUARD_THROW(OBJ,LOCK,EXCEPTION) \
  ACE_Thread_Mutex_Guard OBJ (LOCK); \
    if (OBJ.locked () == 0) TAO_THROW (EXCEPTION);
# define ACE_THREAD_GUARD_THROW_RETURN(OBJ,LOCK,EXCEPTION,RETURN) \
  ACE_Thread_Mutex_Guard OBJ (LOCK); \
    if (OBJ.locked () == 0) TAO_THROW_RETURN (EXCEPTION, RETURN);

# if defined (ACE_HAS_POSIX_SEM)
#   include /**/ <semaphore.h>
typedef struct
{
  sem_t *sema_;
  // Pointer to semaphore handle.  This is allocated by ACE if we are
  // working with an unnamed POSIX semaphore or by the OS if we are
  // working with a named POSIX semaphore.

  char *name_;
  // Name of the semaphore (if this is non-NULL then this is a named
  // POSIX semaphore, else its an unnamed POSIX semaphore).
} ACE_sema_t;
# endif /* ACE_HAS_POSIX_SEM */

struct cancel_state
{
  int cancelstate;
  // e.g., PTHREAD_CANCEL_ENABLE, PTHREAD_CANCEL_DISABLE,
  // PTHREAD_CANCELED.

  int canceltype;
  // e.g., PTHREAD_CANCEL_DEFERRED and PTHREAD_CANCEL_ASYNCHRONOUS.
};

# if defined (ACE_HAS_WINCE)
#   include /**/ <types.h>

typedef DWORD  nlink_t;

// CE's add-on for c-style fstat/stat functionalities.  This struct is
// by no mean complete compared to what you usually find in UNIX
// platforms.  Only members that have direct conversion using Win32's
// BY_HANDLE_FILE_INFORMATION are defined so that users can discover
// non-supported members at compile time.  Time values are of type
// ACE_Time_Value for easy comparison.

struct stat
{
  //  mode_t   st_mode;    // UNIX styled file attribute
  //  nlink_t  st_nlink;   // number of hard links
  ACE_Time_Value st_atime; // time of last access
  ACE_Time_Value st_mtime; // time of last data modification
  off_t st_size;           // file size, in bytes
  //  u_long   st_blksize; // optimal blocksize for I/O
  //  u_long   st_flags;   // user defined flags for file
};

# else /* ! ACE_HAS_WINCE */
#   if defined (ACE_LACKS_SYS_TYPES_H)
#     if ! defined (ACE_PSOS)
  typedef unsigned char u_char;
  typedef unsigned short u_short;
  typedef unsigned int u_int;
  typedef unsigned long u_long;

  typedef unsigned char uchar_t;
  typedef unsigned short ushort_t;
  typedef unsigned int  uint_t;
  typedef unsigned long ulong_t;
#     endif /* ! defined (ACE_PSOS) */
#   else
#     include /**/ <sys/types.h>
#   endif  /* ACE_LACKS_SYS_TYPES_H */

#   if ! defined (ACE_PSOS)
#     include /**/ <sys/stat.h>
#   endif
# endif /* ACE_HAS_WINCE */

# if defined (ACE_HAS_EXCEPTIONS)
#   define ACE_THROW_SPEC(X) throw X
# else  /* ! ACE_HAS_EXCEPTIONS */
#   define ACE_THROW_SPEC(X)
# endif /* ! ACE_HAS_EXCEPTIONS */

# if defined (ACE_HAS_THREADS)

#   if defined (ACE_HAS_STHREADS)
#     include /**/ <synch.h>
#     include /**/ <thread.h>
#     define ACE_SCOPE_PROCESS P_PID
#     define ACE_SCOPE_LWP P_LWPID
#     define ACE_SCOPE_THREAD (ACE_SCOPE_LWP + 1)
#   else
#     define ACE_SCOPE_PROCESS 0
#     define ACE_SCOPE_LWP 1
#     define ACE_SCOPE_THREAD 2
#   endif /* ACE_HAS_STHREADS */

#   if !defined (ACE_HAS_PTHREADS)
#     define ACE_SCHED_OTHER 0
#     define ACE_SCHED_FIFO 1
#     define ACE_SCHED_RR 2
#   endif /* ! ACE_HAS_PTHREADS */

#   if defined (ACE_HAS_PTHREADS)
#     define ACE_SCHED_OTHER SCHED_OTHER
#     define ACE_SCHED_FIFO SCHED_FIFO
#     define ACE_SCHED_RR SCHED_RR

// Definitions for mapping POSIX pthreads draft 6 into 1003.1c names

#     if defined (ACE_HAS_PTHREADS_DRAFT6)
#       define PTHREAD_SCOPE_PROCESS           PTHREAD_SCOPE_LOCAL
#       define PTHREAD_SCOPE_SYSTEM            PTHREAD_SCOPE_GLOBAL
#       define PTHREAD_CREATE_UNDETACHED       0
#       define PTHREAD_CREATE_DETACHED         1
#       define PTHREAD_CREATE_JOINABLE         0
#       define PTHREAD_EXPLICIT_SCHED          0
#       define PTHREAD_MIN_PRIORITY            0
#       define PTHREAD_MAX_PRIORITY            126
#     endif /* ACE_HAS_PTHREADS_DRAFT6 */

// Definitions for THREAD- and PROCESS-LEVEL priorities...some
// implementations define these while others don't.  In order to
// further complicate matters, we don't redefine the default (*_DEF)
// values if they've already been defined, which allows individual
// programs to have their own ACE-wide "default".

// PROCESS-level values
#     if !defined(_UNICOS)
#       define ACE_PROC_PRI_FIFO_MIN  (sched_get_priority_min(SCHED_FIFO))
#       define ACE_PROC_PRI_RR_MIN    (sched_get_priority_min(SCHED_RR))
#       define ACE_PROC_PRI_OTHER_MIN (sched_get_priority_min(SCHED_OTHER))
#     else // UNICOS is missing a sched_get_priority_min() implementation
#       define ACE_PROC_PRI_FIFO_MIN  0
#       define ACE_PROC_PRI_RR_MIN    0
#       define ACE_PROC_PRI_OTHER_MIN 0
#     endif
#     define ACE_PROC_PRI_FIFO_MAX  (sched_get_priority_max(SCHED_FIFO))
#     define ACE_PROC_PRI_RR_MAX    (sched_get_priority_max(SCHED_RR))
#     define ACE_PROC_PRI_OTHER_MAX (sched_get_priority_max(SCHED_OTHER))
#     if !defined(ACE_PROC_PRI_FIFO_DEF)
#       define ACE_PROC_PRI_FIFO_DEF (ACE_PROC_PRI_FIFO_MIN + (ACE_PROC_PRI_FIFO_MAX - ACE_PROC_PRI_FIFO_MIN)/2)
#     endif
#     if !defined(ACE_PROC_PRI_RR_DEF)
#       define ACE_PROC_PRI_RR_DEF (ACE_PROC_PRI_RR_MIN + (ACE_PROC_PRI_RR_MAX - ACE_PROC_PRI_RR_MIN)/2)
#     endif
#     if !defined(ACE_PROC_PRI_OTHER_DEF)
#       define ACE_PROC_PRI_OTHER_DEF (ACE_PROC_PRI_OTHER_MIN + (ACE_PROC_PRI_OTHER_MAX - ACE_PROC_PRI_OTHER_MIN)/2)
#     endif

// THREAD-level values
#     if defined(PRI_FIFO_MIN) && defined(PRI_FIFO_MAX) && defined(PRI_RR_MIN) && defined(PRI_RR_MAX) && defined(PRI_OTHER_MIN) && defined(PRI_OTHER_MAX)
#       define ACE_THR_PRI_FIFO_MIN  PRI_FIFO_MIN
#       define ACE_THR_PRI_FIFO_MAX  PRI_FIFO_MAX
#       define ACE_THR_PRI_RR_MIN    PRI_RR_MIN
#       define ACE_THR_PRI_RR_MAX    PRI_RR_MAX
#       define ACE_THR_PRI_OTHER_MIN PRI_OTHER_MIN
#       define ACE_THR_PRI_OTHER_MAX PRI_OTHER_MAX
#     elif defined (AIX)
#       define ACE_THR_PRI_FIFO_MIN  PRIORITY_MIN
#       define ACE_THR_PRI_FIFO_MAX  PRIORITY_MAX
#       define ACE_THR_PRI_RR_MIN    PRIORITY_MIN
#       define ACE_THR_PRI_RR_MAX    PRIORITY_MAX
#       define ACE_THR_PRI_OTHER_MIN PRIORITY_MIN
#       define ACE_THR_PRI_OTHER_MAX PRIORITY_MAX
#     elif defined (sun)
        // SunOS 5.6 could use sched_get_priority_min/max () for FIFO
        // and RR.  But for OTHER, it returns negative values, which
        // can't be used.  sched_get_priority_min/max () aren't
        // supported in SunOS 5.5.1.
#       define ACE_THR_PRI_FIFO_MIN  0
#       define ACE_THR_PRI_FIFO_MAX  59
#       define ACE_THR_PRI_RR_MIN    0
#       define ACE_THR_PRI_RR_MAX    59
#       define ACE_THR_PRI_OTHER_MIN 0
#       define ACE_THR_PRI_OTHER_MAX 59
#     else
#       define ACE_THR_PRI_FIFO_MIN  ACE_PROC_PRI_FIFO_MIN
#       define ACE_THR_PRI_FIFO_MAX  ACE_PROC_PRI_FIFO_MAX
#       define ACE_THR_PRI_RR_MIN    ACE_PROC_PRI_RR_MIN
#       define ACE_THR_PRI_RR_MAX    ACE_PROC_PRI_RR_MAX
#       define ACE_THR_PRI_OTHER_MIN ACE_PROC_PRI_OTHER_MIN
#       define ACE_THR_PRI_OTHER_MAX ACE_PROC_PRI_OTHER_MAX
#     endif
#     if !defined(ACE_THR_PRI_FIFO_DEF)
#       define ACE_THR_PRI_FIFO_DEF  (ACE_THR_PRI_FIFO_MIN + (ACE_THR_PRI_FIFO_MAX - ACE_THR_PRI_FIFO_MIN)/2)
#     endif
#     if !defined(ACE_THR_PRI_RR_DEF)
#       define ACE_THR_PRI_RR_DEF (ACE_THR_PRI_RR_MIN + (ACE_THR_PRI_RR_MAX - ACE_THR_PRI_RR_MIN)/2)
#     endif
#     if !defined(ACE_THR_PRI_OTHER_DEF)
#       define ACE_THR_PRI_OTHER_DEF (ACE_THR_PRI_OTHER_MIN + (ACE_THR_PRI_OTHER_MAX - ACE_THR_PRI_OTHER_MIN)/2)
#     endif

      // Typedefs to help compatibility with Windows NT and Pthreads.
      typedef pthread_t ACE_hthread_t;
      typedef pthread_t ACE_thread_t;

#     if defined (ACE_HAS_TSS_EMULATION)
        typedef pthread_key_t ACE_OS_thread_key_t;
        typedef u_long ACE_thread_key_t;
#     else  /* ! ACE_HAS_TSS_EMULATION */
        typedef pthread_key_t ACE_thread_key_t;
#     endif /* ! ACE_HAS_TSS_EMULATION */

#     if !defined (ACE_LACKS_COND_T)
 typedef pthread_mutex_t ACE_mutex_t;
 typedef pthread_cond_t ACE_cond_t;
#     endif /* ! ACE_LACKS_COND_T */
typedef pthread_mutex_t ACE_thread_mutex_t;

#     if !defined (PTHREAD_CANCEL_DISABLE)
#       define PTHREAD_CANCEL_DISABLE      0
#     endif /* PTHREAD_CANCEL_DISABLE */

#     if !defined (PTHREAD_CANCEL_ENABLE)
#       define PTHREAD_CANCEL_ENABLE       0
#     endif /* PTHREAD_CANCEL_ENABLE */

#     if !defined (PTHREAD_CANCEL_DEFERRED)
#       define PTHREAD_CANCEL_DEFERRED     0
#     endif /* PTHREAD_CANCEL_DEFERRED */

#     if !defined (PTHREAD_CANCEL_ASYNCHRONOUS)
#       define PTHREAD_CANCEL_ASYNCHRONOUS 0
#     endif /* PTHREAD_CANCEL_ASYNCHRONOUS */

#     define THR_CANCEL_DISABLE      PTHREAD_CANCEL_DISABLE
#     define THR_CANCEL_ENABLE       PTHREAD_CANCEL_ENABLE
#     define THR_CANCEL_DEFERRED     PTHREAD_CANCEL_DEFERRED
#     define THR_CANCEL_ASYNCHRONOUS PTHREAD_CANCEL_ASYNCHRONOUS

#     if !defined (PTHREAD_CREATE_JOINABLE)
#       if defined (PTHREAD_CREATE_UNDETACHED)
#         define PTHREAD_CREATE_JOINABLE PTHREAD_CREATE_UNDETACHED
#       else
#         define PTHREAD_CREATE_JOINABLE 0
#       endif /* PTHREAD_CREATE_UNDETACHED */
#     endif /* PTHREAD_CREATE_JOINABLE */

#     if !defined (PTHREAD_CREATE_DETACHED)
#       define PTHREAD_CREATE_DETACHED 1
#     endif /* PTHREAD_CREATE_DETACHED */

#     if !defined (PTHREAD_PROCESS_PRIVATE) && !defined (ACE_HAS_PTHREAD_PROCESS_ENUM)
#       if defined (PTHREAD_MUTEXTYPE_FAST)
#         define PTHREAD_PROCESS_PRIVATE PTHREAD_MUTEXTYPE_FAST
#       else
#         define PTHREAD_PROCESS_PRIVATE 0
#       endif /* PTHREAD_MUTEXTYPE_FAST */
#     endif /* PTHREAD_PROCESS_PRIVATE */

#     if !defined (PTHREAD_PROCESS_SHARED) && !defined (ACE_HAS_PTHREAD_PROCESS_ENUM)
#       if defined (PTHREAD_MUTEXTYPE_FAST)
#         define PTHREAD_PROCESS_SHARED PTHREAD_MUTEXTYPE_FAST
#       else
#         define PTHREAD_PROCESS_SHARED 1
#       endif /* PTHREAD_MUTEXTYPE_FAST */
#     endif /* PTHREAD_PROCESS_SHARED */

#     if defined (ACE_HAS_PTHREADS_DRAFT4)
#       if defined (PTHREAD_PROCESS_PRIVATE)
#         define USYNC_THREAD    PTHREAD_PROCESS_PRIVATE
#       else
#         define USYNC_THREAD    MUTEX_NONRECURSIVE_NP
#       endif /* PTHREAD_PROCESS_PRIVATE */

#       if defined (PTHREAD_PROCESS_SHARED)
#         define USYNC_PROCESS   PTHREAD_PROCESS_SHARED
#       else
#         define USYNC_PROCESS   MUTEX_NONRECURSIVE_NP
#       endif /* PTHREAD_PROCESS_SHARED */
#     elif !defined (ACE_HAS_STHREADS)
#       define USYNC_THREAD PTHREAD_PROCESS_PRIVATE
#       define USYNC_PROCESS PTHREAD_PROCESS_SHARED
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */

#     define THR_BOUND               0x00000001
#     if defined (CHORUS)
#       define THR_NEW_LWP             0x00000000
#     else
#       define THR_NEW_LWP             0x00000002
#     endif /* CHORUS */
#     define THR_DETACHED            0x00000040
#     define THR_SUSPENDED           0x00000080
#     define THR_DAEMON              0x00000100
#     define THR_JOINABLE            0x00010000
#     define THR_SCHED_FIFO          0x00020000
#     define THR_SCHED_RR            0x00040000
#     define THR_SCHED_DEFAULT       0x00080000
#     if defined (ACE_HAS_IRIX62_THREADS)
#       define THR_SCOPE_SYSTEM        0x00100000
#     else
#       define THR_SCOPE_SYSTEM        THR_BOUND
#     endif /* ACE_HAS_IRIX62_THREADS */
#     define THR_SCOPE_PROCESS       0x00200000
#     define THR_INHERIT_SCHED       0x00400000
#     define THR_EXPLICIT_SCHED      0x00800000
#     define THR_SCHED_IO            0x01000000

#     if !defined (ACE_HAS_STHREADS)
#       if !defined (ACE_HAS_POSIX_SEM)
// This is used to implement semaphores for POSIX pthreads, but
// without POSIX semaphores.  It is different than the POSIX sem_t.
class ACE_Export ACE_sema_t
{
friend class ACE_OS;
protected:
  ACE_mutex_t lock_;
  // Serialize access to internal state.

  ACE_cond_t count_nonzero_;
  // Block until there are no waiters.

  u_long count_;
  // Count of the semaphore.

  u_long waiters_;
  // Number of threads that have called <ACE_OS::sema_wait>.
};
#       endif /* !ACE_HAS_POSIX_SEM */

#       if defined (ACE_LACKS_PTHREAD_YIELD) && defined (ACE_HAS_THR_YIELD)
#         if defined (USYNC_THREAD)
#           undef USYNC_THREAD
#         endif /* USYNC_THREAD */
#         if defined (USYNC_PROCESS)
#           undef USYNC_PROCESS
#         endif /* USYNC_PROCESS */
// If we are on Solaris we can just reuse the existing implementations
// of these synchronization types.
#         if !defined (ACE_LACKS_RWLOCK_T)
#           include /**/ <synch.h>
typedef rwlock_t ACE_rwlock_t;
#         endif /* !ACE_LACKS_RWLOCK_T */
#         include /**/ <thread.h>
#       endif /* defined (ACE_LACKS_PTHREAD_YIELD) && defined (ACE_HAS_THR_YIELD) */

#     else
#       if !defined (ACE_HAS_POSIX_SEM)
typedef sema_t ACE_sema_t;
#       endif /* !ACE_HAS_POSIX_SEM */
#     endif /* !ACE_HAS_STHREADS */
#   elif defined (ACE_HAS_STHREADS)
// Solaris threads, without PTHREADS.
// Typedefs to help compatibility with Windows NT and Pthreads.
typedef thread_t ACE_thread_t;
typedef thread_key_t ACE_thread_key_t;
typedef mutex_t ACE_mutex_t;
#     if !defined (ACE_LACKS_RWLOCK_T)
typedef rwlock_t ACE_rwlock_t;
#     endif /* !ACE_LACKS_RWLOCK_T */
#     if !defined (ACE_HAS_POSIX_SEM)
typedef sema_t ACE_sema_t;
#     endif /* !ACE_HAS_POSIX_SEM */
typedef cond_t ACE_cond_t;
typedef ACE_thread_t ACE_hthread_t;
typedef ACE_mutex_t ACE_thread_mutex_t;

#     define THR_CANCEL_DISABLE      0
#     define THR_CANCEL_ENABLE       0
#     define THR_CANCEL_DEFERRED     0
#     define THR_CANCEL_ASYNCHRONOUS 0
#     define THR_JOINABLE            0
#     define THR_SCHED_FIFO          0
#     define THR_SCHED_RR            0
#     define THR_SCHED_DEFAULT       0

#   elif defined (ACE_PSOS)

// implement ACE_thread_mutex_t and ACE_mutex_t using pSOS semaphores
typedef u_long ACE_mutex_t;
typedef u_long ACE_thread_mutex_t;

typedef u_long ACE_thread_t;
typedef u_long ACE_hthread_t;

// TCB registers 0-7 are for application use
#     define PSOS_TASK_REG_TSS 0
#     define PSOS_TASK_REG_MAX 7

#     define PSOS_TASK_MIN_PRIORITY   1
#     define PSOS_TASK_MAX_PRIORITY 239

// Key type: the ACE TSS emulation requires the key type be unsigned,
// for efficiency.  (Current POSIX and Solaris TSS implementations also
// use unsigned int, so the ACE TSS emulation is compatible with them.)
typedef u_int ACE_thread_key_t;

/* CDG - TBD - revisit these: compare pthreads and pSOS threads */
#     define THR_CANCEL_DISABLE      0  /* thread can never be cancelled */
#     define THR_CANCEL_ENABLE       0      /* thread can be cancelled */
#     define THR_CANCEL_DEFERRED     0      /* cancellation deferred to cancellation point */
#     define THR_CANCEL_ASYNCHRONOUS 0      /* cancellation occurs immediately */

#     define THR_BOUND               0
#     define THR_NEW_LWP             0
#     define THR_DETACHED            0
#     define THR_SUSPENDED           0
#     define THR_DAEMON              0
#     define THR_JOINABLE            0

#     define THR_SCHED_FIFO          0
#     define THR_SCHED_RR            0
#     define THR_SCHED_DEFAULT       0
#     define USYNC_THREAD            T_LOCAL
#     define USYNC_PROCESS           T_GLOBAL

/* from psos.h */
/* #define T_NOPREEMPT     0x00000001   Not preemptible bit */
/* #define T_PREEMPT       0x00000000   Preemptible */
/* #define T_TSLICE        0x00000002   Time-slicing enabled bit */
/* #define T_NOTSLICE      0x00000000   No Time-slicing */
/* #define T_NOASR         0x00000004   ASRs disabled bit */
/* #define T_ASR           0x00000000   ASRs enabled */

/* #define SM_GLOBAL       0x00000001  1 = Global */
/* #define SM_LOCAL        0x00000000  0 = Local */
/* #define SM_PRIOR        0x00000002  Queue by priority */
/* #define SM_FIFO         0x00000000  Queue by FIFO order */

/* #define T_NOFPU         0x00000000   Not using FPU */
/* #define T_FPU           0x00000002   Using FPU bit */

#   elif defined (VXWORKS)
// For mutex implementation using mutual-exclusion semaphores (which
// can be taken recursively).
#     include /**/ <semLib.h>

#     include /**/ <envLib.h>
#     include /**/ <hostLib.h>
#     include /**/ <ioLib.h>
#     include /**/ <remLib.h>
#     include /**/ <selectLib.h>
#     include /**/ <sigLib.h>
#     include /**/ <sockLib.h>
#     include /**/ <sysLib.h>
#     include /**/ <taskLib.h>
#     include /**/ <taskHookLib.h>

extern "C"
struct sockaddr_un {
  short sun_family;    // AF_UNIX.
  char  sun_path[108]; // path name.
};

#     define MAXPATHLEN  1024
#     define MAXNAMLEN   255
#     define NSIG (_NSIGS + 1)

// task options:  the other options are either obsolete, internal, or for
// Fortran or Ada support
#     define VX_UNBREAKABLE        0x0002  /* breakpoints ignored */
#     define VX_FP_TASK            0x0008  /* floating point coprocessor */
#     define VX_PRIVATE_ENV        0x0080  /* private environment support */
#     define VX_NO_STACK_FILL      0x0100  /* do not stack fill for checkstack () */

#     define THR_CANCEL_DISABLE      0
#     define THR_CANCEL_ENABLE       0
#     define THR_CANCEL_DEFERRED     0
#     define THR_CANCEL_ASYNCHRONOUS 0
#     define THR_BOUND               0
#     define THR_NEW_LWP             0
#     define THR_DETACHED            0
#     define THR_SUSPENDED           0
#     define THR_DAEMON              0
#     define THR_JOINABLE            0
#     define THR_SCHED_FIFO          0
#     define THR_SCHED_RR            0
#     define THR_SCHED_DEFAULT       0
#     define USYNC_THREAD            0
#     define USYNC_PROCESS           1 /* it's all global on VxWorks (without MMU
                                     option) */

#     if !defined (ACE_DEFAULT_SYNCH_TYPE)
 // Types include these options: SEM_Q_PRIORITY, SEM_Q_FIFO,
 // SEM_DELETE_SAFE, and SEM_INVERSION_SAFE.  SEM_Q_FIFO is
 // used as the default because that is VxWorks' default.
#       define ACE_DEFAULT_SYNCH_TYPE SEM_Q_FIFO
#     endif /* ! ACE_DEFAULT_SYNCH_TYPE */

typedef SEM_ID ACE_mutex_t;
// implement ACE_thread_mutex_t with ACE_mutex_t sinces there's just one process . . .
typedef ACE_mutex_t ACE_thread_mutex_t;
#     if !defined (ACE_HAS_POSIX_SEM)
// Use VxWorks semaphores, wrapped . . .
typedef struct
{
  SEM_ID sema_;
  // Semaphore handle.  This is allocated by VxWorks.

  char *name_;
  // Name of the semaphore:  always NULL with VxWorks.
} ACE_sema_t;
#     endif /* !ACE_HAS_POSIX_SEM */
typedef char * ACE_thread_t;
typedef int ACE_hthread_t;
// Key type: the ACE TSS emulation requires the key type be unsigned,
// for efficiency.  (Current POSIX and Solaris TSS implementations also
// use u_int, so the ACE TSS emulation is compatible with them.)
typedef u_int ACE_thread_key_t;

      // Marker for ACE_Thread_Manager to indicate that it allocated
      // an ACE_thread_t.  It is placed at the beginning of the ID.
#     define ACE_THR_ID_ALLOCATED '\022'

#   elif defined (ACE_HAS_WTHREADS)

typedef CRITICAL_SECTION ACE_thread_mutex_t;
typedef struct
{
  int type_; // Either USYNC_THREAD or USYNC_PROCESS
  union
  {
    HANDLE proc_mutex_;
    CRITICAL_SECTION thr_mutex_;
  };
} ACE_mutex_t;

// Wrapper for NT Events.
typedef HANDLE ACE_event_t;

//@@ ACE_USES_WINCE_SEMA_SIMULATION is used to debug
//   semaphore simulation on WinNT.  It should be
//   changed to ACE_USES_HAS_WINCE at some later point.
#     if !defined (ACE_USES_WINCE_SEMA_SIMULATION)
typedef HANDLE ACE_sema_t;
#     else

class ACE_Export ACE_sema_t
{
  // = TITLE
  // Semaphore simulation for Windows CE.
public:
  ACE_thread_mutex_t lock_;
  // Serializes access to <count_>.

  ACE_event_t count_nonzero_;
  // This event is signaled whenever the count becomes non-zero.

  u_int count_;
  // Current count of the semaphore.
};

#     endif /* ACE_USES_WINCE_SEMA_SIMULATION */

// These need to be different values, neither of which can be 0...
#     define USYNC_THREAD 1
#     define USYNC_PROCESS 2

#     define THR_CANCEL_DISABLE      0
#     define THR_CANCEL_ENABLE       0
#     define THR_CANCEL_DEFERRED     0
#     define THR_CANCEL_ASYNCHRONOUS 0
#     define THR_DETACHED    0x02000000 /* ?? ignore in most places */
#     define THR_BOUND       0          /* ?? ignore in most places */
#     define THR_NEW_LWP     0          /* ?? ignore in most places */
#     define THR_DAEMON      0          /* ?? ignore in most places */
#     define THR_JOINABLE    0          /* ?? ignore in most places */
#     define THR_SUSPENDED   CREATE_SUSPENDED
#     define THR_USE_AFX             0x01000000
#     define THR_SCHED_FIFO          0
#     define THR_SCHED_RR            0
#     define THR_SCHED_DEFAULT       0
#   endif /* ACE_HAS_PTHREADS / STHREADS / PSOS / VXWORKS / WTHREADS */

#   if defined (ACE_LACKS_COND_T)
class ACE_Export ACE_cond_t
{
  // = TITLE
  //     This structure is used to implement condition variables on
  //     VxWorks and Win32.
  //
  // = DESCRIPTION
  //     At the current time, this stuff only works for threads
  //     within the same process.
public:
  friend class ACE_OS;

  long waiters (void) const;
  // Returns the number of waiters.

protected:
  long waiters_;
  // Number of waiting threads.

  ACE_thread_mutex_t waiters_lock_;
  // Serialize access to the waiters count.

  ACE_sema_t sema_;
  // Queue up threads waiting for the condition to become signaled.

#     if defined (VXWORKS) || defined (ACE_PSOS)
  ACE_sema_t waiters_done_;
  // A semaphore used by the broadcast/signal thread to wait for all
  // the waiting thread(s) to wake up and be released from the
  // semaphore.
#     elif defined (ACE_WIN32)
  HANDLE waiters_done_;
  // An auto reset event used by the broadcast/signal thread to wait
  // for the waiting thread(s) to wake up and get a chance at the
  // semaphore.
#     else
#       error "SOMEONE FIX ME!"
#     endif /* VXWORKS || ACE_PSOS */

  size_t was_broadcast_;
  // Keeps track of whether we were broadcasting or just signaling.
};
#   endif /* ACE_LACKS_COND_T */

#   if defined (ACE_LACKS_RWLOCK_T)
struct ACE_Export ACE_rwlock_t
{
  // = TITLE
  //     This is used to implement readers/writer locks on NT,
  //     VxWorks, and POSIX pthreads.
  //
  // = DESCRIPTION
  //     At the current time, this stuff only works for threads
  //     within the same process.
protected:
  friend class ACE_OS;

  ACE_mutex_t lock_;
  // Serialize access to internal state.

  ACE_cond_t waiting_readers_;
  // Reader threads waiting to acquire the lock.

  int num_waiting_readers_;
  // Number of waiting readers.

  ACE_cond_t waiting_writers_;
  // Writer threads waiting to acquire the lock.

  int num_waiting_writers_;
  // Number of waiting writers.

  int ref_count_;
  // Value is -1 if writer has the lock, else this keeps track of the
  // number of readers holding the lock.

  int important_writer_;
  // indicate that a reader is trying to upgrade

  ACE_cond_t waiting_important_writer_;
  // condition for the upgrading reader
};
#   elif defined (ACE_HAS_STHREADS)
#     include /**/ <synch.h>
typedef rwlock_t ACE_rwlock_t;
#   endif /* ACE_LACKS_RWLOCK_T */

// #define ACE_THR_PRI_FIFO_DEF on all threaded platforms, if not defined
// above or in the individual platform config file.  It should be used by
// applications for a default real-time thread priority.
#   if !defined(ACE_THR_PRI_FIFO_DEF)
#     if defined (ACE_WTHREADS)
      // It would be more in spirit to use THREAD_PRIORITY_NORMAL.  But,
      // using THREAD_PRIORITY_ABOVE_NORMAL should give preference to the
      // threads in this process, even if the process is not in the
      // REALTIME_PRIORITY_CLASS.
#       define ACE_THR_PRI_FIFO_DEF THREAD_PRIORITY_ABOVE_NORMAL
#     else  /* ! ACE_WTHREADS */
#       define ACE_THR_PRI_FIFO_DEF 0
#     endif /* ! ACE_WTHREADS */
#   endif /* ! ACE_THR_PRI_FIFO_DEF */

#if defined (ACE_HAS_RECURSIVE_MUTEXES)
typedef ACE_thread_mutex_t ACE_recursive_thread_mutex_t;
#else
class ACE_recursive_thread_mutex_t
{
  // = TITLE
  //     Implement a thin C++ wrapper that allows nested acquisition
  //     and release of a mutex that occurs in the same thread.
  //
  // = DESCRIPTION
  //     This implementation is based on an algorithm sketched by Dave
  //     Butenhof <butenhof@zko.dec.com>.  Naturally, I take the
  //     credit for any mistakes ;-)
public:
  ACE_thread_mutex_t nesting_mutex_;
  // Guards the state of the nesting level and thread id.

  ACE_cond_t lock_available_;
  // This condition variable suspends other waiting threads until the
  // mutex is available.

  int nesting_level_;
  // Current nesting level of the recursion.

  ACE_thread_t owner_id_;
  // Current owner of the lock.
};
#endif /* ACE_WIN32 */

# else /* !ACE_HAS_THREADS, i.e., the OS/platform doesn't support threading. */

// Give these things some reasonable value...
#   define ACE_SCOPE_PROCESS 0
#   define ACE_SCOPE_LWP 1
#   define ACE_SCOPE_THREAD 2
#   define ACE_SCHED_OTHER 0
#   define ACE_SCHED_FIFO 1
#   define ACE_SCHED_RR 2
#   define THR_CANCEL_DISABLE      0
#   define THR_CANCEL_ENABLE       0
#   define THR_CANCEL_DEFERRED     0
#   define THR_CANCEL_ASYNCHRONOUS 0
#   define THR_JOINABLE    0       /* ?? ignore in most places */
#   define THR_DETACHED    0       /* ?? ignore in most places */
#   define THR_DAEMON      0       /* ?? ignore in most places */
#   define THR_BOUND       0       /* ?? ignore in most places */
#   define THR_NEW_LWP     0       /* ?? ignore in most places */
#   define THR_SUSPENDED   0       /* ?? ignore in most places */
#   define THR_SCHED_FIFO          0
#   define THR_SCHED_RR            0
#   define THR_SCHED_DEFAULT       0
#   define USYNC_THREAD 0
#   define USYNC_PROCESS 0
// These are dummies needed for class OS.h
typedef int ACE_cond_t;
typedef int ACE_mutex_t;
typedef int ACE_thread_mutex_t;
typedef int ACE_recursive_thread_mutex_t;
#   if !defined (ACE_HAS_POSIX_SEM) && !defined (ACE_PSOS)
typedef int ACE_sema_t;
#   endif /* !ACE_HAS_POSIX_SEM && !ACE_PSOS */
typedef int ACE_rwlock_t;
typedef int ACE_thread_t;
typedef int ACE_hthread_t;
typedef u_int ACE_thread_key_t;

# endif /* ACE_HAS_THREADS */

# if defined (ACE_PSOS)

// Wrapper for NT events on pSOS.
class ACE_Export ACE_event_t
{
  friend class ACE_OS;

protected:

  ACE_mutex_t lock_;
  // Protect critical section.

  ACE_cond_t condition_;
  // Keeps track of waiters.

  int manual_reset_;
  // Specifies if this is an auto- or manual-reset event.

  int is_signaled_;
  // "True" if signaled.

  u_long waiting_threads_;
  // Number of waiting threads.
};

# endif /* ACE_PSOS */

// Standard C Library includes
// NOTE: stdarg.h must be #included before stdio.h on LynxOS.
# include /**/ <stdarg.h>
# if !defined (ACE_HAS_WINCE)
#   include /**/ <assert.h>
#   include /**/ <stdio.h>
// this is a nasty hack to get around problems with the
// pSOS definition of BUFSIZ as the config table entry
// (which is valued using the LC_BUFSIZ value anyway)
#   if defined (ACE_PSOS)
#     if defined (BUFSIZ)
#       undef BUFSIZ
#     endif /* defined (BUFSIZ) */
#     define BUFSIZ LC_BUFSIZ
#   endif /* defined (ACE_PSOS) */

#   include /**/ <new.h>
#   include /**/ <signal.h>
#   include /**/ <errno.h>
#   include /**/ <fcntl.h>
# endif /* ACE_HAS_WINCE */

# include /**/ <limits.h>
# include /**/ <ctype.h>
# include /**/ <string.h>
# include /**/ <stdlib.h>
# include /**/ <float.h>

// This is defined by XOPEN to be a minimum of 16.  POSIX.1g
// also defines this value.  platform-specific config.h can
// override this if need be.
# if !defined (IOV_MAX)
#  define IOV_MAX 16
# endif /* IOV_MAX */

# if defined (ACE_PSOS_SNARFS_HEADER_INFO)

  // header information snarfed from compiler provided header files
  // that are not included because there is already an identically
  // named file provided with pSOS, which does not have this info

  // from compiler supplied stdio.h
  extern FILE *fdopen(int, const char *);
  extern int getopt(int, char *const *, const char *);
  extern char *tempnam(const char *, const char *);
  extern "C" int fileno(FILE *);

//  #define fileno(stream)      ((stream)->_file)

  // from compiler supplied string.h
  extern char *strdup(const char *);

  // from compiler supplied stat.h
  extern mode_t umask(mode_t);
  extern int mkfifo(const char *, mode_t);
  extern int mkdir(const char *, mode_t);

  // from compiler supplied stdlib.h
  extern int putenv(char *);

  int isatty (ACE_HANDLE h);

# endif /* ACE_PSOS_SNARFS_HEADER_INFO */

# if defined (ACE_LACKS_SCHED_H)
#   include /**/ <sched.h>
# endif /* ACE_LACKS_SCHED_H */

# if defined (ACE_HAS_WINCE)
#   define islower iswlower
#   define isdigit iswdigit
# endif /* ACE_HAS_WINCE */


// If the user wants minimum IOStream inclusion, we will just include
// the forward declarations
# if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
// Forward declaration for streams
#   include "ace/iosfwd.h"
# else /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */
// Else they will get all the stream header files
#   include "ace/streams.h"
# endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

# if !defined (ACE_HAS_WINCE)
#   include /**/ <fcntl.h>
# endif /* ACE_HAS_WINCE */

// This must come after signal.h is #included.
# if defined (SCO)
#   define SIGIO SIGPOLL
#   include /**/ <sys/regset.h>
# endif /* SCO */

# if defined ACE_HAS_BYTESEX_H
#   include /**/ <bytesex.h>
# endif /* ACE_HAS_BYTESEX_H */
# include "ace/Basic_Types.h"

# if defined (ACE_HAS_SIG_MACROS)
#   undef sigemptyset
#   undef sigfillset
#   undef sigaddset
#   undef sigdelset
#   undef sigismember
# endif /* ACE_HAS_SIG_MACROS */

// This must come after signal.h is #included.  It's to counteract
// the sigemptyset and sigfillset #defines, which only happen
// when __OPTIMIZE__ is #defined (really!) on Linux.
# if defined (linux) && defined (__OPTIMIZE__)
#   undef sigemptyset
#   undef sigfillset
# endif /* linux && __OPTIMIZE__ */

# if defined (ACE_HAS_BROKEN_SENDMSG)
typedef struct msghdr ACE_SENDMSG_TYPE;
# else
typedef const struct msghdr ACE_SENDMSG_TYPE;
# endif /* ACE_HAS_BROKEN_SENDMSG */

# if defined (ACE_HAS_BROKEN_RANDR)
// The SunOS 5.4.X version of rand_r is inconsistent with the header
// files...
typedef u_int ACE_RANDR_TYPE;
extern "C" int rand_r (ACE_RANDR_TYPE seed);
# else
#   if defined (HPUX_10)
// HP-UX 10.x's stdlib.h (long *) doesn't match that man page (u_int *)
typedef long ACE_RANDR_TYPE;
#   else
typedef u_int ACE_RANDR_TYPE;
#   endif /* HPUX_10 */
# endif /* ACE_HAS_BROKEN_RANDR */

# if defined (ACE_HAS_UTIME)
#   include /**/ <utime.h>
# endif /* ACE_HAS_UTIME */

# if !defined (ACE_HAS_MSG) && !defined (SCO)
struct msghdr {};
# endif /* ACE_HAS_MSG */

# if defined (ACE_HAS_MSG) && defined (ACE_LACKS_MSG_ACCRIGHTS)
#   if !defined (msg_accrights)
#     undef msg_control
#     define msg_accrights msg_control
#   endif /* ! msg_accrights */

#   if !defined (msg_accrightslen)
#     undef msg_controllen
#     define msg_accrightslen msg_controllen
#   endif /* ! msg_accrightslen */
# endif /* ACE_HAS_MSG && ACE_LACKS_MSG_ACCRIGHTS */

# if !defined (ACE_HAS_SIG_ATOMIC_T)
typedef int sig_atomic_t;
# endif /* !ACE_HAS_SIG_ATOMIC_T */

# if !defined (ACE_HAS_SSIZE_T)
typedef int ssize_t;
# endif /* ACE_HAS_SSIZE_T */

# if defined (ACE_HAS_OLD_MALLOC)
typedef char *ACE_MALLOC_T;
# else
typedef void *ACE_MALLOC_T;
# endif /* ACE_HAS_OLD_MALLOC */

# if defined (ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES)
// Prototypes for both signal() and struct sigaction are consistent..
#   if defined (ACE_HAS_SIG_C_FUNC)
extern "C" {
#   endif /* ACE_HAS_SIG_C_FUNC */
#   if !defined (ACE_PSOS)
typedef void (*ACE_SignalHandler)(int);
typedef void (*ACE_SignalHandlerV)(int);
#   endif /* !defined (ACE_PSOS) */
#   if defined (ACE_HAS_SIG_C_FUNC)
}
#   endif /* ACE_HAS_SIG_C_FUNC */
# elif defined (ACE_HAS_LYNXOS_SIGNALS)
typedef void (*ACE_SignalHandler)(...);
typedef void (*ACE_SignalHandlerV)(...);
# elif defined (ACE_HAS_TANDEM_SIGNALS)
typedef void (*ACE_SignalHandler)(...);
typedef void (*ACE_SignalHandlerV)(...);
# elif defined (ACE_HAS_IRIX_53_SIGNALS)
typedef void (*ACE_SignalHandler)(...);
typedef void (*ACE_SignalHandlerV)(...);
# elif defined (ACE_HAS_SPARCWORKS_401_SIGNALS)
typedef void (*ACE_SignalHandler)(int, ...);
typedef void (*ACE_SignalHandlerV)(int,...);
# elif defined (ACE_HAS_SUNOS4_SIGNAL_T)
typedef void (*ACE_SignalHandler)(...);
typedef void (*ACE_SignalHandlerV)(...);
# elif defined (ACE_HAS_SVR4_SIGNAL_T)
// SVR4 Signals are inconsistent (e.g., see struct sigaction)..
typedef void (*ACE_SignalHandler)(int);
#   if !defined (m88k)     /*  with SVR4_SIGNAL_T */
typedef void (*ACE_SignalHandlerV)(void);
#   else
typedef void (*ACE_SignalHandlerV)(int);
#   endif  //  m88k        /*  with SVR4_SIGNAL_T */
# elif defined (ACE_WIN32)
typedef void (__cdecl *ACE_SignalHandler)(int);
typedef void (__cdecl *ACE_SignalHandlerV)(int);
# elif defined (ACE_HAS_UNIXWARE_SVR4_SIGNAL_T)
typedef void (*ACE_SignalHandler)(int);
typedef void (*ACE_SignalHandlerV)(...);
# else /* This is necessary for some older broken version of cfront */
#   if defined (SIG_PF)
#     define ACE_SignalHandler SIG_PF
#   else
typedef void (*ACE_SignalHandler)(int);
#   endif /* SIG_PF */
typedef void (*ACE_SignalHandlerV)(...);
# endif /* ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES */

# if defined (BUFSIZ)
#   define ACE_STREAMBUF_SIZE BUFSIZ
# else
#   define ACE_STREAMBUF_SIZE 1024
# endif /* BUFSIZ */

# if defined (ACE_WIN32)
// Turn off warnings for /W4
// To resume any of these warning: #pragma warning(default: 4xxx)
// which should be placed after these defines
#   ifndef ALL_WARNINGS
// #pragma warning(disable: 4101)  // unreferenced local variable
#     pragma warning(disable: 4127)  /* constant expression for TRACE/ASSERT */
#     pragma warning(disable: 4134)  /* message map member fxn casts */
#     pragma warning(disable: 4511)  /* private copy constructors are good to have */
#     pragma warning(disable: 4512)  /* private operator= are good to have */
#     pragma warning(disable: 4514)  /* unreferenced inlines are common */
#     pragma warning(disable: 4710)  /* private constructors are disallowed */
#     pragma warning(disable: 4705)  /* statement has no effect in optimized code */
// #pragma warning(disable: 4701)  // local variable *may* be used without init
// #pragma warning(disable: 4702)  // unreachable code caused by optimizations
#     pragma warning(disable: 4791)  /* loss of debugging info in retail version */
// #pragma warning(disable: 4204)  // non-constant aggregate initializer
#     pragma warning(disable: 4275)  /* deriving exported class from non-exported */
#     pragma warning(disable: 4251)  /* using non-exported as public in exported */
#     pragma warning(disable: 4786)  /* identifier was truncated to '255' characters in the browser information */
#     pragma warning(disable: 4097)  /* typedef-name used as synonym for class-name */
#   endif /*!ALL_WARNINGS */

// STRICT type checking in WINDOWS.H enhances type safety for Windows
// programs by using distinct types to represent all the different
// HANDLES in Windows. So for example, STRICT prevents you from
// mistakenly passing an HPEN to a routine expecting an HBITMAP.
// Note that we only use this if we
#   if defined (ACE_HAS_STRICT) && (ACE_HAS_STRICT != 0)
#     if !defined (STRICT)   /* may already be defined */
#       define STRICT
#     endif /* !STRICT */
#   endif /* ACE_HAS_STRICT */

#   if !defined (ACE_HAS_WINCE)
#     include /**/ <sys/timeb.h>
#   endif /* ACE_HAS_WINCE */

// The following defines are used by the ACE Name Server...
#   if !defined (ACE_DEFAULT_NAMESPACE_DIR_W)
#     define ACE_DEFAULT_NAMESPACE_DIR_W L"C:\\temp"
#   endif /* ACE_DEFAULT_NAMESPACE_DIR_W */

#   if !defined (ACE_DEFAULT_NAMESPACE_DIR_A)
#     define ACE_DEFAULT_NAMESPACE_DIR_A "C:\\temp"
#   endif /* ACE_DEFAULT_NAMESPACE_DIR_A */

#   if !defined (ACE_DEFAULT_LOCALNAME_A)
#     define ACE_DEFAULT_LOCALNAME_A "localnames"
#   endif /* ACE_DEFAULT_LOCALNAME_A */

#   if !defined (ACE_DEFAULT_LOCALNAME_W)
#     define ACE_DEFAULT_LOCALNAME_W L"localnames"
#   endif /* ACE_DEFAULT_LOCALNAME_W */

#   if !defined (ACE_DEFAULT_GLOBALNAME_A)
#     define ACE_DEFAULT_GLOBALNAME_A "globalnames"
#   endif /* ACE_DEFAULT_GLOBALNAME_A */

#   if !defined (ACE_DEFAULT_GLOBALNAME_W)
#     define ACE_DEFAULT_GLOBALNAME_W L"globalnames"
#   endif /* ACE_DEFAULT_GLOBALNAME_W */

// Need to work around odd glitches with NT.
#   if !defined (ACE_MAX_DEFAULT_PORT)
#     define ACE_MAX_DEFAULT_PORT 0
#   endif /* ACE_MAX_DEFAULT_PORT */

// We're on WinNT or Win95
#   define ACE_PLATFORM_A "Win32"
#   define ACE_PLATFORM_EXE_SUFFIX_A ".exe"
#   define ACE_PLATFORM_W L"Win32"
#   define ACE_PLATFORM_EXE_SUFFIX_W L".exe"

// Used for ACE_MMAP_Memory_Pool
#   if !defined (ACE_DEFAULT_BACKING_STORE)
#     define ACE_DEFAULT_BACKING_STORE ACE_TEXT ("C:\\temp\\ace-malloc-XXXXXX")
#   endif /* ACE_DEFAULT_BACKING_STORE */

// Used for ACE_FILE_Connector
#   if !defined (ACE_DEFAULT_TEMP_FILE)
#     define ACE_DEFAULT_TEMP_FILE ACE_TEXT ("C:\\temp\\ace-file-XXXXXX")
#   endif /* ACE_DEFAULT_TEMP_FILE */

// Used for logging
#   if !defined (ACE_DEFAULT_LOGFILE)
#     define ACE_DEFAULT_LOGFILE "C:\\temp\\logfile"
#   endif /* ACE_DEFAULT_LOGFILE */

// Used for dynamic linking
#   if !defined (ACE_DEFAULT_SVC_CONF)
#     define ACE_DEFAULT_SVC_CONF ".\\svc.conf"
#   endif /* ACE_DEFAULT_SVC_CONF */

// The following are #defines and #includes that are specific to
// WIN32.
#   define ACE_STDIN GetStdHandle (STD_INPUT_HANDLE)
#   define ACE_STDOUT GetStdHandle (STD_OUTPUT_HANDLE)
#   define ACE_STDERR GetStdHandle (STD_ERROR_HANDLE)

// Default semaphore key and mutex name
#   if !defined (ACE_DEFAULT_SEM_KEY)
#     define ACE_DEFAULT_SEM_KEY "ACE_SEM_KEY"
#   endif /* ACE_DEFAULT_SEM_KEY */

#   define ACE_INVALID_SEM_KEY 0

#   if defined (ACE_HAS_WINCE)
// @@ WinCE probably doesn't have structural exception support
//    But I need to double check to find this out
#     define ACE_SEH_TRY if (1)
#     define ACE_SEH_EXCEPT(X) while (0)
#     define ACE_SEH_FINALLY if (1)
#   else
#     if defined(__BORLANDC__)
#       if (__BORLANDC__ >= 0x0530) /* Borland C++ Builder 3.0 */
#         define ACE_SEH_TRY try
#         define ACE_SEH_EXCEPT(X) __except(X)
#         define ACE_SEH_FINALLY __finally
#       else
#         define ACE_SEH_TRY if (1)
#         define ACE_SEH_EXCEPT(X) while (0)
#         define ACE_SEH_FINALLY if (1)
#       endif
#     else
#       define ACE_SEH_TRY __try
#       define ACE_SEH_EXCEPT(X) __except(X)
#       define ACE_SEH_FINALLY __finally
#     endif /* __BORLANDC__ */
#   endif /* ACE_HAS_WINCE */

#   if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
typedef int (*ACE_SEH_EXCEPT_HANDLER)(void *);
// Prototype of win32 structured exception handler functions.
// They are used to get the exception handling expression or
// as exception handlers.
#   endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */

// The "null" device on Win32.
#   define ACE_DEV_NULL "nul"

// Define the pathname separator characters for Win32 (ugh).
#   define ACE_DIRECTORY_SEPARATOR_STR_A "\\"
#   define ACE_DIRECTORY_SEPARATOR_STR_W L"\\"
#   define ACE_DIRECTORY_SEPARATOR_CHAR_A '\\'
#   define ACE_DIRECTORY_SEPARATOR_CHAR_W L'\\'
#   define ACE_LD_SEARCH_PATH "PATH"
#   define ACE_LD_SEARCH_PATH_SEPARATOR_STR ";"
#   define ACE_DLL_SUFFIX ".dll"
#   define ACE_DLL_PREFIX ""

// This will help until we figure out everything:
#   define NFDBITS 32 /* only used in unused functions... */
// These two may be used for internal flags soon:
#   define MAP_PRIVATE 1
#   define MAP_SHARED  2
#   define MAP_FIXED   4

#   define RUSAGE_SELF 1

struct shmaddr { };
struct msqid_ds {};

// Fake the UNIX rusage structure.  Perhaps we can add more to this
// later on?
struct rusage
{
  FILETIME ru_utime;
  FILETIME ru_stime;
};

// MMAP flags
#   define PROT_READ PAGE_READONLY
#   define PROT_WRITE PAGE_READWRITE
#   define PROT_RDWR PAGE_READWRITE
/* If we can find suitable use for these flags, here they are:
PAGE_WRITECOPY
PAGE_EXECUTE
PAGE_EXECUTE_READ
PAGE_EXECUTE_READWRITE
PAGE_EXECUTE_WRITECOPY
PAGE_GUARD
PAGE_NOACCESS
PAGE_NOCACHE  */

#   if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
#     include "ace/ws2tcpip.h"
#   endif /* ACE_HAS_WINSOCK2 */

// error code mapping
#   define ETIME                   ERROR_SEM_TIMEOUT
#   define EWOULDBLOCK             WSAEWOULDBLOCK
#   define EINPROGRESS             WSAEINPROGRESS
#   define EALREADY                WSAEALREADY
#   define ENOTSOCK                WSAENOTSOCK
#   define EDESTADDRREQ            WSAEDESTADDRREQ
#   define EMSGSIZE                WSAEMSGSIZE
#   define EPROTOTYPE              WSAEPROTOTYPE
#   define ENOPROTOOPT             WSAENOPROTOOPT
#   define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
#   define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
#   define EOPNOTSUPP              WSAEOPNOTSUPP
#   define EPFNOSUPPORT            WSAEPFNOSUPPORT
#   define EAFNOSUPPORT            WSAEAFNOSUPPORT
#   define EADDRINUSE              WSAEADDRINUSE
#   define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
#   define ENETDOWN                WSAENETDOWN
#   define ENETUNREACH             WSAENETUNREACH
#   define ENETRESET               WSAENETRESET
#   define ECONNABORTED            WSAECONNABORTED
#   define ECONNRESET              WSAECONNRESET
#   define ENOBUFS                 WSAENOBUFS
#   define EISCONN                 WSAEISCONN
#   define ENOTCONN                WSAENOTCONN
#   define ESHUTDOWN               WSAESHUTDOWN
#   define ETOOMANYREFS            WSAETOOMANYREFS
#   define ETIMEDOUT               WSAETIMEDOUT
#   define ECONNREFUSED            WSAECONNREFUSED
#   define ELOOP                   WSAELOOP
#   define EHOSTDOWN               WSAEHOSTDOWN
#   define EHOSTUNREACH            WSAEHOSTUNREACH
#   define EPROCLIM                WSAEPROCLIM
#   define EUSERS                  WSAEUSERS
#   define EDQUOT                  WSAEDQUOT
#   define ESTALE                  WSAESTALE
#   define EREMOTE                 WSAEREMOTE
// Grrr! These two are already defined by the horrible 'standard'
// library.
// #define ENAMETOOLONG            WSAENAMETOOLONG
// #define ENOTEMPTY               WSAENOTEMPTY

#   if !defined (ACE_HAS_WINCE)
#     include /**/ <time.h>
#     include /**/ <direct.h>
#     include /**/ <process.h>
#     include /**/ <io.h>
#   endif /* ACE_HAS_WINCE */

#   if defined (__BORLANDC__)
#     include /**/ <fcntl.h>
#     define _chdir chdir
#     define _ftime ftime
#     define _access access
#     define _getcwd getcwd
#     define _isatty isatty
#     define _umask umask
#     define _fstat fstat
#     define _stat stat
#     define _stricmp stricmp
#     define _strnicmp strnicmp

#     define _timeb timeb

#     define _O_CREAT O_CREAT
#     define _O_EXCL  O_EXCL
#     define _O_TRUNC O_TRUNC
#     define _O_TEMPORARY 0x0800 // see fcntl.h
#   endif /* __BORLANDC__ */

typedef OVERLAPPED ACE_OVERLAPPED;
typedef DWORD ACE_thread_t;
typedef HANDLE ACE_hthread_t;
typedef long pid_t;
typedef DWORD ACE_thread_key_t;
#   if !defined (__BORLANDC__)
typedef DWORD nlink_t;
#   endif /* __BORLANDC__ */

// 64-bit quad-word definitions.
typedef unsigned __int64 ACE_QWORD;
typedef unsigned __int64 ACE_hrtime_t;
inline ACE_QWORD ACE_MAKE_QWORD (DWORD lo, DWORD hi) { return ACE_QWORD (lo) | (ACE_QWORD (hi) << 32); }
inline DWORD ACE_LOW_DWORD  (ACE_QWORD q) { return (DWORD) q; }
inline DWORD ACE_HIGH_DWORD (ACE_QWORD q) { return (DWORD) (q >> 32); }

// Win32 dummies to help compilation.

#   if !defined (__BORLANDC__)
typedef int mode_t;
typedef int uid_t;
typedef int gid_t;
#   endif /* __BORLANDC__ */
typedef char *caddr_t;
struct rlimit { };
struct t_call { };
struct t_bind { };
struct t_info { };
struct t_optmgmt { };
struct t_discon { };
struct t_unitdata { };
struct t_uderr { };
struct netbuf { };

// This is for file descriptors.
typedef HANDLE ACE_HANDLE;

// For Win32 compatibility.
typedef SOCKET ACE_SOCKET;

#   define ACE_INVALID_HANDLE INVALID_HANDLE_VALUE
#   define ACE_SYSCALL_FAILED 0xFFFFFFFF

// Needed to map calls to NT transparently.
#   define MS_ASYNC 0
#   define MS_INVALIDATE 0

// Reliance on CRT - I don't really like this.

#   define O_NDELAY    0
#   if !defined (MAXPATHLEN)
#     define MAXPATHLEN  _MAX_PATH
#   endif /* !MAXPATHLEN */
#   define MAXNAMLEN   _MAX_FNAME
#   define EADDRINUSE WSAEADDRINUSE

// The ordering of the fields in this struct is important.  It has to
// match those in WSABUF.
struct iovec
{
  size_t iov_len; // byte count to read/write
  char *iov_base; // data to be read/written
};

struct msghdr
{
  sockaddr * msg_name;
  // optional address

  int msg_namelen;
  // size of address

  iovec *msg_iov;
  /* scatter/gather array */

  int msg_iovlen;
  // # elements in msg_iov

  caddr_t msg_accrights;
  // access rights sent/received

  int msg_accrightslen;
};


typedef int ACE_idtype_t;
typedef DWORD ACE_id_t;
#   define ACE_SELF (0)
typedef int ACE_pri_t;

// Dynamic loading-related types - used for dlopen and family.
#   if !defined(RTLD_LAZY)
#     define RTLD_LAZY 1
#   endif /* !RTLD_LAZY */
typedef HINSTANCE ACE_SHLIB_HANDLE;
const ACE_SHLIB_HANDLE ACE_SHLIB_INVALID_HANDLE = 0;
const int ACE_DEFAULT_SHLIB_MODE = 0;

# elif defined (ACE_PSOS)

typedef ACE_UINT64 ACE_hrtime_t;

#   if defined (ACE_SIGINFO_IS_SIGINFO_T)
  typedef struct siginfo siginfo_t;
#   endif /* ACE_LACKS_SIGINFO_H */

# else /* !defined (ACE_WIN32) && !defined (ACE_PSOS) */

#   if (defined (ACE_HAS_UNICODE) && (defined (UNICODE)))
typedef const wchar_t *LPCTSTR;
typedef wchar_t *LPTSTR;
typedef wchar_t TCHAR;
#   else
typedef const char *LPCTSTR;
typedef char *LPTSTR;
typedef char TCHAR;
#   endif /* ACE_HAS_UNICODE && UNICODE */

#   if defined (m88k)
#     define RUSAGE_SELF 1
#   endif  /*  m88k */

// Default port is MAX_SHORT.
#   define ACE_MAX_DEFAULT_PORT 65535

// Default semaphore key
#   if !defined (ACE_DEFAULT_SEM_KEY)
#     define ACE_DEFAULT_SEM_KEY 1234
#   endif /* ACE_DEFAULT_SEM_KEY */

#   define ACE_INVALID_SEM_KEY -1

// Define the pathname separator characters for UNIX.
#   define ACE_DIRECTORY_SEPARATOR_STR_A "/"
#   define ACE_DIRECTORY_SEPARATOR_CHAR_A '/'

// We're some kind of UNIX...
#   define ACE_PLATFORM_A "UNIX"
#   define ACE_PLATFORM_EXE_SUFFIX_A ""

#   if defined (ACE_HAS_UNICODE)
#     define ACE_DIRECTORY_SEPARATOR_STR_W L"/"
#     define ACE_DIRECTORY_SEPARATOR_CHAR_W L'/'
#     define ACE_PLATFORM_W L"UNIX"
#     define ACE_PLATFORM_EXE_SUFFIX_W L""
#   else
#     define ACE_DIRECTORY_SEPARATOR_STR_W "/"
#     define ACE_DIRECTORY_SEPARATOR_CHAR_W '/'
#     define ACE_PLATFORM_W "UNIX"
#     define ACE_PLATFORM_EXE_SUFFIX_W ""
#   endif /* ACE_HAS_UNICODE */

#   if !defined (ACE_LD_SEARCH_PATH)
#     define ACE_LD_SEARCH_PATH "LD_LIBRARY_PATH"
#   endif /* ACE_LD_SEARCH_PATH */
#   if !defined (ACE_LD_SEARCH_PATH_SEPARATOR_STR)
#     define ACE_LD_SEARCH_PATH_SEPARATOR_STR ":"
#   endif /* ACE_LD_SEARCH_PATH_SEPARATOR_STR */

#   if !defined (ACE_DLL_SUFFIX)
#     define ACE_DLL_SUFFIX ".so"
#   endif /* ACE_DLL_SUFFIX */
#   if !defined (ACE_DLL_PREFIX)
#     define ACE_DLL_PREFIX "lib"
#   endif /* ACE_DLL_PREFIX */

// The following 3 defines are used by the ACE Name Server...
#   if !defined (ACE_DEFAULT_NAMESPACE_DIR_A)
#     define ACE_DEFAULT_NAMESPACE_DIR_A "/tmp"
#   endif /* ACE_DEFAULT_NAMESPACE_DIR_A */

#   if !defined (ACE_DEFAULT_LOCALNAME_A)
#     define ACE_DEFAULT_LOCALNAME_A "localnames"
#   endif /* ACE_DEFAULT_LOCALNAME_A */

#   if !defined (ACE_DEFAULT_GLOBALNAME_A)
#     define ACE_DEFAULT_GLOBALNAME_A "globalnames"
#   endif /* ACE_DEFAULT_GLOBALNAME_A */

#   if defined (ACE_HAS_UNICODE)
#     if !defined (ACE_DEFAULT_NAMESPACE_DIR_W)
#       define ACE_DEFAULT_NAMESPACE_DIR_W L"/tmp"
#     endif /* ACE_DEFAULT_NAMESPACE_DIR_W */
#     if !defined (ACE_DEFAULT_LOCALNAME_W)
#       define ACE_DEFAULT_LOCALNAME_W L"localnames"
#     endif /* ACE_DEFAULT_LOCALNAME_W */
#     if !defined (ACE_DEFAULT_GLOBALNAME_W)
#       define ACE_DEFAULT_GLOBALNAME_W L"globalnames"
#     endif /* ACE_DEFAULT_GLOBALNAME_W */
#   else
#     if !defined (ACE_DEFAULT_NAMESPACE_DIR_W)
#       define ACE_DEFAULT_NAMESPACE_DIR_W "/tmp"
#     endif /* ACE_DEFAULT_NAMESPACE_DIR_W */
#     if !defined (ACE_DEFAULT_LOCALNAME_W)
#       define ACE_DEFAULT_LOCALNAME_W "localnames"
#     endif /* ACE_DEFAULT_LOCALNAME_W */
#     if !defined (ACE_DEFAULT_GLOBALNAME_W)
#       define ACE_DEFAULT_GLOBALNAME_W "globalnames"
#     endif /* ACE_DEFAULT_GLOBALNAME_W */
#   endif /* ACE_HAS_UNICODE */

// Used for ACE_MMAP_Memory_Pool
#   if !defined (ACE_DEFAULT_BACKING_STORE)
#     define ACE_DEFAULT_BACKING_STORE "/tmp/ace-malloc-XXXXXX"
#   endif /* ACE_DEFAULT_BACKING_STORE */

// Used for ACE_FILE_Connector
#   if !defined (ACE_DEFAULT_TEMP_FILE)
#     define ACE_DEFAULT_TEMP_FILE "/tmp/ace-file-XXXXXX"
#   endif /* ACE_DEFAULT_TEMP_FILE */

// Used for logging
#   if !defined (ACE_DEFAULT_LOGFILE)
#     define ACE_DEFAULT_LOGFILE "/tmp/logfile"
#   endif /* ACE_DEFAULT_LOGFILE */

// Used for dynamic linking.
#   if !defined (ACE_DEFAULT_SVC_CONF)
#     define ACE_DEFAULT_SVC_CONF "./svc.conf"
#   endif /* ACE_DEFAULT_SVC_CONF */

// The following are #defines and #includes that are specific to UNIX.

#   define ACE_STDIN 0
#   define ACE_STDOUT 1
#   define ACE_STDERR 2

// Be consistent with Winsock naming
#   define ACE_INVALID_HANDLE -1
#   define ACE_SYSCALL_FAILED -1

#   define ACE_SEH_TRY if (1)
#   define ACE_SEH_EXCEPT(X) while (0)
#   define ACE_SEH_FINALLY if (1)

// The "null" device on UNIX.
#   define ACE_DEV_NULL "/dev/null"


// Wrapper for NT events on UNIX.
class ACE_Export ACE_event_t
{
  friend class ACE_OS;
protected:
  ACE_mutex_t lock_;
  // Protect critical section.

  ACE_cond_t condition_;
  // Keeps track of waiters.

  int manual_reset_;
  // Specifies if this is an auto- or manual-reset event.

  int is_signaled_;
  // "True" if signaled.

  u_long waiting_threads_;
  // Number of waiting threads.
};

// Provide compatibility with Windows NT.
typedef int ACE_HANDLE;
// For Win32 compatibility.
typedef ACE_HANDLE ACE_SOCKET;

struct ACE_OVERLAPPED
{
  u_long Internal;
  u_long InternalHigh;
  u_long Offset;
  u_long OffsetHigh;
  ACE_HANDLE hEvent;
};

// Add some typedefs and macros to enhance Win32 conformance...
#   if !defined (LPSECURITY_ATTRIBUTES)
#     define LPSECURITY_ATTRIBUTES int
#   endif /* !defined LPSECURITY_ATTRIBUTES */
#   if !defined (GENERIC_READ)
#     define GENERIC_READ 0
#   endif /* !defined GENERIC_READ */
#   if !defined (FILE_SHARE_READ)
#     define FILE_SHARE_READ 0
#   endif /* !defined FILE_SHARE_READ */
#   if !defined (OPEN_EXISTING)
#     define OPEN_EXISTING 0
#   endif /* !defined OPEN_EXISTING */
#   if !defined (FILE_ATTRIBUTE_NORMAL)
#     define FILE_ATTRIBUTE_NORMAL 0
#   endif /* !defined FILE_ATTRIBUTE_NORMAL */
#   if !defined (MAXIMUM_WAIT_OBJECTS)
#     define MAXIMUM_WAIT_OBJECTS 0
#   endif /* !defined MAXIMUM_WAIT_OBJECTS */
#   if !defined (FILE_FLAG_OVERLAPPED)
#     define FILE_FLAG_OVERLAPPED 0
#   endif /* !defined FILE_FLAG_OVERLAPPED */
#   if !defined (FILE_FLAG_SEQUENTIAL_SCAN)
#     define FILE_FLAG_SEQUENTIAL_SCAN 0
#   endif   /* FILE_FLAG_SEQUENTIAL_SCAN */

#   if defined (ACE_HAS_BROKEN_IF_HEADER)
struct ifafilt;
#   endif /* ACE_HAS_BROKEN_IF_HEADER */

#   if defined (ACE_HAS_AIX_BROKEN_SOCKET_HEADER)
#     undef __cplusplus
#     include /**/ <sys/socket.h>
#     define __cplusplus
#   else
#     include /**/ <sys/socket.h>
#   endif /* ACE_HAS_AIX_BROKEN_SOCKET_HEADER */

extern "C"
{
#   if defined (VXWORKS)
  struct  hostent {
    char    *h_name;        /* official name of host */
    char    **h_aliases;    /* aliases:  not used on VxWorks */
    int     h_addrtype;     /* host address type */
    int     h_length;       /* address length */
    char    **h_addr_list;  /* (first, only) address from name server */
#     define h_addr h_addr_list[0]   /* the first address */
  };
#   elif defined (ACE_HAS_CYGWIN32_SOCKET_H)
#     include /**/ <cygwin32/socket.h>
#   else
#     if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#       define queue _Queue_
#     endif /* ACE_HAS_STL_QUEUE_CONFLICT */
#     include /**/ <netdb.h>
#     if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#       undef queue
#     endif /* ACE_HAS_STL_QUEUE_CONFLICT */
#     include /**/ <netdb.h>
#   endif /* VXWORKS */

// This part if to avoid STL name conflict with the map structure
// in net/if.h.
#   if defined (ACE_HAS_STL_MAP_CONFLICT)
#     define map _Resource_Allocation_Map_
#   endif /* ACE_HAS_STL_MAP_CONFLICT */
#   include /**/ <net/if.h>
#   if defined (ACE_HAS_STL_MAP_CONFLICT)
#     undef map
#   endif /* ACE_HAS_STL_MAP_CONFLICT */

#   if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#     define queue _Queue_
#   endif /* ACE_HAS_STL_QUEUE_CONFLICT */
#   include /**/ <netinet/in.h>
#   if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#     undef queue
#   endif /* ACE_HAS_STL_QUEUE_CONFLICT */

#   if defined(VXWORKS) && defined(ghs)
// Works around a lack of proper prototypes for these functions on VxWorks
unsigned long inet_addr(const char *);
char    *inet_ntoa(const struct in_addr);
struct  in_addr inet_makeaddr(const int, const int);
unsigned long inet_network(const char *);
#   else
#     include /**/ <arpa/inet.h>
#   endif /* VXWORKS && ghs */
}
#   if !defined (ACE_LACKS_TCP_H)
#     include /**/ <netinet/tcp.h>
#   endif /* ACE_LACKS_TCP_H */

#   if defined (__Lynx__)
#     ifndef howmany
#       define howmany(x, y)   (((x)+((y)-1))/(y))
#     endif /* howmany */
#   endif /* __Lynx__ */

#   if defined (CYGWIN32)
#     include /**/ <sys/uio.h>
#     include /**/ <sys/file.h>
#     include /**/ <sys/time.h>
#     include /**/ <sys/resource.h>
#     include /**/ <sys/wait.h>
#     include /**/ <pwd.h>
#   elif defined (CHORUS)
#     include /**/ <chorus.h>
#     include /**/ <cx/select.h>
#     include /**/ <sys/uio.h>
#     include /**/ <time.h>
#     include /**/ <stdfileio.h>
#     include /**/ <am/afexec.h>
#     include /**/ <sys/types.h>
#     include /**/ <sys/signal.h>
#     include /**/ <sys/wait.h>
#     include /**/ <pwd.h>
#     include /**/ <timer/chBench.h>
extern_C int      getgid          __((void));
extern_C int      getuid          __((void));
extern_C char*    getcwd          __((char* buf, size_t size));
extern_C int      pipe            __((int* fildes));
extern_C int      gethostname     __((char*, size_t));

// This must come after limits.h is included
#     define MAXPATHLEN _POSIX_PATH_MAX

typedef cx_fd_mask fd_mask;
#     ifndef howmany
#       define howmany(x, y)   (((x)+((y)-1))/(y))
#     endif /* howmany */
typedef void (*__sighandler_t)(int); // keep Signal compilation happy
#   elif ! defined (VXWORKS)
#     include /**/ <sys/uio.h>
#     include /**/ <sys/ipc.h>
#     include /**/ <sys/shm.h>
#     include /**/ <sys/sem.h>
#     include /**/ <sys/file.h>
#     include /**/ <sys/time.h>
#     include /**/ <sys/resource.h>
#     include /**/ <sys/wait.h>
#     include /**/ <pwd.h>
#   endif /* ! VXWORKS */
#   include /**/ <sys/ioctl.h>
#   include /**/ <dirent.h>

// IRIX5 defines bzero() in this odd file...
#   if defined (ACE_HAS_BSTRING)
#     include /**/ <bstring.h>
#   endif /* ACE_HAS_BSTRING */

// AIX defines bzero() in this odd file...
#   if defined (ACE_HAS_STRINGS)
#     include /**/ <strings.h>
#   endif /* ACE_HAS_STRINGS */

#   if defined (ACE_HAS_TERM_IOCTLS)
#     include /**/ <sys/termios.h>
#   endif /* ACE_HAS_TERM_IOCTLS */

#   if !defined (ACE_LACKS_UNISTD_H)
#     include /**/ <unistd.h>
#   endif /* ACE_LACKS_UNISTD_H */

#   if defined (ACE_HAS_AIO_CALLS)
#     include /**/ <aio.h>
#   endif /* ACE_HAS_AIO_CALLS */

#   if !defined (ACE_LACKS_PARAM_H)
#     include /**/ <sys/param.h>
#   endif /* ACE_LACKS_PARAM_H */

#   if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS) && !defined (VXWORKS)
#     include /**/ <sys/un.h>
#   endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#   if defined (ACE_HAS_SIGINFO_T)
#     if !defined (ACE_LACKS_SIGINFO_H)
#       include /**/ <siginfo.h>
#     endif /* ACE_LACKS_SIGINFO_H */
#     if !defined (ACE_LACKS_UCONTEXT_H)
#       include /**/ <ucontext.h>
#     endif /* ACE_LACKS_UCONTEXT_H */
#   endif /* ACE_HAS_SIGINFO_T */

#   if defined (ACE_HAS_POLL)
#     include /**/ <poll.h>
#   endif /* ACE_HAS_POLL */

#   if defined (ACE_HAS_STREAMS)
#     if defined (AIX)
#       if !defined (_XOPEN_EXTENDED_SOURCE)
#         define _XOPEN_EXTENDED_SOURCE
#       endif /* !_XOPEN_EXTENDED_SOURCE */
#       include /**/ <stropts.h>
#       undef _XOPEN_EXTENDED_SOURCE
#     else
#       include /**/ <stropts.h>
#     endif /* AIX */
#   endif /* ACE_HAS_STREAMS */

#   if defined (ACE_LACKS_T_ERRNO)
extern int t_errno;
#   endif /* ACE_LACKS_T_ERRNO */

#   if defined (DIGITAL_UNIX)
  // sigwait is yet another macro on Digital UNIX 4.0, just causing
  // trouble when introducing member functions with the same name.
  // Thanks to Thilo Kielmann" <kielmann@informatik.uni-siegen.de> for
  // this fix.
#     undef sigwait
#     if defined  (__DECCXX_VER)
    // cxx on Digital Unix 4.0 needs this declaration.  With it,
    // ::_Psigwait () works with cxx -pthread.  g++ does _not_ need it.
    extern "C" int _Psigwait __((const sigset_t *set, int *sig));
#     endif /* __DECCXX_VER */
#   elif !defined (ACE_HAS_SIGWAIT)
  extern "C" int sigwait (sigset_t *set);
#   endif /* ! DIGITAL_UNIX && ! ACE_HAS_SIGWAIT */

#   if defined (ACE_HAS_SELECT_H)
#     include /**/ <sys/select.h>
#   endif /* ACE_HAS_SELECT_H */

#   if defined (ACE_HAS_ALLOCA_H)
#     include /**/ <alloca.h>
#   endif /* ACE_HAS_ALLOCA_H */

#   if defined (ACE_HAS_TIUSER_H) || defined (ACE_HAS_XTI)
#     if defined (ACE_HAS_TIUSER_H_BROKEN_EXTERN_C)
extern "C" {
#     endif /* ACE_HAS_TIUSER_H_BROKEN_EXTERN_C */
#     if defined (ACE_HAS_TIUSER_H)
#       include /**/ <tiuser.h>
#     else
#       if defined (ACE_HAS_BROKEN_XTI_MACROS)
#         undef TCP_NODELAY
#         undef TCP_MAXSEG
#       endif /* ACE_HAS_BROKEN_XTI_MACROS */
#       if defined ACE_HAS_SYS_XTI_H
#         define class ace_xti_class
#         include /**/ <sys/xti.h>
#         undef class
#       else
#         include /**/ <xti.h>
#       endif /* ACE_HAS_SYS_XTI_H */
#     endif /* ACE_HAS_TIUSER_H */
#     if defined (ACE_HAS_TIUSER_H_BROKEN_EXTERN_C)
}
#     endif /* ACE_HAS_TIUSER_H_BROKEN_EXTERN_C */
#   endif /* ACE_HAS_TIUSER_H || ACE_HAS_XTI */

/* Set the proper handle type for dynamically-loaded libraries. */
/* Also define a default 'mode' for loading a library - the names and values */
/* differ between OSes, so if you write code that uses the mode, be careful */
/* of the platform differences. */
#   if defined (ACE_HAS_SVR4_DYNAMIC_LINKING)
#     if defined (ACE_HAS_DLFCN_H_BROKEN_EXTERN_C)
extern "C" {
#     endif /* ACE_HAS_DLFCN_H_BROKEN_EXTERN_C */
#     include /**/ <dlfcn.h>
#     if defined (ACE_HAS_DLFCN_H_BROKEN_EXTERN_C)
}
#     endif /* ACE_HAS_DLFCN_H_BROKEN_EXTERN_C */
  typedef void *ACE_SHLIB_HANDLE;
  const ACE_SHLIB_HANDLE ACE_SHLIB_INVALID_HANDLE = 0;
#     if !defined (RTLD_LAZY)
#       define RTLD_LAZY 1
#     endif /* !RTLD_LAZY */
  const int ACE_DEFAULT_SHLIB_MODE = RTLD_LAZY;
#   elif defined (__hpux)
#     if defined(__GNUC__) || __cplusplus >= 199707L
#       include /**/ <dl.h>
#     else
#       include /**/ <cxxdl.h>
#     endif /* (g++ || HP aC++) vs. HP C++ */
  typedef shl_t ACE_SHLIB_HANDLE;
  const ACE_SHLIB_HANDLE ACE_SHLIB_INVALID_HANDLE = 0;
  const int ACE_DEFAULT_SHLIB_MODE = BIND_DEFERRED;
#   else
#     if !defined(RTLD_LAZY)
#       define RTLD_LAZY 1
#     endif /* !RTLD_LAZY */
  typedef void *ACE_SHLIB_HANDLE;
  const ACE_SHLIB_HANDLE ACE_SHLIB_INVALID_HANDLE = 0;
  const int ACE_DEFAULT_SHLIB_MODE = RTLD_LAZY;
#   endif /* ACE_HAS_SVR4_DYNAMIC_LINKING */

#   if defined (ACE_HAS_SOCKIO_H)
#     include /**/ <sys/sockio.h>
#   endif /* ACE_HAS_SOCKIO_ */

// There must be a better way to do this...
#   if !defined (RLIMIT_NOFILE)
#     if defined (linux) || defined (AIX) || defined (SCO)
#       if defined (RLIMIT_OFILE)
#         define RLIMIT_NOFILE RLIMIT_OFILE
#       else
#         define RLIMIT_NOFILE 200
#       endif /* RLIMIT_OFILE */
#     endif /* defined (linux) || defined (AIX) || defined (SCO) */
#   endif /* RLIMIT_NOFILE */

#   if !defined (ACE_HAS_TLI_PROTOTYPES)
// Define ACE_TLI headers for systems that don't prototype them....
extern "C"
{
  int t_accept(int fildes, int resfd, struct t_call *call);
  char *t_alloc(int fildes, int struct_type, int fields);
  int t_bind(int fildes, struct t_bind *req, struct t_bind *ret);
  int t_close(int fildes);
  int t_connect(int fildes, struct t_call *sndcall,
                struct t_call *rcvcall);
  void t_error(char *errmsg);
  int t_free(char *ptr, int struct_type);
  int t_getinfo(int fildes, struct t_info *info);
  int t_getname (int fildes, struct netbuf *namep, int type);
  int t_getstate(int fildes);
  int t_listen(int fildes, struct t_call *call);
  int t_look(int fildes);
  int t_open(char *path, int oflag, struct t_info *info);
  int t_optmgmt(int fildes, struct t_optmgmt *req,
                struct t_optmgmt *ret);
  int t_rcv(int fildes, char *buf, u_int nbytes, int *flags);
  int t_rcvconnect(int fildes, struct t_call *call);
  int t_rcvdis(int fildes, struct t_discon *discon);
  int t_rcvrel(int fildes);
  int t_rcvudata(int fildes, struct t_unitdata *unitdata, int *flags);
  int t_rcvuderr(int fildes, struct t_uderr *uderr);
  int t_snd(int fildes, char *buf, u_int nbytes, int flags);
  int t_snddis(int fildes, struct t_call *call);
  int t_sndrel(int fildes);
  int t_sndudata(int fildes, struct t_unitdata *unitdata);
  int t_sync(int fildes);
  int t_unbind(int fildes);
}
#   endif /* !ACE_HAS_TLI_PROTOTYPES */

#   if defined (ACE_LACKS_MMAP)
#     define PROT_READ 0
#     define PROT_WRITE 0
#     define PROT_EXEC 0
#     define PROT_NONE 0
#     define PROT_RDWR 0
#     define MAP_PRIVATE 0
#     define MAP_SHARED 0
#     define MAP_FIXED 0
#   endif /* ACE_LACKS_MMAP */

// Fixes a problem with HP/UX.
#   if defined (ACE_HAS_BROKEN_MMAP_H)
extern "C"
{
#     include /**/ <sys/mman.h>
}
#   elif !defined (ACE_LACKS_MMAP)
#     include /**/ <sys/mman.h>
#   endif /* ACE_HAS_BROKEN_MMAP_H */

// OSF1 has problems with sys/msg.h and C++...
#   if defined (ACE_HAS_BROKEN_MSG_H)
#     define _KERNEL
#   endif /* ACE_HAS_BROKEN_MSG_H */
#   if !defined (ACE_LACKS_SYSV_MSG_H)
#     include /**/ <sys/msg.h>
#   endif /* ACE_LACKS_SYSV_MSG_H */
#   if defined (ACE_HAS_BROKEN_MSG_H)
#     undef _KERNEL
#   endif /* ACE_HAS_BROKEN_MSG_H */

#   if defined (ACE_LACKS_SYSV_MSQ_PROTOS)
extern "C"
{
  int msgget (key_t, int);
  int msgrcv (int, void *, size_t, long, int);
  int msgsnd (int, const void *, size_t, int);
  int msgctl (int, int, struct msqid_ds *);
}
#   endif /* ACE_LACKS_SYSV_MSQ_PROTOS */

#   if defined (ACE_HAS_PRIOCNTL)
#     include /**/ <sys/priocntl.h>
#   endif /* ACE_HAS_PRIOCNTL */

#   if defined (ACE_HAS_IDTYPE_T)
  typedef idtype_t ACE_idtype_t;
#   else
  typedef int ACE_idtype_t;
#   endif /* ACE_HAS_IDTYPE_T */

#   if defined (ACE_HAS_STHREADS) || defined (DIGITAL_UNIX)
#     if defined (ACE_LACKS_PRI_T)
    typedef int pri_t;
#     endif /* ACE_LACKS_PRI_T */
  typedef id_t ACE_id_t;
#     define ACE_SELF P_MYID
  typedef pri_t ACE_pri_t;
#   else  /* ! ACE_HAS_STHREADS && ! DIGITAL_UNIX */
  typedef long ACE_id_t;
#     define ACE_SELF (-1)
  typedef short ACE_pri_t;
#   endif /* ! ACE_HAS_STHREADS && ! DIGITAL_UNIX */

#   if defined (ACE_HAS_HI_RES_TIMER) &&  !defined (ACE_LACKS_LONGLONG_T)
  /* hrtime_t is defined on systems (Suns) with ACE_HAS_HI_RES_TIMER */
  typedef hrtime_t ACE_hrtime_t;
#   else  /* ! ACE_HAS_HI_RES_TIMER  ||  ACE_LACKS_LONGLONG_T */
  typedef ACE_UINT64 ACE_hrtime_t;
#   endif /* ! ACE_HAS_HI_RES_TIMER  ||  ACE_LACKS_LONGLONG_T */

# endif /* !defined (ACE_WIN32) && !defined (ACE_PSOS) */

# if !defined (ACE_DEFAULT_SYNCH_TYPE)
#   define ACE_DEFAULT_SYNCH_TYPE USYNC_THREAD
# endif /* ! ACE_DEFAULT_SYNCH_TYPE */

# if !defined (ACE_MAP_PRIVATE)
#   define ACE_MAP_PRIVATE MAP_PRIVATE
# endif /* ! ACE_MAP_PRIVATE */

# if !defined (ACE_MAP_SHARED)
#   define ACE_MAP_SHARED MAP_SHARED
# endif /* ! ACE_MAP_SHARED */

# if !defined (ACE_MAP_FIXED)
#   define ACE_MAP_FIXED MAP_FIXED
# endif /* ! ACE_MAP_FIXED */

# if defined (ACE_LACKS_UTSNAME_T)
#   define _SYS_NMLN 257
struct utsname
{
  TCHAR sysname[_SYS_NMLN];
  TCHAR nodename[_SYS_NMLN];
  TCHAR release[_SYS_NMLN];
  TCHAR version[_SYS_NMLN];
  TCHAR machine[_SYS_NMLN];
};
# else
#   include /**/ <sys/utsname.h>
# endif /* ACE_LACKS_UTSNAME_T */

// Increase the range of "address families".  Please note that this
// must appear _after_ the include of sys/socket.h, for the AF_FILE
// definition on Linux/glibc2.
# define AF_ANY (-1)
# define AF_SPIPE (AF_MAX + 1)
# if !defined (AF_FILE)
#   define AF_FILE (AF_MAX + 2)
# endif /* ! AF_FILE */
# define AF_DEV (AF_MAX + 3)
# define AF_UPIPE (AF_SPIPE)

# if defined (ACE_SELECT_USES_INT)
typedef int ACE_FD_SET_TYPE;
# else
typedef fd_set ACE_FD_SET_TYPE;
# endif /* ACE_SELECT_USES_INT */

# if !defined (MAXNAMELEN)
#   if defined (FILENAME_MAX)
#     define MAXNAMELEN FILENAME_MAX
#   else
#     define MAXNAMELEN 256
#   endif /* FILENAME_MAX */
# endif /* MAXNAMELEN */

# if !defined(MAXHOSTNAMELEN)
#   define MAXHOSTNAMELEN  256
# endif /* MAXHOSTNAMELEN */

# if defined (ACE_LACKS_SIGSET)
typedef u_int sigset_t;
# endif /* ACE_LACKS_SIGSET */

# if defined (ACE_LACKS_SIGACTION)
struct sigaction
{
  int sa_flags;
  ACE_SignalHandlerV sa_handler;
  sigset_t sa_mask;
};
# endif /* ACE_LACKS_SIGACTION */

# if !defined (SIGHUP)
#   define SIGHUP 0
# endif /* SIGHUP */

# if !defined (SIGINT)
#   define SIGINT 0
# endif /* SIGINT */

# if !defined (SIGSEGV)
#   define SIGSEGV 0
# endif /* SIGSEGV */

# if !defined (SIGIO)
#   define SIGIO 0
# endif /* SIGSEGV */

# if !defined (SIGUSR1)
#   define SIGUSR1 0
# endif /* SIGUSR1 */

# if !defined (SIGUSR2)
#   define SIGUSR2 0
# endif /* SIGUSR2 */

# if !defined (SIGCHLD)
#   define SIGCHLD 0
# endif /* SIGCHLD */

# if !defined (SIGCLD)
#   define SIGCLD SIGCHLD
# endif /* SIGCLD */

# if !defined (SIGQUIT)
#   define SIGQUIT 0
# endif /* SIGQUIT */

# if !defined (SIGPIPE)
#   define SIGPIPE 0
# endif /* SIGPIPE */

# if !defined (SIGALRM)
#   define SIGALRM 0
# endif /* SIGALRM */

# if !defined (SIG_DFL)
#   define SIG_DFL ((__sighandler_t) 0)
# endif /* SIG_DFL */

# if !defined (SIG_IGN)
#   define SIG_IGN ((__sighandler_t) 1)     /* ignore signal */
# endif /* SIG_IGN */

# if !defined (SIG_ERR)
#   define SIG_ERR ((__sighandler_t) -1)    /* error return from signal */
# endif /* SIG_ERR */

# if !defined (O_NONBLOCK)
#   define O_NONBLOCK  1
# endif /* O_NONBLOCK  */

# if !defined (SIG_BLOCK)
#   define SIG_BLOCK   1
# endif /* SIG_BLOCK   */

# if !defined (SIG_UNBLOCK)
#   define SIG_UNBLOCK 2
# endif /* SIG_UNBLOCK */

# if !defined (SIG_SETMASK)
#   define SIG_SETMASK 3
# endif /* SIG_SETMASK */

# if !defined (IPC_CREAT)
#   define IPC_CREAT 0
# endif /* IPC_CREAT */

# if !defined (IPC_NOWAIT)
#   define IPC_NOWAIT 0
# endif /* IPC_NOWAIT */

# if !defined (IPC_RMID)
#   define IPC_RMID 0
# endif /* IPC_RMID */

# if !defined (IPC_EXCL)
#   define IPC_EXCL 0
# endif /* IPC_EXCL */

# if !defined (IP_DROP_MEMBERSHIP)
#   define IP_DROP_MEMBERSHIP 0
# endif /* IP_DROP_MEMBERSHIP */

# if !defined (IP_ADD_MEMBERSHIP)
#   define IP_ADD_MEMBERSHIP 0
#   define ACE_LACKS_IP_ADD_MEMBERSHIP
# endif /* IP_ADD_MEMBERSHIP */

# if !defined (SIOCGIFBRDADDR)
#   define SIOCGIFBRDADDR 0
# endif /* SIOCGIFBRDADDR */

# if !defined (SIOCGIFADDR)
#   define SIOCGIFADDR 0
# endif /* SIOCGIFADDR */

# if !defined (IPC_PRIVATE)
#   define IPC_PRIVATE ACE_INVALID_SEM_KEY
# endif /* IPC_PRIVATE */

# if !defined (IPC_STAT)
#   define IPC_STAT 0
# endif /* IPC_STAT */

# if !defined (GETVAL)
#   define GETVAL 0
# endif /* GETVAL */

# if !defined (F_GETFL)
#   define F_GETFL 0
# endif /* F_GETFL */

# if !defined (SETVAL)
#   define SETVAL 0
# endif /* SETVAL */

# if !defined (GETALL)
#   define GETALL 0
# endif /* GETALL */

# if !defined (SETALL)
#   define SETALL 0
# endif /* SETALL */

# if !defined (SEM_UNDO)
#   define SEM_UNDO 0
# endif /* SEM_UNDO */

# if defined (__Lynx__)
  // LynxOS sets NSIG to the highest-numbered signal.
#   define ACE_NSIG (NSIG + 1)
# else
  // All other platforms set NSIG to one greater than the
  // highest-numbered signal.
#   define ACE_NSIG NSIG
# endif /* __Lynx__ */

# if !defined (R_OK)
#   define R_OK    04      /* Test for Read permission. */
# endif /* R_OK */

# if !defined (W_OK)
#   define W_OK    02      /* Test for Write permission. */
# endif /* W_OK */

# if !defined (X_OK)
#   define X_OK    01      /* Test for eXecute permission. */
# endif /* X_OK */

# if !defined (F_OK)
#   define F_OK    0       /* Test for existence of File. */
# endif /* F_OK */

# if !defined (ESUCCESS)
#   define ESUCCESS 0
# endif /* !ESUCCESS */

# if !defined (EIDRM)
#   define EIDRM 0
# endif /* !EIDRM */

# if !defined (ENOSYS)
#   define ENOSYS EFAULT /* Operation not supported or unknown error. */
# endif /* !ENOSYS */

# if !defined (ENOTSUP)
#   define ENOTSUP ENOSYS  /* Operation not supported. */
# endif /* !ENOTSUP */

# if !defined (WNOHANG)
#   define WNOHANG 42
# endif /* !WNOHANG */

# if !defined (EDEADLK)
#   define EDEADLK 1000 /* Some large number.... */
# endif /* !EDEADLK */

# if !defined (MS_SYNC)
#   define MS_SYNC 0x0
# endif /* !MS_SYNC */

# if !defined (PIPE_BUF)
#   define PIPE_BUF 5120
# endif /* PIPE_BUF */

# if !defined (PROT_RDWR)
#   define PROT_RDWR (PROT_READ|PROT_WRITE)
# endif /* PROT_RDWR */

# if !defined (WNOHANG)
#   define WNOHANG 0
# endif /* WNOHANG */

# if defined (ACE_HAS_POSIX_NONBLOCK)
#   define ACE_NONBLOCK O_NONBLOCK
# else
#   define ACE_NONBLOCK O_NDELAY
# endif /* ACE_HAS_POSIX_NONBLOCK */

// These are used by the <ACE_IPC_SAP::enable> and
// <ACE_IPC_SAP::disable> methods.  They must be unique and cannot
// conflict with the value of <ACE_NONBLOCK>.  We make the numbers
// negative here so they won't conflict with other values like SIGIO,
// etc.
# define ACE_SIGIO -1
# define ACE_SIGURG -2
# define ACE_CLOEXEC -3

# define LOCALNAME 0
# define REMOTENAME 1

# if !defined (ETIMEDOUT) && defined (ETIME)
#   define ETIMEDOUT ETIME
# endif /* ETIMEDOUT */

# if !defined (ETIME) && defined (ETIMEDOUT)
#   define ETIME ETIMEDOUT
# endif /* ETIMED */

# if !defined (EBUSY)
#   define EBUSY ETIME
# endif /* EBUSY */

# if !defined (_SC_TIMER_MAX)
#   define _SC_TIMER_MAX 44
# endif /* _SC_TIMER_MAX */

// Default number of <ACE_Event_Handler>s supported by
// <ACE_Timer_Heap>.
# if !defined (ACE_DEFAULT_TIMERS)
#   define ACE_DEFAULT_TIMERS _SC_TIMER_MAX
# endif /* ACE_DEFAULT_TIMERS */

# if defined (ACE_HAS_STRUCT_NETDB_DATA)
typedef char ACE_HOSTENT_DATA[sizeof(struct hostent_data)];
typedef char ACE_SERVENT_DATA[sizeof(struct servent_data)];
typedef char ACE_PROTOENT_DATA[sizeof(struct protoent_data)];
# else
#   if !defined ACE_HOSTENT_DATA_SIZE
#     define ACE_HOSTENT_DATA_SIZE (4*1024)
#   endif /*ACE_HOSTENT_DATA_SIZE */
#   if !defined ACE_SERVENT_DATA_SIZE
#     define ACE_SERVENT_DATA_SIZE (4*1024)
#   endif /*ACE_SERVENT_DATA_SIZE */
#   if !defined ACE_PROTOENT_DATA_SIZE
#     define ACE_PROTOENT_DATA_SIZE (2*1024)
#   endif /*ACE_PROTOENT_DATA_SIZE */
typedef char ACE_HOSTENT_DATA[ACE_HOSTENT_DATA_SIZE];
typedef char ACE_SERVENT_DATA[ACE_SERVENT_DATA_SIZE];
typedef char ACE_PROTOENT_DATA[ACE_PROTOENT_DATA_SIZE];
# endif /* ACE_HAS_STRUCT_NETDB_DATA */

# if !defined (ACE_HAS_SEMUN) || (defined (__GLIBC__) && defined (_SEM_SEMUN_UNDEFINED))
union semun
{
  int val; // value for SETVAL
  struct semid_ds *buf; // buffer for IPC_STAT & IPC_SET
  u_short *array; // array for GETALL & SETALL
};
# endif /* !ACE_HAS_SEMUN || (defined (__GLIBC__) && defined (_SEM_SEMUN_UNDEFINED)) */

// Max size of an ACE Log Record data buffer.  This can be reset in
// the config.h file if you'd like to increase or decrease the size.
# if !defined (ACE_MAXLOGMSGLEN)
#   define ACE_MAXLOGMSGLEN 4 * 1024
# endif /* ACE_MAXLOGMSGLEN */

// Max size of an ACE Token.
# define ACE_MAXTOKENNAMELEN 40

// Max size of an ACE Token client ID.
# define ACE_MAXCLIENTIDLEN MAXHOSTNAMELEN + 20

// Create some useful typedefs.
typedef const char **SYS_SIGLIST;
typedef void *(*ACE_THR_FUNC)(void *);
// This is for C++ static methods.
# if defined (VXWORKS)
typedef FUNCPTR ACE_THR_FUNC_INTERNAL;  // where typedef int (*FUNCPTR) (...)
# elif defined (ACE_PSOS)
typedef void (*ACE_THR_FUNC_INTERNAL)(void *);
# else
typedef ACE_THR_FUNC ACE_THR_FUNC_INTERNAL;
# endif /* VXWORKS */

extern "C" {
typedef void (*ACE_THR_C_DEST)(void *);
}
typedef void (*ACE_THR_DEST)(void *);

extern "C"
{
# if defined (VXWORKS)
typedef FUNCPTR ACE_THR_C_FUNC;  // where typedef int (*FUNCPTR) (...)
# elif defined (ACE_PSOS)
// needed to handle task entry point type inconsistencies in pSOS+
typedef void (*PSOS_TASK_ENTRY_POINT)();
typedef void (*ACE_THR_C_FUNC)(void *);
# else
typedef void *(*ACE_THR_C_FUNC)(void *);
# endif /* VXWORKS */
}

# if !defined (MAP_FAILED)
#   define MAP_FAILED ((void *) -1)
# elif defined (ACE_HAS_LONG_MAP_FAILED)
#   undef MAP_FAILED
#   define MAP_FAILED ((void *) -1L)
# endif /* MAP_FAILED */

# if defined (ACE_HAS_CHARPTR_DL)
typedef char * ACE_DL_TYPE;
#   if defined (ACE_HAS_UNICODE)
typedef wchar_t * ACE_WIDE_DL_TYPE;
#   endif /* ACE_HAS_UNICODE */
# else
typedef const char * ACE_DL_TYPE;
#   if defined (ACE_HAS_UNICODE)
typedef const wchar_t * ACE_WIDE_DL_TYPE;
#   endif /* ACE_HAS_UNICODE */
# endif /* ACE_HAS_CHARPTR_DL */

# if !defined (ACE_HAS_SIGINFO_T)
struct ACE_Export siginfo_t
{
  siginfo_t (ACE_HANDLE handle);
  siginfo_t (ACE_HANDLE *handles);      // JCEJ 12/23/96

  ACE_HANDLE si_handle_;
  // Win32 HANDLE that has become signaled.

  ACE_HANDLE *si_handles_;
  // Array of Win32 HANDLEs all of which have become signaled.
};
# endif /* ACE_HAS_SIGINFO_T */

# if !defined (ACE_HAS_UCONTEXT_T)
typedef int ucontext_t;
# endif /* ACE_HAS_UCONTEXT_T */

# if !defined (SA_SIGINFO)
#   define SA_SIGINFO 0
# endif /* SA_SIGINFO */

# if !defined (SA_RESTART)
#   define SA_RESTART 0
# endif /* SA_RESTART */

# if defined (ACE_HAS_TIMOD_H)
#   if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#     define queue _Queue_
#   endif /* ACE_HAS_STL_QUEUE_CONFLICT */
#   include /**/ <sys/timod.h>
#   if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#     undef queue
#   endif /* ACE_HAS_STL_QUEUE_CONFLICT */
# elif defined (ACE_HAS_OSF_TIMOD_H)
#   include /**/ <tli/timod.h>
# endif /* ACE_HAS_TIMOD_H */

# if defined (ACE_HAS_BROKEN_T_ERRNO)
#   undef t_errno
# endif /* ACE_HAS_BROKEN_T_ERRNO */

# if defined rewinddir
#   undef rewinddir
# endif /* rewinddir */

class ACE_Export ACE_Thread_ID
{
  // = TITLE
  //     Defines a platform-independent thread ID.
public:
  // = Initialization method.
  ACE_Thread_ID (ACE_thread_t, ACE_hthread_t);
  ACE_Thread_ID (const ACE_Thread_ID &id);

  // = Set/Get the Thread ID.
  ACE_thread_t id (void);
  void id (ACE_thread_t);

  // = Set/Get the Thread handle.
  ACE_hthread_t handle (void);
  void handle (ACE_hthread_t);

  // != Comparison operator.
  int operator== (const ACE_Thread_ID &) const;
  int operator!= (const ACE_Thread_ID &) const;

private:
  ACE_thread_t thread_id_;
  // Identify the thread.

  ACE_hthread_t thread_handle_;
  // Handle to the thread (typically used to "wait" on Win32).
};

// Type of the extended signal handler.
typedef void (*ACE_Sig_Handler_Ex) (int, siginfo_t *siginfo, ucontext_t *ucontext);

// If the xti.h file redefines the function names, do it now, else
// when the functigon definitions are encountered, they won't match the
// declaration here.

# if defined (ACE_REDEFINES_XTI_FUNCTIONS)
#   include /**/ <xti.h>
#   if defined (UNIXWARE_2_0)         /* They apparantly forgot one... */
extern "C" int _xti_error(char *);
#   endif /* UNIXWARE */
# endif /* ACE_REDEFINES_XTI_FUNCTIONS */

// = The ACE_Sched_Priority type should be used for platform-
//   independent thread and process priorities, by convention.
//   int should be used for OS-specific priorities.
typedef int ACE_Sched_Priority;

// forward declaration
class ACE_Sched_Params;

# if defined (ACE_LACKS_FILELOCKS)
#   if ! defined (VXWORKS) && ! defined (ACE_PSOS)
// VxWorks defines struct flock in sys/fcntlcom.h.  But it doesn't
// appear to support flock ().
struct flock
{
  short l_type;
  short l_whence;
  off_t l_start;
  off_t l_len;          /* len == 0 means until end of file */
  long  l_sysid;
  pid_t l_pid;
  long  l_pad[4];               /* reserve area */
};
#   endif /* ! VXWORKS */
# endif /* ACE_LACKS_FILELOCKS */

# if !defined (ACE_HAS_IP_MULTICAST)  &&  defined (ACE_LACKS_IP_ADD_MEMBERSHIP)
  // Even if ACE_HAS_IP_MULTICAST is not defined, if IP_ADD_MEMBERSHIP
  // is defined, assume that the ip_mreq struct is also defined
  // (presumably in netinet/in.h).
  struct ip_mreq
  {
    struct in_addr imr_multiaddr;
    // IP multicast address of group
    struct in_addr imr_interface;
    // local IP address of interface
  };
# endif /* ! ACE_HAS_IP_MULTICAST  &&  ACE_LACKS_IP_ADD_MEMBERSHIP */

# if !defined (ACE_HAS_STRBUF_T)
struct strbuf
{
  int maxlen; // no. of bytes in buffer.
  int len;    // no. of bytes returned.
  void *buf;  // pointer to data.
};
# endif /* ACE_HAS_STRBUF_T */

class ACE_Export ACE_Str_Buf : public strbuf
{
  // = TITLE
  //     Simple wrapper for STREAM pipes strbuf.
public:
  // = Initialization method
  ACE_Str_Buf (void *b = 0, int l = 0, int max = 0);
  // Constructor.

  ACE_Str_Buf (strbuf &);
  // Constructor.
};

# if defined (ACE_HAS_BROKEN_BITSHIFT)
  // This might not be necessary any more:  it was added prior to the
  // (fd_mask) cast being added to the version below.  Maybe that cast
  // will fix the problem on tandems.    Fri Dec 12 1997 David L. Levine
#   define ACE_MSB_MASK (~(ACE_UINT32 (1) << ACE_UINT32 (NFDBITS - 1)))
# else
  // This needs to go here to avoid overflow problems on some compilers.
#   if defined (ACE_WIN32)
    //  Does ACE_WIN32 have an fd_mask?
#     define ACE_MSB_MASK (~(1 << (NFDBITS - 1)))
#   else  /* ! ACE_WIN32 */
#     define ACE_MSB_MASK (~((fd_mask) 1 << (NFDBITS - 1)))
#   endif /* ! ACE_WIN32 */
# endif /* ACE_HAS_BROKEN_BITSHIFT */

// Signature for registering a cleanup function that is used by the
// <ACE_Object_Manager> and the <ACE_Thread_Manager>.
# if defined (ACE_HAS_SIG_C_FUNC)
extern "C" {
# endif /* ACE_HAS_SIG_C_FUNC */
typedef void (*ACE_CLEANUP_FUNC)(void *object, void *param) /* throw () */;
# if defined (ACE_HAS_SIG_C_FUNC)
}
# endif /* ACE_HAS_SIG_C_FUNC */

// For use by ACE_OS::exit ().
extern "C"
{
  typedef void (*ACE_EXIT_HOOK) ();
}

# if defined (ACE_WIN32)
// Default WIN32 structured exception handler.
int ACE_SEH_Default_Exception_Selector (void *);
int ACE_SEH_Default_Exception_Handler (void *);
# endif /* ACE_WIN32 */

class ACE_Export ACE_Cleanup
{
  // = TITLE
  //    Base class for objects that are cleaned by ACE_Object_Manager.
public:
  ACE_Cleanup (void);
  // No-op constructor.

  virtual ~ACE_Cleanup (void);
  // Destructor.

  virtual void cleanup (void *param = 0);
  // Cleanup method that, by default, simply deletes itself.
};

// Adapter for cleanup, used by ACE_Object_Manager.
extern "C" ACE_Export
void ace_cleanup_destroyer (ACE_Cleanup *, void *param = 0);

class ACE_Export ACE_Cleanup_Info
{
  // = TITLE
  //     Hold cleanup information for thread/process
public:
  ACE_Cleanup_Info (void);
  // Default constructor.

  int operator== (const ACE_Cleanup_Info &o) const;
  // Equality operator.

  int operator!= (const ACE_Cleanup_Info &o) const;
  // Inequality operator.

  void *object_;
  // Point to object that gets passed into the <cleanup_hook_>.

  ACE_CLEANUP_FUNC cleanup_hook_;
  // Cleanup hook that gets called back.

  void *param_;
  // Parameter passed to the <cleanup_hook_>.
};

// Run the thread entry point for the <ACE_Thread_Adapter>.  This must
// be an extern "C" to make certain compilers happy...
extern "C" ACE_Export void *ace_thread_adapter (void *args);

// Forward decl.
class ACE_Thread_Manager;
class ACE_Thread_Descriptor;

class ACE_Export ACE_Thread_Adapter
{
  // = TITLE
  //     Converts a C++ function into a function <ace_thread_adapter>
  //     function that can be called from a thread creation routine
  //     (e.g., <pthread_create> or <_beginthreadex>) that expects an
  //     extern "C" entry point.  This class also makes it possible to
  //     transparently provide hooks to register a thread with an
  //     <ACE_Thread_Manager>.
  //
  // = DESCRIPTION
  //     This class is used in <ACE_OS::thr_create>.  In general, the
  //     thread that creates an object of this class is different from
  //     the thread that calls <invoke> on this object.  Therefore,
  //     the <invoke> method is responsible for deleting itself.
public:
  ACE_Thread_Adapter (ACE_THR_FUNC user_func,
                      void *arg,
                      ACE_THR_C_FUNC entry_point = (ACE_THR_C_FUNC) ace_thread_adapter,
                      ACE_Thread_Manager *thr_mgr = 0,
                      ACE_Thread_Descriptor *td = 0
# if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
                      , ACE_SEH_EXCEPT_HANDLER selector = 0,
                      ACE_SEH_EXCEPT_HANDLER handler = 0
# endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
                      );
  // Constructor.

  void *invoke (void);
  // Execute the <user_func_> with the <arg>.  This function deletes
  // <this>, thereby rendering the object useless after the call
  // returns.

  ACE_Thread_Manager *thr_mgr (void);
  // Accessor for the optional <Thread_Manager>.

  ACE_THR_C_FUNC entry_point (void);
  // Accessor for the C entry point function to the OS thread creation
  // routine.

private:
  ~ACE_Thread_Adapter (void);
  // Ensure that this object must be allocated on the heap.

  void inherit_log_msg (void);
  // Inherit the logging features if the parent thread has an
  // <ACE_Log_Msg>.

  ACE_THR_FUNC user_func_;
  // Thread startup function passed in by the user (C++ linkage).

  void *arg_;
  // Argument to thread startup function.

  ACE_THR_C_FUNC entry_point_;
  // Entry point to the underlying OS thread creation call (C
  // linkage).

  ACE_Thread_Manager *thr_mgr_;
  // Optional thread manager.

  ACE_Thread_Descriptor *thr_desc_;
  // Optional thread descriptor.  Passing this pointer in will force
  // the spawned thread to cache this location in <Log_Msg> and wait
  // until <Thread_Manager> fills in all information in thread
  // descriptor.

# if !defined (ACE_THREADS_DONT_INHERIT_LOG_MSG)
  ostream *ostream_;
  // Ostream where the new TSS Log_Msg will use.

  u_long priority_mask_;
  // Priority_mask to be used in new TSS Log_Msg.

  int tracing_enabled_;
  // Are we allowing tracing in this thread?

  int restart_;
  // Indicates whether we should restart system calls that are
  // interrupted.

  int trace_depth_;
  // Depth of the nesting for printing traces.

#   if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
  ACE_SEH_EXCEPT_HANDLER seh_except_selector_;
  ACE_SEH_EXCEPT_HANDLER seh_except_handler_;
#   endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
# endif /* ACE_THREADS_DONT_INHERIT_LOG_MSG */

  friend class ACE_Thread_Adapter_Has_Private_Destructor;
  // Friend declaration to avoid compiler warning:  only defines a private
  // destructor and has no friends.
};

class ACE_Export ACE_Thread_Control
{
  // = TITLE
  //     Used to keep track of a thread's activities within its entry
  //     point function.
  //
  // = DESCRIPTION
  //     A <ACE_Thread_Manager> uses this class to ensure that threads
  //     it spawns automatically register and unregister themselves
  //     with it.
  //
  //     This class can be stored in thread-specific storage using the
  //     <ACE_TSS> wrapper.  When a thread exits the
  //     <ACE_TSS::cleanup> function deletes this object, thereby
  //     ensuring that it gets removed from its associated
  //     <ACE_Thread_Manager>.
public:
  ACE_Thread_Control (ACE_Thread_Manager *tm = 0,
                      int insert = 0);
  // Initialize the thread control object.  If <insert> != 0, then
  // register the thread with the Thread_Manager.

  ~ACE_Thread_Control (void);
  // Remove the thread from its associated <Thread_Manager> and exit
  // the thread if <do_thr_exit> is enabled.

  void *exit (void *status,
              int do_thr_exit);
  // Remove this thread from its associated <Thread_Manager> and exit
  // the thread if <do_thr_exit> is enabled.

  int insert (ACE_Thread_Manager *tm, int insert = 0);
  // Store the <Thread_Manager> and use it to register ourselves for
  // correct shutdown.

  ACE_Thread_Manager *thr_mgr (void);
  // Returns the current <Thread_Manager>.

  ACE_Thread_Manager *thr_mgr (ACE_Thread_Manager *);
  // Atomically set a new <Thread_Manager> and return the old
  // <Thread_Manager>.

  void *status (void *status);
  // Set the exit status (and return existing status).

  void *status (void);
  // Get the current exit status.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Thread_Manager *tm_;
  // Pointer to the thread manager for this block of code.

  void *status_;
  // Keeps track of the exit status for the thread.
};

class ACE_Export ACE_Thread_Exit
{
  // = TITLE
  //    Keep exit information for a Thread in thread specific storage.
  //    so that the thread-specific exit hooks will get called no
  //    matter how the thread exits (e.g., via <ACE_Thread::exit>, C++
  //    or Win32 exception, "falling off the end" of the thread entry
  //    point function, etc.).
  //
  // = DESCRIPTION
  //    This clever little helper class is stored in thread-specific
  //    storage using the <ACE_TSS> wrapper.  When a thread exits the
  //    <ACE_TSS::cleanup> function deletes this object, thereby
  //    closing it down gracefully.
public:
  ACE_Thread_Exit (void);
  // Capture the Thread that will be cleaned up automatically.

  void thr_mgr (ACE_Thread_Manager *tm);
  // Set the <ACE_Thread_Manager>.

  void *status (void *s);
  // Set the exit status.

  void *status (void);
  // Get the exit status.

  ~ACE_Thread_Exit (void);
  // Destructor calls the thread-specific exit hooks when a thread
  // exits.

  static ACE_Thread_Exit *instance (void);
  // Singleton access point.

  static void cleanup (void *instance, void *);
  // Cleanup method, used by the <ACE_Object_Manager> to destroy the
  // singleton.

private:
  void *status_;
  // Exit status...

  ACE_Thread_Control thread_control_;
  // Automatically add/remove the thread from the
  // <ACE_Thread_Manager>.
};

# if defined (ACE_LACKS_FLOATING_POINT)
typedef ACE_UINT32 ACE_timer_t;
# else
typedef double ACE_timer_t;
# endif /* ACE_LACKS_FLOATING_POINT */

# if defined (ACE_HAS_PRUSAGE_T)
    typedef prusage_t ACE_Rusage;
# elif defined (ACE_HAS_GETRUSAGE)
    typedef rusage ACE_Rusage;
# else
    typedef int ACE_Rusage;
# endif /* ACE_HAS_PRUSAGE_T */

# if defined (ACE_WIN32)
    // = typedef for the _stat data structure
    typedef struct _stat ACE_stat;
# else
    typedef struct stat ACE_stat;
# endif /* ACE_WIN32 */

// We need this for MVS...
extern "C" {
  typedef int (*ACE_COMPARE_FUNC)(const void *, const void *);
}

class ACE_Export ACE_OS
{
  // = TITLE
  //     This class defines an OS independent programming API that
  //     shields developers from nonportable aspects of writing
  //     efficient system programs on Win32, POSIX and other versions
  //     of UNIX, and various real-time operating systems.
  //
  // = DESCRIPTION
  //     This class encapsulates the differences between various OS
  //     platforms.  When porting ACE to a new platform, this class is
  //     the place to focus on.  Once this file is ported to a new
  //     platform, pretty much everything else comes for "free."  See
  //     <www.cs.wustl.edu/~schmidt/ACE_wrappers/etc/ACE-porting.html>
  //     for instructions on porting ACE.  Please see the README file
  //     in this directory for complete information on the meaning of
  //     the various macros.
  ACE_CLASS_IS_NAMESPACE (ACE_OS);
public:

# if defined (CHORUS)
  // We must format this code as follows to avoid confusing OSE.
  enum ACE_HRTimer_Op
    {
      ACE_HRTIMER_START = K_BSTART,
      ACE_HRTIMER_INCR = K_BPOINT,
      ACE_HRTIMER_STOP = K_BSTOP,
      ACE_HRTIMER_GETTIME = 0xFFFF
    };
# else  /* ! CHORUS */
  enum ACE_HRTimer_Op
    {
      ACE_HRTIMER_START = 0x0,  // Only use these if you can stand
      ACE_HRTIMER_INCR = 0x1,   // for interrupts to be disabled during
      ACE_HRTIMER_STOP = 0x2,   // the timed interval!!!!
      ACE_HRTIMER_GETTIME = 0xFFFF
    };
# endif /* ! CHORUS */

  class ace_flock_t
  {
    // = TITLE
    //     OS file locking structure.
  public:
    void dump (void) const;
  // Dump state of the object.

# if defined (ACE_WIN32)
    ACE_OVERLAPPED overlapped_;
# else
    struct flock lock_;
# endif /* ACE_WIN32 */

    LPCTSTR lockname_;
    // Name of this filelock.

    ACE_HANDLE handle_;
    // Handle to the underlying file.

# if defined (CHORUS)
    ACE_mutex_t *process_lock_;
    // This is the mutex that's stored in shared memory.  It can only
    // be destroyed by the actor that initialized it.
# endif /* CHORUS */
  };

# if defined (ACE_WIN32)
  // = Default Win32 Security Attributes definition.
  static LPSECURITY_ATTRIBUTES default_win32_security_attributes (LPSECURITY_ATTRIBUTES);
# endif /* ACE_WIN32 */

  // = A set of wrappers for miscellaneous operations.
  static int atoi (const char *s);
  static char *getenv (const char *symbol);
  static int putenv (const char *string);
  static int getopt (int argc,
                     char *const *argv,
                     const char *optstring);
  static int argv_to_string (ASYS_TCHAR **argv,
                             ASYS_TCHAR *&buf,
                             int substitute_env_args = 1);
  static int string_to_argv (ASYS_TCHAR *buf,
                             size_t &argc,
                             ASYS_TCHAR **&argv,
                             int substitute_env_args = 1);
  static long sysconf (int);

  // = A set of wrappers for condition variables.
  static int cond_broadcast (ACE_cond_t *cv);
  static int cond_destroy (ACE_cond_t *cv);
  static int cond_init (ACE_cond_t *cv,
                        int type = ACE_DEFAULT_SYNCH_TYPE,
                        LPCTSTR name = 0,
                        void *arg = 0);
  static int cond_signal (ACE_cond_t *cv);
  static int cond_timedwait (ACE_cond_t *cv,
                             ACE_mutex_t *m,
                             ACE_Time_Value *);
  static int cond_wait (ACE_cond_t *cv,
                        ACE_mutex_t *m);
# if defined (ACE_WIN32) && defined (ACE_HAS_WTHREADS)
  static int cond_timedwait (ACE_cond_t *cv,
                             ACE_thread_mutex_t *m,
                             ACE_Time_Value *);
  static int cond_wait (ACE_cond_t *cv,
                        ACE_thread_mutex_t *m);
# endif /* ACE_WIN32 && ACE_HAS_WTHREADS */

  // = A set of wrappers for determining config info.
  static LPTSTR cuserid (LPTSTR user,
                         size_t maxlen = 32);
  static int uname (struct utsname *name);
  static long sysinfo (int cmd,
                       char *buf,
                       long count);
  static int hostname (char *name,
                       size_t maxnamelen);

  // = A set of wrappers for explicit dynamic linking.
  static int dlclose (ACE_SHLIB_HANDLE handle);
  // WinCE only supports UNICODE, so we don't need these functions.
# if !defined (ACE_HAS_WINCE)
  static char *dlerror (void);
  static ACE_SHLIB_HANDLE dlopen (ACE_DL_TYPE filename,
                                  int mode = ACE_DEFAULT_SHLIB_MODE);
  static void *dlsym (ACE_SHLIB_HANDLE handle, ACE_DL_TYPE symbol);
# endif /* ! ACE_HAS_WINCE */

  // = A set of wrappers for the directory iterator.
  static DIR *opendir (const char *filename);
  static void closedir (DIR *);
  static struct dirent *readdir (DIR *);
  static int readdir_r (DIR *dirp,
                        struct dirent *entry,
                        struct dirent **result);
  static long telldir (DIR *);
  static void seekdir (DIR *,
                       long loc);
  static void rewinddir (DIR *);

  // = A set of wrappers for stdio file operations.
  static int last_error (void);
  static void last_error (int);
  static int fclose (FILE *fp);
  static int fcntl (ACE_HANDLE handle,
                    int cmd,
                    int val = 0);
  static int fdetach (const char *file);

  static int fsync(ACE_HANDLE handle);

# if !defined (ACE_HAS_WINCE)
  // CE doesn't support these char version functions.
  // However, we should provide UNICODE version of them.
  static FILE *fopen (const char *filename,
                      const char *mode);
#   if defined (fdopen)
#     undef fdopen
#   endif /* fdopen */
  static FILE *fdopen (ACE_HANDLE handle,
                       const char *mode);
  static char *fgets (char *buf,
                      int size,
                      FILE *fp);
  static int stat (const char *file,
                   struct stat *);
  static int truncate (const char *filename,
                       off_t length);
  static int fprintf (FILE *fp,
                      const char *format,
                      ...);
  static int sprintf (char *buf,
                      const char *format,
                      ...);
  static int vsprintf (char *buffer,
                       const char *format,
                       va_list argptr);
  static void perror (const char *s);

  static int printf (const char *format, ...);

  // The old gets () which directly maps to the evil, unprotected
  // gets () has been deprecated.  If you really need gets (),
  // consider the following one.

  // A better gets ().
  //   If n == 0, input is swallowed, but NULL is returned.
  //   Otherwise, reads up to n-1 bytes (not including the newline),
  //              then swallows rest up to newline
  //              then swallows newline
  static char *gets (char *str, int n = 0);
  static int puts (const char *s);
  static int fputs (const char *s,
                    FILE *stream);
# endif /* ! ACE_HAS_WINCE */

  static int fflush (FILE *fp);
  static size_t fread (void *ptr,
                       size_t size,
                       size_t nelems,
                       FILE *fp);
  static int fseek (FILE *fp,
                    long offset,
                    int ptrname);
  static int fstat (ACE_HANDLE,
                    struct stat *);
  static int lstat (const char *,
                    struct stat *);
  static int ftruncate (ACE_HANDLE,
                        off_t);
  static size_t fwrite (const void *ptr,
                        size_t size,
                        size_t nitems,
                        FILE *fp);
  static void rewind (FILE *fp);

  // = Wrappers for searching and sorting.
  static void *bsearch (const void *key,
                        const void *base,
                        size_t nel,
                        size_t size,
                        ACE_COMPARE_FUNC);
  static void qsort (void *base,
                     size_t nel,
                     size_t width,
                     ACE_COMPARE_FUNC);

  // = A set of wrappers for file locks.
  static int flock_init (ACE_OS::ace_flock_t *lock,
                         int flags = 0,
                         LPCTSTR name = 0,
                         mode_t perms = 0);
  static int flock_destroy (ACE_OS::ace_flock_t *lock);
# if defined (ACE_WIN32)
  static void adjust_flock_params (ACE_OS::ace_flock_t *lock,
                                   short whence,
                                   off_t &start,
                                   off_t &len);
# endif /* ACE_WIN32 */
  static int flock_rdlock (ACE_OS::ace_flock_t *lock,
                           short whence = 0,
                           off_t start = 0,
                           off_t len = 0);
  static int flock_tryrdlock (ACE_OS::ace_flock_t *lock,
                              short whence = 0,
                              off_t start = 0,
                              off_t len = 0);
  static int flock_trywrlock (ACE_OS::ace_flock_t *lock,
                              short whence = 0,
                              off_t start = 0,
                              off_t len = 0);
  static int flock_unlock (ACE_OS::ace_flock_t *lock,
                           short whence = 0,
                           off_t start = 0,
                           off_t len = 0);
  static int flock_wrlock (ACE_OS::ace_flock_t *lock,
                           short whence = 0,
                           off_t start = 0,
                           off_t len = 0);

  // = A set of wrappers for low-level process operations.
  static int execl (const char *path,
                    const char *arg0, ...);
  static int execle (const char *path,
                     const char *arg0, ...);
  static int execlp (const char *file,
                     const char *arg0, ...);
  static int execv (const char *path,
                    char *const argv[]);
  static int execvp (const char *file,
                     char *const argv[]);
  static int execve (const char *path,
                     char *const argv[],
                     char *const envp[]);
  static void _exit (int status = 0);
  static void exit (int status = 0);
  static void abort (void);
  static pid_t fork (void);
  static pid_t fork (const char *program_name);
  static pid_t fork_exec (ASYS_TCHAR *argv[]);
  // Forks and exec's a process in a manner that works on Solaris and
  // NT.  argv[0] must be the full path name to the executable.

  static int getpagesize (void);
  static int allocation_granularity (void);
  static gid_t getgid (void);
  static pid_t getpid (void);
  static pid_t getpgid (pid_t pid);
  static uid_t getuid (void);
  static int setuid (uid_t);
  static pid_t setsid (void);
  static int system (const char *s);
  static pid_t wait (int * = 0);
  static pid_t waitpid (pid_t,
                        int * = 0,
                        int = 0);

  // = A set of wrappers for timers and resource stats.
  static u_int alarm (u_int secs);
  static u_int ualarm (u_int usecs,
                       u_int interval = 0);
  static u_int ualarm (const ACE_Time_Value &tv,
                       const ACE_Time_Value &tv_interval = ACE_Time_Value::zero);
  static ACE_hrtime_t gethrtime (const ACE_HRTimer_Op = ACE_HRTIMER_GETTIME);
# if defined (ACE_HAS_POWERPC_TIMER) && (defined (ghs) || defined (__GNUG__))
  static void readPPCTimeBase (u_long &most,
                               u_long &least);
# endif /* ACE_HAS_POWERPC_TIMER  &&  (ghs or __GNUG__) */
  static int clock_gettime (clockid_t,
                            struct timespec *);
  static ACE_Time_Value gettimeofday (void);
  static int getrusage (int who,
                        struct rusage *rusage);
  static int getrlimit (int resource,
                        struct rlimit *rl);
  static int setrlimit (int resource,
                        ACE_SETRLIMIT_TYPE *rl);
  static int sleep (u_int seconds);
  static int sleep (const ACE_Time_Value &tv);
  static int nanosleep (const struct timespec *requested,
                        struct timespec *remaining = 0);

# if defined (ACE_HAS_BROKEN_R_ROUTINES)
#   undef ctime_r
#   undef asctime_r
#   undef rand_r
#   undef getpwnam_r
# endif /* ACE_HAS_BROKEN_R_ROUTINES */

# if defined (difftime)
#   define ACE_DIFFTIME(t1, t0) difftime(t1,t0)
#   undef difftime
# endif /* difftime */

  // = A set of wrappers for operations on time.
# if !defined (ACE_HAS_WINCE)
  static time_t mktime (struct tm *timeptr);
# endif /* !ACE_HAS_WINCE */
  static double difftime (time_t t1,
                          time_t t0);
  static time_t time (time_t *tloc = 0);
  static struct tm *localtime (const time_t *clock);
  static struct tm *localtime_r (const time_t *clock,
                                 struct tm *res);
  static struct tm *gmtime (const time_t *clock);
  static struct tm *gmtime_r (const time_t *clock,
                              struct tm *res);
  static char *asctime (const struct tm *tm);
  static char *asctime_r (const struct tm *tm,
                          char *buf, int buflen);
# if !defined (ACE_HAS_WINCE)
  static char *ctime (const time_t *t);
  static char *ctime_r (const time_t *clock,
                        char *buf,
                        int buflen);
# else
  static wchar_t *ctime (const time_t *t);
  static wchar_t *ctime_r (const time_t *clock,
                           wchar_t *buf,
                           int buflen);
# endif /* !ACE_HAS_WINCE */
  static size_t strftime (char *s,
                          size_t maxsize,
                          const char *format,
                          const struct tm *timeptr);

  // = A set of wrappers for memory managment.
  static void *sbrk (int brk);
  static void *calloc (size_t elements,
                       size_t sizeof_elements);
  static void *malloc (size_t);
  static void *realloc (void *,
                        size_t);
  static void free (void *);

  // = A set of wrappers for memory copying operations.
  static int memcmp (const void *t,
                     const void *s,
                     size_t len);
  static void *memcpy (void *t,
                       const void *s,
                       size_t len);
  static void *memmove (void *t,
                        const void *s,
                        size_t len);
  static void *memset (void *s,
                       int c,
                       size_t len);

  // = A set of wrappers for System V message queues.
  static int msgctl (int msqid,
                     int cmd,
                     struct msqid_ds *);
  static int msgget (key_t key,
                     int msgflg);
  static int msgrcv (int int_id,
                     void *buf,
                     size_t len,
                     long type,
                     int flags);
  static int msgsnd (int int_id,
                     const void *buf,
                     size_t len,
                     int flags);

  // = A set of wrappers for memory mapped files.
  static int madvise (caddr_t addr,
                      size_t len,
                      int advice);
  static void *mmap (void *addr,
                     size_t len,
                     int prot,
                     int flags,
                     ACE_HANDLE handle,
                     off_t off = 0,
                     ACE_HANDLE *file_mapping = 0,
                     LPSECURITY_ATTRIBUTES sa = 0);
  static int mprotect (void *addr,
                       size_t len,
                       int prot);
  static int msync (void *addr,
                    size_t len,
                    int sync);
  static int munmap (void *addr,
                     size_t len);

  // = A set of wrappers for recursive mutex locks.
  static int recursive_mutex_init (ACE_recursive_thread_mutex_t *m,
                                   LPCTSTR name = 0,
                                   void *arg = 0,
                                   LPSECURITY_ATTRIBUTES sa = 0);
  static int recursive_mutex_destroy (ACE_recursive_thread_mutex_t *m);

  static int recursive_mutex_lock (ACE_recursive_thread_mutex_t *m);

  static int recursive_mutex_trylock (ACE_recursive_thread_mutex_t *m);

  static int recursive_mutex_unlock (ACE_recursive_thread_mutex_t *m);

  // = A set of wrappers for mutex locks.
  static int mutex_init (ACE_mutex_t *m,
                         int type = ACE_DEFAULT_SYNCH_TYPE,
                         LPCTSTR name = 0,
                         void *arg = 0,
                         LPSECURITY_ATTRIBUTES sa = 0);
  static int mutex_destroy (ACE_mutex_t *m);

  static int mutex_lock (ACE_mutex_t *m);
  // Win32 note: Abandoned mutexes are not treated differently. 0 is
  // returned since the calling thread does get the ownership.

  static int mutex_lock (ACE_mutex_t *m,
                         int &abandoned);
  // This method is only implemented for Win32.  For abandoned
  // mutexes, <abandoned> is set to 1 and 0 is returned.

  static int mutex_trylock (ACE_mutex_t *m);
  // Win32 note: Abandoned mutexes are not treated differently. 0 is
  // returned since the calling thread does get the ownership.

  static int mutex_trylock (ACE_mutex_t *m,
                            int &abandoned);
  // This method is only implemented for Win32.  For abandoned
  // mutexes, <abandoned> is set to 1 and 0 is returned.

  static int mutex_unlock (ACE_mutex_t *m);

  // = A set of wrappers for mutex locks that only work within a
  // single process.
  static int thread_mutex_init (ACE_thread_mutex_t *m,
                                int type = ACE_DEFAULT_SYNCH_TYPE,
                                LPCTSTR name = 0,
                                void *arg = 0);
  static int thread_mutex_destroy (ACE_thread_mutex_t *m);
  static int thread_mutex_lock (ACE_thread_mutex_t *m);
  static int thread_mutex_trylock (ACE_thread_mutex_t *m);
  static int thread_mutex_unlock (ACE_thread_mutex_t *m);

  // = A set of wrappers for low-level file operations.
  static int access (const char *path,
                     int amode);
  static int close (ACE_HANDLE handle);
  static ACE_HANDLE creat (LPCTSTR filename,
                           mode_t mode);
  static ACE_HANDLE dup (ACE_HANDLE handle);
  static int dup2 (ACE_HANDLE oldfd,
                   ACE_HANDLE newfd);
  static int fattach (int handle,
                      const char *path);
  static long filesize (ACE_HANDLE handle);
  static int getmsg (ACE_HANDLE handle,
                     struct strbuf *ctl,
                     struct strbuf
                     *data, int *flags);
  static int getpmsg (ACE_HANDLE handle,
                      struct strbuf *ctl,
                      struct strbuf
                      *data,
                      int *band,
                      int *flags);
  static int ioctl (ACE_HANDLE handle,
                    int cmd,
                    void * = 0);
  static int isastream (ACE_HANDLE handle);
  static int isatty (ACE_HANDLE handle);
  static off_t lseek (ACE_HANDLE handle,
                      off_t offset,
                      int whence);
  static ACE_HANDLE open (const char *filename,
                          int mode,
                          int perms = 0,
                          LPSECURITY_ATTRIBUTES sa = 0);
  static int putmsg (ACE_HANDLE handle,
                     const struct strbuf *ctl,
                     const struct strbuf *data,
                     int flags);
  static int putpmsg (ACE_HANDLE handle,
                      const struct strbuf *ctl,
                      const struct strbuf *data,
                      int band,
                      int flags);
  static ssize_t read (ACE_HANDLE handle,
                       void *buf,
                       size_t len);
  static ssize_t read (ACE_HANDLE handle,
                       void *buf,
                       size_t len,
                       ACE_OVERLAPPED *);
  static int readlink (const char *path,
                       char *buf,
                       size_t bufsiz);
  static ssize_t pread (ACE_HANDLE handle,
                        void *buf,
                        size_t nbyte,
                        off_t offset);
  static int recvmsg (ACE_HANDLE handle,
                      struct msghdr *msg,
                      int flags);
  static int sendmsg (ACE_HANDLE handle,
                      const struct msghdr *msg,
                      int flags);
  static ssize_t write (ACE_HANDLE handle,
                        const void *buf,
                        size_t nbyte);
  static ssize_t write (ACE_HANDLE handle,
                        const void *buf,
                        size_t nbyte,
                        ACE_OVERLAPPED *);
  static ssize_t pwrite (ACE_HANDLE handle,
                         const void *buf,
                         size_t nbyte,
                         off_t offset);
  static ssize_t readv (ACE_HANDLE handle,
                        iovec *iov,
                        int iovlen);
  static ssize_t writev (ACE_HANDLE handle,
                         const iovec *iov,
                         int iovcnt);
  static ssize_t recvv (ACE_HANDLE handle,
                        iovec *iov,
                        int iovlen);
  static ssize_t sendv (ACE_HANDLE handle,
                        const iovec *iov,
                        int iovcnt);

  // = A set of wrappers for event demultiplexing and IPC.
  static int select (int width,
                     fd_set *rfds,
                     fd_set *wfds,
                     fd_set *efds,
                     const ACE_Time_Value *tv = 0);
  static int select (int width,
                     fd_set *rfds,
                     fd_set *wfds,
                     fd_set *efds,
                     const ACE_Time_Value &tv);
  static int poll (struct pollfd *pollfds,
                   u_long len,
                   ACE_Time_Value *tv = 0);
  static int poll (struct pollfd *pollfds,
                   u_long len,
                   const ACE_Time_Value &tv);
  static int pipe (ACE_HANDLE handles[]);

  static ACE_HANDLE shm_open (const char *filename,
                              int mode,
                              int perms = 0,
                              LPSECURITY_ATTRIBUTES sa = 0);
  static int shm_unlink (const char *path);

  // = A set of wrappers for directory operations.
  static mode_t umask (mode_t cmask);
# if !defined (ACE_HAS_MOSTLY_UNICODE_APIS)
  static int chdir (const char *path);
  static int mkdir (const char *path,
                    mode_t mode = ACE_DEFAULT_DIR_PERMS);
  static int mkfifo (const char *file,
                     mode_t mode = ACE_DEFAULT_FILE_PERMS);
  static char *mktemp (char *t);
  static char *getcwd (char *,
                       size_t);
  static int rename (const char *old_name,
                     const char *new_name);
  static int unlink (const char *path);
  static char *tempnam (const char *dir = 0,
                        const char *pfx = 0);
# endif /* ACE_HAS_MOSTLY_UNICODE_APIS */

  // = A set of wrappers for random number operations.
  static int rand (void);
  static int rand_r (ACE_RANDR_TYPE &seed);
  static void srand (u_int seed);

  // = A set of wrappers for readers/writer locks.
  static int rwlock_init (ACE_rwlock_t *rw,
                          int type = ACE_DEFAULT_SYNCH_TYPE,
                          LPCTSTR name = 0,
                          void *arg = 0);
  static int rwlock_destroy (ACE_rwlock_t *rw);
  static int rw_rdlock (ACE_rwlock_t *rw);
  static int rw_wrlock (ACE_rwlock_t *rw);
  static int rw_tryrdlock (ACE_rwlock_t *rw);
  static int rw_trywrlock (ACE_rwlock_t *rw);
  static int rw_trywrlock_upgrade (ACE_rwlock_t *rw);
  static int rw_unlock (ACE_rwlock_t *rw);

  // = A set of wrappers for auto-reset and manuaevents.
  static int event_init (ACE_event_t *event,
                         int manual_reset = 0,
                         int initial_state = 0,
                         int type = ACE_DEFAULT_SYNCH_TYPE,
                         LPCTSTR name = 0,
                         void *arg = 0,
                         LPSECURITY_ATTRIBUTES sa = 0);
  static int event_destroy (ACE_event_t *event);
  static int event_wait (ACE_event_t *event);
  static int event_timedwait (ACE_event_t *event,
                              ACE_Time_Value *timeout);
  static int event_signal (ACE_event_t *event);
  static int event_pulse (ACE_event_t *event);
  static int event_reset (ACE_event_t *event);

  // = A set of wrappers for semaphores.
  static int sema_destroy (ACE_sema_t *s);
  static int sema_init (ACE_sema_t *s,
                        u_int count,
                        int type = ACE_DEFAULT_SYNCH_TYPE,
                        LPCTSTR name = 0,
                        void *arg = 0,
                        int max = 0x7fffffff,
                        LPSECURITY_ATTRIBUTES sa = 0);
  static int sema_post (ACE_sema_t *s);
  static int sema_post (ACE_sema_t *s,
                        size_t release_count);
  static int sema_trywait (ACE_sema_t *s);
  static int sema_wait (ACE_sema_t *s);
  static int sema_wait (ACE_sema_t *s,
                        ACE_Time_Value &tv);

  // = A set of wrappers for System V semaphores.
  static int semctl (int int_id,
                     int semnum,
                     int cmd,
                     semun);
  static int semget (key_t key,
                     int nsems,
                     int flags);
  static int semop (int int_id,
                    struct sembuf *sops,
                    size_t nsops);

  // = Thread scheduler interface.
  static int sched_params (const ACE_Sched_Params &, ACE_id_t id = ACE_SELF);
  // Set scheduling parameters.  An id of ACE_SELF indicates, e.g.,
  // set the parameters on the calling thread.

  // = A set of wrappers for System V shared memory.
  static void *shmat (int int_id,
                      void *shmaddr,
                      int shmflg);
  static int shmctl (int int_id,
                     int cmd,
                     struct shmid_ds *buf);
  static int shmdt (void *shmaddr);
  static int shmget (key_t key,
                     int size,
                     int flags);

  // = A set of wrappers for Signals.
  static int kill (pid_t pid,
                   int signum);
  static int sigaction (int signum,
                        const struct sigaction *nsa,
                        struct sigaction *osa);
  static int sigaddset (sigset_t *s,
                        int signum);
  static int sigdelset (sigset_t *s,
                        int signum);
  static int sigemptyset (sigset_t *s);
  static int sigfillset (sigset_t *s);
  static int sigismember (sigset_t *s,
                          int signum);
  static ACE_SignalHandler signal (int signum,
                                   ACE_SignalHandler);
  static int sigprocmask (int how,
                          const sigset_t *nsp,
                          sigset_t *osp);

  // = A set of wrappers for sockets.
  static ACE_HANDLE accept (ACE_HANDLE handle,
                            struct sockaddr *addr,
                            int *addrlen);
  static int bind (ACE_HANDLE s,
                   struct sockaddr *name,
                   int namelen);
  static int connect (ACE_HANDLE handle,
                      struct sockaddr *addr,
                      int addrlen);
  static int closesocket (ACE_HANDLE s);
  static struct hostent *gethostbyaddr (const char *addr,
                                        int length,
                                        int type);
  static struct hostent *gethostbyname (const char *name);
  static struct hostent *gethostbyaddr_r (const char *addr,
                                          int length,
                                          int type,
                                          struct hostent *result,
                                          ACE_HOSTENT_DATA buffer,
                                          int *h_errnop);
  static struct hostent *gethostbyname_r (const char *name,
                                          struct hostent *result,
                                          ACE_HOSTENT_DATA buffer,
                                          int *h_errnop);
  static int getpeername (ACE_HANDLE handle,
                          struct sockaddr *addr,
                          int *addrlen);
  static struct protoent *getprotobyname (const char *name);
  static struct protoent *getprotobyname_r (const char *name,
                                            struct protoent *result,
                                            ACE_PROTOENT_DATA buffer);
  static struct protoent *getprotobynumber (int proto);
  static struct protoent *getprotobynumber_r (int proto,
                                              struct protoent *result,
                                              ACE_PROTOENT_DATA buffer);
  static struct servent *getservbyname (const char *svc,
                                        const char *proto);
  static struct servent *getservbyname_r (const char *svc,
                                          const char *proto,
                                          struct servent *result,
                                          ACE_SERVENT_DATA buf);
  static int getsockname (ACE_HANDLE handle,
                          struct sockaddr *addr,
                          int *addrlen);
  static int getsockopt (ACE_HANDLE handle,
                         int level,
                         int optname,
                         char *optval,
                         int *optlen);
  static long inet_addr (const char *name);
  static char *inet_ntoa (const struct in_addr addr);
  static int inet_aton (const char *strptr,
                        struct in_addr *addr);

  static int listen (ACE_HANDLE handle,
                     int backlog);
  static int recv (ACE_HANDLE handle,
                   char *buf,
                   int len,
                   int flags = 0);
  static int recvfrom (ACE_HANDLE handle,
                       char *buf,
                       int len,
                       int flags,
                       struct sockaddr *addr,
                       int *addrlen);
  static int send (ACE_HANDLE handle,
                   const char *buf,
                   int len, int
                   flags = 0);
  static int sendto (ACE_HANDLE handle,
                     const char *buf,
                     int len,
                     int flags,
                     const struct sockaddr *addr,
                     int addrlen);
  static int setsockopt (ACE_HANDLE handle,
                         int level,
                         int optname,
                         const char *optval,
                         int optlen);
  static int shutdown (ACE_HANDLE handle,
                       int how);
  static ACE_HANDLE socket (int domain,
                            int type,
                            int proto);
  static int socketpair (int domain,
                         int type,
                         int protocol,
                         ACE_HANDLE sv[2]);
  static int socket_init (int version_high = 1,
                          int version_low = 1);
  // Initialize WinSock before first use (e.g., when a DLL is first
  // loaded or the first use of a socket() call.

  static int socket_fini (void);
  // Finalize WinSock after last use (e.g., when a DLL is unloaded).

  // = A set of wrappers for password routines.
  static void setpwent (void);
  static void endpwent (void);
  static struct passwd *getpwent (void);
  static struct passwd *getpwnam (const char *user);
  static struct passwd *getpwnam_r (const char *name,
                                    struct passwd *pwent,
                                    char *buffer,
                                    int buflen);

  // = A set of wrappers for regular expressions.
  static char *compile (const char *instring,
                        char *expbuf,
                        char *endbuf);
  static int step (const char *str,
                   char *expbuf);

  // = A set of wrappers for non-UNICODE string operations.
  static int to_lower (int c);
  static int strcasecmp (const char *s,
                         const char *t);
  static int strncasecmp (const char *s,
                          const char *t,
                          size_t len);
  static char *strcat (char *s,
                       const char *t);
  static char *strchr (char *s,
                       int c);
  static const char *strchr (const char *s,
                             int c);
  static char *strrchr (char *s,
                        int c);
  static const char *strrchr (const char *s,
                              int c);
  static char *strnchr (char *s,
                        int c,
                        size_t len);
  static const char *strnchr (const char *s,
                              int c,
                              size_t len);
  static int strcmp (const char *s,
                     const char *t);
  static int strncmp (const char *s,
                      const char *t,
                      size_t len);
  static char *strcpy (char *s,
                       const char *t);
  static char *strecpy (char *des, const char *src);
  // Copies <src> to <des>, returning a pointer to the end of the
  // copied region, rather than the beginning, as <strcpy> does.
  static char *strpbrk (char *s1,
                        const char *s2);
  static const char *strpbrk (const char *s1,
                              const char *s2);
  static size_t strcspn (const char *s,
                         const char *reject);
  static size_t strspn(const char *s1,
                       const char *s2);
  static char *strstr (char *s,
                       const char *t);
  static const char *strstr (const char *s,
                             const char *t);
  static char *strnstr (char *s,
                        const char *t,
                        size_t len);
  static const char *strnstr (const char *s,
                              const char *t,
                              size_t len);
  static char *strdup (const char *s); // Uses malloc
  static char *strenvdup (const char *str);
  static size_t strlen (const char *s);
  static char *strncpy (char *s,
                        const char *t,
                        size_t len);
  static char *strncat (char *s,
                        const char *t,
                        size_t len);
  static char *strtok (char *s,
                       const char *tokens);
  static char *strtok_r (char *s,
                         const char *tokens,
                         char **lasts);
  static long strtol (const char *s,
                      char **ptr,
                      int base);
  static u_long strtoul (const char *s,
                         char **ptr,
                         int base);
  static double strtod (const char *s,
                        char **endptr);
  static int ace_isspace (const char s);

# if !defined (ACE_HAS_WCHAR_TYPEDEFS_CHAR)
  static size_t strlen (const wchar_t *s);
  static wchar_t *strcpy (wchar_t *s,
                          const wchar_t *t);
  static int strcmp (const wchar_t *s,
                     const wchar_t *t);
# endif /* ! ACE_HAS_WCHAR_TYPEDEFS_CHAR */

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_USHORT)
  static size_t strlen (const ACE_UINT16 *s);
  static ACE_UINT16 *strcpy (ACE_UINT16 *s,
                             const ACE_UINT16 *t);
  static int strcmp (const ACE_USHORT16 *s,
                     const ACE_USHORT16 *t);
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_USHORT */

  // The following WChar typedef and functions are used by TAO.  TAO
  // does not use wchar_t because the size of wchar_t is
  // platform-dependent.
  typedef ACE_UINT32 WChar;
  static u_int wslen (const WChar *);
  static WChar *wscpy (WChar *, const WChar *);

# if defined (ACE_HAS_UNICODE)
  // = A set of wrappers for UNICODE string operations.
  static int atoi (const wchar_t *s);
  static wint_t to_lower (wint_t c);
  static wchar_t *strcat (wchar_t *s,
                          const wchar_t *t);
  static wchar_t *strchr (wchar_t *s,
                          wint_t c);
  static const wchar_t *strchr (const wchar_t *s,
                                wint_t c);
  static wchar_t *strecpy (wchar_t *s, const wchar_t *t);
  static wchar_t *strrchr (wchar_t *s,
                           wint_t c);
  static const wchar_t *strrchr (const wchar_t *s,
                                 wint_t c);
  static wchar_t *strnchr (wchar_t *s,
                           wint_t c,
                           size_t len);
  static const wchar_t *strnchr (const wchar_t *s,
                                 wint_t c,
                                 size_t len);
  static int strncmp (const wchar_t *s,
                      const wchar_t *t,
                      size_t len);
  static int strcasecmp (const wchar_t *s,
                         const wchar_t *t);
  static int strncasecmp (const wchar_t *s,
                          const wchar_t *t,
                          size_t len);
  static wchar_t *strpbrk (wchar_t *s1,
                           const wchar_t *s2);
  static const wchar_t *strpbrk (const wchar_t *s1,
                                 const wchar_t *s2);
  static wchar_t *strncpy (wchar_t *s,
                           const wchar_t *t,
                           size_t len);
  static wchar_t *strncat (wchar_t *s,
                           const wchar_t *t,
                           size_t len);
  static wchar_t *strtok (wchar_t *s,
                          const wchar_t *tokens);
  static long strtol (const wchar_t *s,
                      wchar_t **ptr,
                      int base);
  static int ace_isspace (wchar_t c);

#   if defined (ACE_WIN32)
  static wchar_t *strstr (wchar_t *s,
                          const wchar_t *t);
  static const wchar_t *strstr (const wchar_t *s,
                                const wchar_t *t);
  static wchar_t *strnstr (wchar_t *s,
                           const wchar_t *t,
                           size_t len);
  static const wchar_t *strnstr (const wchar_t *s,
                                 const wchar_t *t,
                                 size_t len);
  static wchar_t *strdup (const wchar_t *s); // Uses malloc
  static int sprintf (wchar_t *buf,
                      const wchar_t *format,
                      ...);
#     if 0
  static int sprintf (wchar_t *buf,
                      const char *format,
                      ...);
#     endif /* 0 */
  // the following three are newly added for CE.
  // but they can also be use on Win32.
  //   static char *fgets (wchar_t *buf,
  //                       int size,
  //                       FILE *fp);
  static int fprintf (FILE *fp,
                      const wchar_t *format,
                      ...);
  static void perror (const wchar_t *s);

  static int vsprintf (wchar_t *buffer,
                       const wchar_t *format,
                       va_list argptr);

  static int access (const wchar_t *path,
                     int amode);
  static FILE *fopen (const wchar_t *filename,
                      const wchar_t *mode);
  static FILE *fdopen (ACE_HANDLE handle,
                       const wchar_t *mode);
  static int stat (const wchar_t *file,
                   struct stat *);
  static int truncate (const wchar_t *filename,
                       off_t length);
  static wchar_t *getenv (const wchar_t *symbol);
  static int system (const wchar_t *s);
  static int hostname (wchar_t *name,
                       size_t maxnamelen);
  static ACE_HANDLE open (const wchar_t *filename,
                          int mode,
                          int perms = 0,
                          LPSECURITY_ATTRIBUTES sa = 0);
  static int rename (const wchar_t *oldname,
                     const wchar_t *newname);
  static int unlink (const wchar_t *path);
#     if defined (ACE_HAS_WINCE)
  static wchar_t *dlerror (void);
#     endif /* !ACE_HAS_WINCE */
  static ACE_SHLIB_HANDLE dlopen (ACE_WIDE_DL_TYPE filename,
                                  int mode = ACE_DEFAULT_SHLIB_MODE);
  static void *dlsym (ACE_SHLIB_HANDLE handle, ACE_WIDE_DL_TYPE symbol);
  static wchar_t *mktemp (wchar_t *t);
  static int mkdir (const wchar_t *path,
                    mode_t mode = ACE_DEFAULT_DIR_PERMS);
  static int chdir (const wchar_t *path);
  static wchar_t *getcwd (wchar_t *,
                          size_t);
#   endif /* ACE_WIN32 */
# endif /* ACE_HAS_UNICODE */

  // = A set of wrappers for TLI.
  static int t_accept (ACE_HANDLE fildes,
                       int resfd,
                       struct t_call
                       *call);
  static char *t_alloc (ACE_HANDLE fildes,
                        int struct_type,
                        int
                        fields);
  static int t_bind (ACE_HANDLE fildes,
                     struct t_bind *req,
                     struct
                     t_bind *ret);
  static int t_close (ACE_HANDLE fildes);
  static int t_connect(int fildes,
                       struct t_call *sndcall,
                       struct t_call *rcvcall);
  static void t_error (char *errmsg);
  static int t_free (char *ptr,
                     int struct_type);
  static int t_getinfo (ACE_HANDLE fildes,
                        struct t_info *info);
  static int t_getname (ACE_HANDLE fildes,
                        struct netbuf *namep,
                        int type);
  static int t_getstate (ACE_HANDLE fildes);
  static int t_listen (ACE_HANDLE fildes,
                       struct t_call *call);
  static int t_look (ACE_HANDLE fildes);
  static int t_open (char *path,
                     int oflag,
                     struct t_info *info);
  static int t_optmgmt (ACE_HANDLE fildes,
                        struct t_optmgmt *req,
                        struct t_optmgmt *ret);
  static int t_rcv (ACE_HANDLE fildes,
                    char *buf,
                    u_int nbytes,
                    int *flags);
  static int t_rcvdis (ACE_HANDLE fildes,
                       struct t_discon *discon);
  static int t_rcvrel (ACE_HANDLE fildes);
  static int t_rcvudata (ACE_HANDLE fildes,
                         struct t_unitdata *unitdata,
                         int *flags);
  static int t_rcvuderr (ACE_HANDLE fildes,
                         struct t_uderr *uderr);
  static int t_snd (ACE_HANDLE fildes,
                    char *buf,
                    u_int nbytes,
                    int flags);
  static int t_snddis (ACE_HANDLE fildes,
                       struct t_call *call);
  static int t_sndrel (ACE_HANDLE fildes);
  static int t_sync (ACE_HANDLE fildes);
  static int t_unbind (ACE_HANDLE fildes);

# if 0
  // = A set of wrappers for threads (these are portable since they use the ACE_Thread_ID).
  static int thr_continue (const ACE_Thread_ID &thread);
  static int thr_create (ACE_THR_FUNC,
                         void *args,
                         long flags,
                         ACE_Thread_ID *,
                         long priority = ACE_DEFAULT_THREAD_PRIORITY,
                         void *stack = 0,
                         size_t stacksize = 0);
  static int thr_getprio (ACE_Thread_ID thr_id,
                          int &prio,
                          int *policy = 0);
  static int thr_join (ACE_Thread_ID waiter_id,
                       void **status);
  static int thr_kill (ACE_Thread_ID thr_id,
                       int signum);
  static ACE_Thread_ID thr_self (void);
  static int thr_setprio (ACE_Thread_ID thr_id,
                          int prio);
  static int thr_setprio (const ACE_Sched_Priority prio);
  static int thr_suspend (ACE_Thread_ID target_thread);
  static int thr_cancel (ACE_Thread_ID t_id);
# endif /* 0 */

  // = A set of wrappers for threads

  // These are non-portable since they use ACE_thread_t and
  // ACE_hthread_t and will go away in a future release.
  static int thr_continue (ACE_hthread_t target_thread);
  static int thr_create (ACE_THR_FUNC func,
                         void *args,
                         long flags,
                         ACE_thread_t *thr_id,
                         ACE_hthread_t *t_handle = 0,
                         long priority = ACE_DEFAULT_THREAD_PRIORITY,
                         void *stack = 0,
                         size_t stacksize = 0,
                         ACE_Thread_Adapter *thread_adapter = 0);
  // Creates a new thread having <flags> attributes and running <func>
  // with <args> (if <thread_adapter> is non-0 then <func> and <args>
  // are ignored and are obtained from <thread_adapter>).  <thr_id>
  // and <t_handle> are set to the thread's ID and handle (?),
  // respectively.  The thread runs at <priority> priority (see
  // below).
  //
  // The <flags> are a bitwise-OR of the following:
  // = BEGIN<INDENT>
  // THR_CANCEL_DISABLE, THR_CANCEL_ENABLE, THR_CANCEL_DEFERRED,
  // THR_CANCEL_ASYNCHRONOUS, THR_BOUND, THR_NEW_LWP, THR_DETACHED,
  // THR_SUSPENDED, THR_DAEMON, THR_JOINABLE, THR_SCHED_FIFO,
  // THR_SCHED_RR, THR_SCHED_DEFAULT
  // = END<INDENT>
  //
  // By default, or if <priority> is set to
  // ACE_DEFAULT_THREAD_PRIORITY, an "appropriate" priority value for
  // the given scheduling policy (specified in <flags}>, e.g.,
  // <THR_SCHED_DEFAULT>) is used.  This value is calculated
  // dynamically, and is the median value between the minimum and
  // maximum priority values for the given policy.  If an explicit
  // value is given, it is used.  Note that actual priority values are
  // EXTREMEMLY implementation-dependent, and are probably best
  // avoided.
  //
  // Note that <thread_adapter> is always deleted by <thr_create>,
  // therefore it must be allocated with global operator new.

  static int thr_getprio (ACE_hthread_t thr_id,
                          int &prio);
  static int thr_join (ACE_hthread_t waiter_id,
                       void **status);
  static int thr_join (ACE_thread_t waiter_id,
                       ACE_thread_t *thr_id,
                       void **status);
  static int thr_kill (ACE_thread_t thr_id,
                       int signum);
  static ACE_thread_t thr_self (void);
  static void thr_self (ACE_hthread_t &);
  static int thr_setprio (ACE_hthread_t thr_id,
                          int prio);
  static int thr_setprio (const ACE_Sched_Priority prio);
  static int thr_suspend (ACE_hthread_t target_thread);
  static int thr_cancel (ACE_thread_t t_id);

  static int thr_cmp (ACE_hthread_t t1,
                      ACE_hthread_t t2);
  static int thr_equal (ACE_thread_t t1,
                        ACE_thread_t t2);
  static void thr_exit (void *status = 0);
  static int thr_getconcurrency (void);
  static int lwp_getparams (ACE_Sched_Params &);
# if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  static int thr_getspecific (ACE_OS_thread_key_t key,
                              void **data);
# endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
  static int thr_getspecific (ACE_thread_key_t key,
                              void **data);
  static int thr_keyfree (ACE_thread_key_t key);
  static int thr_key_detach (void *inst);
# if defined (ACE_HAS_THR_C_DEST)
#   if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  static int thr_keycreate (ACE_OS_thread_key_t *key,
                            ACE_THR_C_DEST,
                            void *inst = 0);
#   endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
  static int thr_keycreate (ACE_thread_key_t *key,
                            ACE_THR_C_DEST,
                            void *inst = 0);
# else
#   if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  static int thr_keycreate (ACE_OS_thread_key_t *key,
                            ACE_THR_DEST,
                            void *inst = 0);
#   endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
  static int thr_keycreate (ACE_thread_key_t *key,
                            ACE_THR_DEST,
                            void *inst = 0);
# endif /* ACE_HAS_THR_C_DEST */
  static int thr_key_used (ACE_thread_key_t key);
  static size_t thr_min_stack (void);
  static int thr_setconcurrency (int hint);
  static int lwp_setparams (const ACE_Sched_Params &);
# if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  static int thr_setspecific (ACE_OS_thread_key_t key,
                              void *data);
# endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
  static int thr_setspecific (ACE_thread_key_t key,
                              void *data);
  static int thr_sigsetmask (int how,
                             const sigset_t *nsm,
                             sigset_t *osm);
  static int thr_setcancelstate (int new_state,
                                 int *old_state);
  static int thr_setcanceltype (int new_type,
                                int *old_type);
  static int sigwait (sigset_t *set,
                      int *sig = 0);
  static void thr_testcancel (void);
  static void thr_yield (void);

  static void unique_name (const void *object,
                           LPTSTR name,
                           size_t length);
  // This method uses process id and object pointer to come up with a
  // machine wide unique name.  The process ID will provide uniqueness
  // between processes on the same machine. The "this" pointer of the
  // <object> will provide uniqueness between other "live" objects in
  // the same process. The uniqueness of this name is therefore only
  // valid for the life of <object>.

  static ACE_thread_t NULL_thread;
  // This is necessary to deal with POSIX pthreads and their use of
  // structures for thread ids.

  static ACE_hthread_t NULL_hthread;
  // This is necessary to deal with POSIX pthreads and their use of
  // structures for thread handles.

  static ACE_thread_key_t NULL_key;
  // This is necessary to deal with POSIX pthreads and their use of
  // structures for TSS keys.

# if defined (CHORUS)
  static KnCap actorcaps_[ACE_CHORUS_MAX_ACTORS];
  // This is used to map an actor's id into a KnCap for killing and
  // waiting actors.
# endif /* CHORUS */

# if defined (ACE_WIN32)
  static int socket_initialized_;
  // Keeps track of whether we've already initialized WinSock...
# endif /* ACE_WIN32 */

  static void mutex_lock_cleanup (void *mutex);
  // Handle asynchronous thread cancellation cleanup.

  static void cleanup_tss (const u_int main_thread);
  // Call TSS destructors for the current thread.  If the current
  // thread is the main thread, then the argument must be 1.
  // For private use of ACE_Object_Manager and ACE_Thread_Adapter only.

# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0) && defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS)
  static int netdb_acquire (void);
  static int netdb_release (void);
# endif /* defined (ACE_MT_SAFE) && ACE_LACKS_NETDB_REENTRANT_FUNCTIONS */

  static int scheduling_class (const char *class_name, ACE_id_t &);
  // Find the schedling class ID that corresponds to the class name.

  static int set_scheduling_params (const ACE_Sched_Params &,
                                    ACE_id_t id = ACE_SELF);
  // Friendly interface to priocntl (2).

  // Can't call the following priocntl, because that's a macro on Solaris.
  static int priority_control (ACE_idtype_t, ACE_id_t, int, void *);
  // Low-level interface to priocntl (2).

private:
  static ACE_EXIT_HOOK exit_hook_;
  // Function that is called by ACE_OS::exit (), if non-null.

  static ACE_EXIT_HOOK set_exit_hook (ACE_EXIT_HOOK exit_hook) {
    ACE_EXIT_HOOK old_hook = exit_hook_;
    exit_hook_ = exit_hook;
    return old_hook;
  }
  // For use by ACE_Object_Manager only, to register its exit hook..

  friend class ACE_OS_Object_Manager;
  // Allow the ACE_OS_Object_Manager to call set_exit_hook.

# if defined (ACE_WIN32)
#   if defined (ACE_HAS_WINCE)
  static const wchar_t *day_of_week_name[7];
  static const wchar_t *month_name[12];
  // Supporting data for ctime and ctime_r functions on WinCE.
#   endif /* ACE_HAS_WINCE */

  static void fopen_mode_to_open_mode_converter (char x, int &hmode);
  // Translate fopen's mode char to open's mode.  This helper function
  // is here to avoid maintaining several pieces of identical code.
# endif /* ACE_WIN32 */
};

class ACE_Export ACE_Object_Manager_Base
{
  // = TITLE
  //     Base class for ACE_Object_Manager(s).
  //
  // = DESCRIPTION
  //     Encapsulates the most useful ACE_Object_Manager data structures.
public:
# if (defined (ACE_PSOS) && defined (__DIAB))  || \
     (defined (__DECCXX_VER) && __DECCXX_VER < 60000000)
  // The Diab compiler got confused and complained about access rights
  // if this section was protected (changing this to public makes it happy).
  // Similarly, DEC CXX 5.6 needs the methods to be public.
public:
# else  /* ! (ACE_PSOS && __DIAB)  ||  ! __DECCXX_VER < 60000000 */
protected:
# endif /* ! (ACE_PSOS && __DIAB)  ||  ! __DECCXX_VER < 60000000 */
  ACE_Object_Manager_Base (void);
  // Default constructor.

  virtual ~ACE_Object_Manager_Base (void);
  // Destructor.

public:
  virtual int init (void) = 0;
  // Explicitly initialize.

  virtual int fini (void) = 0;
  // Explicitly destroy.

  enum Object_Manager_State
    {
      OBJ_MAN_UNINITIALIZED = 0,
      OBJ_MAN_INITIALIZING,
      OBJ_MAN_INITIALIZED,
      OBJ_MAN_SHUTTING_DOWN,
      OBJ_MAN_SHUT_DOWN
    };

protected:
  int starting_up_i (void);
  // Returns 1 before ACE_Object_Manager_Base has been constructed.
  // This flag can be used to determine if the program is constructing
  // static objects.  If no static object spawns any threads, the
  // program will be single-threaded when this flag returns 1.  (Note
  // that the program still might construct some static objects when
  // this flag returns 0, if ACE_HAS_NONSTATIC_OBJECT_MANAGER is not
  // defined.)

  int shutting_down_i (void);
  // Returns 1 after ACE_Object_Manager_Base has been destroyed.  This
  // flag can be used to determine if the program is in the midst of
  // destroying static objects.  (Note that the program might destroy
  // some static objects before this flag can return 1, if
  // ACE_HAS_NONSTATIC_OBJECT_MANAGER is not defined.)

  Object_Manager_State object_manager_state_;
  // State of the Object_Manager;

  u_int dynamically_allocated_;
  // Flag indicating whether the ACE_Object_Manager was dynamically
  // allocated by ACE.  (If is was dynamically allocated by the
  // application, then the application is responsible for destroying
  // it.)

  ACE_Object_Manager_Base *next_;
  // Link to next Object_Manager, for chaining.
private:
  // Disallow copying by not implementing the following . . .
  ACE_Object_Manager_Base (const ACE_Object_Manager_Base &);
  ACE_Object_Manager_Base &operator= (const ACE_Object_Manager_Base &);
};

extern "C"
void
ACE_OS_Object_Manager_Internal_Exit_Hook ();


class ACE_Export ACE_OS_Object_Manager : public ACE_Object_Manager_Base
{
public:
  virtual int init (void);
  // Explicitly initialize.

  virtual int fini (void);
  // Explicitly destroy.

  enum Preallocated_Object
    {
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
      ACE_OS_MONITOR_LOCK,
      ACE_TSS_CLEANUP_LOCK,
#   if defined (ACE_HAS_TSS_EMULATION) && \
       defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
      ACE_TSS_BASE_LOCK,
#   endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
# else
      // There currently are no preallocated arrays in the ACE
      // library.  If the application doesn't have any, make sure
      // the the preallocated_array size is at least one by declaring
      // this dummy . . .
      ACE_OS_EMPTY_PREALLOCATED_OBJECT,
# endif /* ACE_MT_SAFE */

      ACE_OS_PREALLOCATED_OBJECTS  // This enum value must be last!
    };
  // Unique identifiers for preallocated objects.

public:
  // Application code should not use these explicitly, so they're
  // hidden here.  They're public so that the ACE_Object_Manager can
  // be constructed/destructed in main () with
  // ACE_HAS_NONSTATIC_OBJECT_MANAGER.
  ACE_OS_Object_Manager ();
  // Constructor.

  ~ACE_OS_Object_Manager ();
  // Destructor.

private:
  friend class ACE_OS;
  friend class ACE_Object_Manager;
  friend class ACE_OS_Object_Manager_Manager;
  friend class ACE_TSS_Cleanup;
  friend class ACE_TSS_Emulation;
  friend void ACE_OS_Object_Manager_Internal_Exit_Hook ();
  // This class is for internal use by ACE_OS, etc., only.

  static ACE_OS_Object_Manager *instance (void);
  // Accessor to singleton instance.

  static ACE_OS_Object_Manager *instance_;
  // Singleton instance pointer.

  static void *preallocated_object[ACE_OS_PREALLOCATED_OBJECTS];
  // Table of preallocated objects.
};

# if defined (ACE_HAS_WINCE)

//          **** Warning ****
// You should not use the following functions under CE at all.
//
//          **** Warning ****

size_t fwrite (void *buf, size_t sz, size_t count, FILE *fp);
size_t fread (void *buf, size_t sz, size_t count, FILE *fp);
int getc (FILE *fp);
int ferror (FILE *fp);
int isatty (ACE_HANDLE h);
ACE_HANDLE fileno (FILE *fp);
int fflush (FILE *fp);
void exit (int status);
int fprintf (FILE *fp, char *format, const char *msg); // not a general purpose
                                                 // fprintf at all.
int printf (const char *format, ...);
int putchar (int c);

//          **** End CE Warning ****

# endif /* ACE_HAS_WINCE */

# if defined (ACE_LACKS_TIMEDWAIT_PROTOTYPES)
extern "C" ssize_t send_timedwait (ACE_HANDLE handle,
                                   const char *buf,
                                   int len,
                                   int flags,
                                   struct timespec *timeout);
extern "C" ssize_t recv_timedwait (ACE_HANDLE handle,
                                   char *buf,
                                   int len,
                                   int flags,
                                   struct timespec *timeout);
extern "C" ssize_t recvfrom_timedwait (ACE_HANDLE handle,
                                       char *buf,
                                       int len,
                                       int flags,
                                       struct sockaddr *addr,
                                       int
                                       *addrlen,
                                       struct timespec *timeout);
extern "C" ssize_t recvmsg_timedwait (ACE_HANDLE handle,
                                      struct msghdr *msg,
                                      int flags,
                                      struct timespec *timeout);
extern "C" ssize_t read_timedwait (ACE_HANDLE handle,
                                   void *buf,
                                   size_t len,
                                   struct timespec *timeout);
extern "C" ssize_t sendto_timedwait (ACE_HANDLE handle,
                                     const char *buf,
                                     int len,
                                     int flags,
                                     const struct sockaddr *addr,
                                     int addrlen,
                                     struct timespec *timeout);
extern "C" ssize_t sendmsg_timedwait (ACE_HANDLE handle,
                                      ACE_SENDMSG_TYPE *msg,
                                      int flags,
                                      struct timespec *timeout);
extern "C" ssize_t write_timedwait (ACE_HANDLE handle,
                                    const void *buf,
                                    size_t n,
                                    struct timespec *timeout);
extern "C" ssize_t readv_timedwait (ACE_HANDLE handle,
                                    iovec *iov,
                                    int iovcnt,
                                    struct timespec* timeout);
extern "C" ssize_t writev_timedwait (ACE_HANDLE handle,
                                     ACE_WRITEV_TYPE *iov,
                                     int iovcnt,
                                     struct timespec *timeout);
# endif /* ACE_LACKS_TIMEDWAIT_PROTOTYPES */

# if defined (ACE_HAS_TSS_EMULATION)
    // Allow config.h to set the default number of thread keys.
#   if !defined (ACE_DEFAULT_THREAD_KEYS)
#     define ACE_DEFAULT_THREAD_KEYS 64
#   endif /* ! ACE_DEFAULT_THREAD_KEYS */

class ACE_Export ACE_TSS_Emulation
{
  // = TITLE
  //     Thread-specific storage emulation.
  //
  // = DESCRIPTION
  //     This provides a thread-specific storage implementation.
  //     It is intended for use on platforms that don't have a
  //     native TSS, or have a TSS with limitations such as the
  //     number of keys or lack of support for removing keys.
public:
  typedef void (*ACE_TSS_DESTRUCTOR)(void *value) /* throw () */;

  enum { ACE_TSS_THREAD_KEYS_MAX = ACE_DEFAULT_THREAD_KEYS };
  // Maximum number of TSS keys allowed over the life of the program.

  static u_int total_keys ();
  // Returns the total number of keys allocated so far.

  static int next_key (ACE_thread_key_t &key);
  // Sets the argument to the next available key.  Returns 0 on success,
  // -1 if no keys are available.

  static ACE_TSS_DESTRUCTOR tss_destructor (const ACE_thread_key_t key);
  // Returns the exit hook associated with the key.  Does _not_ check
  // for a valid key.

  static void tss_destructor (const ACE_thread_key_t key,
                              ACE_TSS_DESTRUCTOR destructor);
  // Associates the TSS destructor with the key.  Does _not_ check
  // for a valid key.

  static void *&ts_object (const ACE_thread_key_t key);
  // Accesses the object referenced by key in the current thread's TSS array.
  // Does _not_ check for a valid key.

  static void *tss_open (void *ts_storage[ACE_TSS_THREAD_KEYS_MAX]);
  // Setup an array to be used for local TSS.  Returns the array address on
  // success.  Returns 0 if local TSS had already been setup for this thread.
  // There is no corresponding tss_close () because it is not needed.

private:
  // Global TSS structures.
  static u_int total_keys_;
  // Always contains the value of the next key to be allocated.

  static ACE_TSS_DESTRUCTOR tss_destructor_ [ACE_TSS_THREAD_KEYS_MAX];
  // Array of thread exit hooks (TSS destructors) that are called for each
  // key (that has one) when the thread exits.

#   if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  static void **tss_base (void* ts_storage[] = 0);
  // Location of current thread's TSS array.
#   else  /* ! ACE_HAS_THREAD_SPECIFIC_STORAGE */
  static void **&tss_base ();
  // Location of current thread's TSS array.
#   endif /* ! ACE_HAS_THREAD_SPECIFIC_STORAGE */

#   if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  // Rely on native thread specific storage for the implementation,
  // but just use one key.
  static ACE_OS_thread_key_t native_tss_key_;

  // Used to indicate if native tss key has been allocated
  static int key_created_;
#   endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */
};

# else   /* ! ACE_HAS_TSS_EMULATION */
#   if defined (TLS_MINIMUM_AVAILABLE)
    // WIN32 platforms define TLS_MINIMUM_AVAILABLE natively.
#     define ACE_DEFAULT_THREAD_KEYS TLS_MINIMUM_AVAILABLE
#   endif /* TSL_MINIMUM_AVAILABLE */

# endif /* ACE_HAS_TSS_EMULATION */

// moved ACE_TSS_Ref, ACE_TSS_Info, and ACE_TSS_Keys class
// declarations from OS.cpp so they are visible to the single
// file of template instantiations.
# if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)
class ACE_TSS_Ref
{
  // = TITLE
  //     "Reference count" for thread-specific storage keys.
  //
  // = DESCRIPTION
  //     Since the <ACE_Unbounded_Stack> doesn't allow duplicates, the
  //     "reference count" is the identify of the thread_id.
public:
  ACE_TSS_Ref (ACE_thread_t id);
  // Constructor

  ACE_TSS_Ref (void);
  // Default constructor

  int operator== (const ACE_TSS_Ref &) const;
  // Check for equality.

  int operator!= (const ACE_TSS_Ref &) const;
  // Check for inequality.

// private:

  ACE_thread_t tid_;
  // ID of thread using a specific key.
};

class ACE_TSS_Info
{
  // = TITLE
  //     Thread Specific Key management.
  //
  // = DESCRIPTION
  //     This class maps a key to a "destructor."
public:
  ACE_TSS_Info (ACE_thread_key_t key,
                void (*dest)(void *) = 0,
                void *tss_inst = 0);
  // Constructor

  ACE_TSS_Info (void);
  // Default constructor

  int key_in_use (void) const { return thread_count_ != -1; }
  // Returns 1 if the key is in use, 0 if not.

  void key_in_use (int flag) { thread_count_ = flag == 0  ?  -1  :  1; }
  // Mark the key as being in use if the flag is non-zero, or
  // not in use if the flag is 0.

  int operator== (const ACE_TSS_Info &) const;
  // Check for equality.

  int operator!= (const ACE_TSS_Info &) const;
  // Check for inequality.

  void dump (void);
  // Dump the state.

private:
  ACE_thread_key_t key_;
  // Key to the thread-specific storage item.

  void (*destructor_)(void *);
  // "Destructor" that gets called when the item is finally released.

  void *tss_obj_;
  // Pointer to ACE_TSS<xxx> instance that has/will allocate the key.

  int thread_count_;
  // Count of threads that are using this key.  Contains -1 when the
  // key is not in use.

  friend class ACE_TSS_Cleanup;
};

class ACE_TSS_Keys
{
  // = TITLE
  //     Collection of in-use flags for a thread's TSS keys.
  //     For internal use only by ACE_TSS_Cleanup; it is public because
  //     some compilers can't use nested classes for template instantiation
  //     parameters.
  //
  // = DESCRIPTION
  //     Wrapper around array of whether each key is in use.  A simple
  //     typedef doesn't work with Sun C++ 4.2.
public:
  ACE_TSS_Keys (void);
  // Default constructor, to initialize all bits to zero (unused).

  int test_and_set (const ACE_thread_key_t key);
  // Mark the specified key as being in use, if it was not already so marked.
  // Returns 1 if the had already been marked, 0 if not.

  int test_and_clear (const ACE_thread_key_t key);
  // Mark the specified key as not being in use, if it was not already so
  // cleared.  Returns 1 if the had already been cleared, 0 if not.

private:
  static void find (const u_int key, u_int &word, u_int &bit);
  // For a given key, find the word and bit number that represent it.

  enum
    {
#   if ACE_SIZEOF_LONG == 8
      ACE_BITS_PER_WORD = 64,
#   elif ACE_SIZEOF_LONG == 4
      ACE_BITS_PER_WORD = 32,
#   else
#     error ACE_TSS_Keys only supports 32 or 64 bit longs.
#   endif /* ACE_SIZEOF_LONG == 8 */
      ACE_WORDS = (ACE_DEFAULT_THREAD_KEYS - 1) / ACE_BITS_PER_WORD + 1
    };

  u_long key_bit_words_[ACE_WORDS];
  // Bit flag collection.  A bit value of 1 indicates that the key is in
  // use by this thread.
};

# endif /* defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) */

class ACE_Export ACE_OS_WString
{
  // = TITLE
  //     A lightweight wchar* to char* string conversion class.
  //
  // = DESCRIPTION
  //     The purpose of this class is to perform conversion between
  //     wchar and char strings.  It is not intended for general
  //     purpose use.
public:
  ACE_OS_WString (const ACE_USHORT16 *s);
  // Ctor must take a wchar stirng.

  ~ACE_OS_WString (void);
  // Dtor will free up the memory.

  char *char_rep (void);
  // Return the internal char* representation.

private:
  char *rep_;
  // Internal pointer to the converted string.

  ACE_OS_WString (void);
  ACE_OS_WString (ACE_OS_WString &);
  ACE_OS_WString& operator= (ACE_OS_WString &);
  // Disallow these operation.
};

class ACE_Export ACE_OS_CString
{
  // = TITLE
  //     A lightweight char* to wchar* string conversion class.
  //
  // = DESCRIPTION
  //     The purpose of this class is to perform conversion from
  //     char* to wchar* strings.  It is not intended for general
  //     purpose use.
public:
  ACE_OS_CString (const char *s);
  // Ctor must take a wchar stirng.

  ~ACE_OS_CString (void);
  // Dtor will free up the memory.

  ACE_USHORT16 *wchar_rep (void);
  // Return the internal char* representation.

private:
  ACE_USHORT16 *rep_;
  // Internal pointer to the converted string.

  ACE_OS_CString (void);
  ACE_OS_CString (ACE_OS_CString &);
  ACE_OS_CString operator= (ACE_OS_CString &);
  // Disallow these operation.
};

// Support non-scalar thread keys, such as with some POSIX
// implementations, e.g., MVS.
# if defined (ACE_HAS_NONSCALAR_THREAD_KEY_T)
#   define ACE_KEY_INDEX(OBJ,KEY) \
  u_int OBJ; \
  ACE_OS::memcpy (&OBJ, &KEY, sizeof (u_int))
# else
#   define ACE_KEY_INDEX(OBJ,KEY) u_int OBJ = KEY
# endif /* ACE_HAS_NONSCALAR_THREAD_KEY_T */

// A useful abstraction for expressions involving operator new since
// we can change memory allocation error handling policies (e.g.,
// depending on whether ANSI/ISO exception handling semantics are
// being used).

# if defined(ACE_NEW_THROWS_EXCEPTIONS)
#   if defined (__SUNPRO_CC)
#     include /**/ <exception.h>
      // Note: we catch ::xalloc rather than just xalloc because of a
      // name clash with unsafe_ios::xalloc()
#     define ACE_bad_alloc ::xalloc
#   else
    // I know this works for HP aC++... if <stdexcept> is used, it
    // introduces other stuff that breaks things, like <memory>, which
    // screws up auto_ptr.
#     include /**/ <new>
#     define ACE_bad_alloc bad_alloc
#   endif /* __SUNPRO_CC */

#   define ACE_NEW_RETURN(POINTER,CONSTRUCTOR,RET_VAL) \
   do { try { POINTER = new CONSTRUCTOR; } \
        catch (ACE_bad_alloc) { errno = ENOMEM; return RET_VAL; } \
   } while (0)

#   define ACE_NEW(POINTER,CONSTRUCTOR) \
   do { try { POINTER = new CONSTRUCTOR; } \
        catch (ACE_bad_alloc) { errno = ENOMEM; return; } \
   } while (0)
#   define ACE_NEW_THROW_EX(POINTER,CONSTRUCTOR,EXCEPTION) \
     do { try { POINTER = new CONSTRUCTOR; } \
       catch (ACE_bad_alloc) { errno = ENOMEM; ACE_THROW_INT (EXCEPTION); } \
     } while (0)
// The following ACE_NEW_THROW* macros are to be depricated soon.
#   define ACE_NEW_THROW(POINTER,CONSTRUCTOR,EXCEPTION) \
     do { try { POINTER = new CONSTRUCTOR; } \
       catch (ACE_bad_alloc) { errno = ENOMEM; TAO_THROW (EXCEPTION); } \
     } while (0)
#   define ACE_NEW_THROW_RETURN(POINTER,CONSTRUCTOR,EXCEPTION,RET_VAL) \
     do { try { POINTER = new CONSTRUCTOR; } \
        catch (ACE_bad_alloc) { errno = ENOMEM; TAO_THROW_RETURN (EXCEPTION,RET_VAL); } \
     } while (0)
#   define ACE_NEW_TRY_THROW(POINTER,CONSTRUCTOR,EXCEPTION) \
  do { try { POINTER = new CONSTRUCTOR; } \
       catch (ACE_bad_alloc) { errno = ENOMEM; TAO_TRY_THROW (EXCEPTION); } \
     } while (0)

# else /* ACE_NEW_THROWS_EXCEPTIONS */

#   define ACE_NEW_RETURN(POINTER,CONSTRUCTOR,RET_VAL) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL; } \
   } while (0)
#   define ACE_NEW(POINTER,CONSTRUCTOR) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return; } \
   } while (0)
#   define ACE_NEW_THROW_EX(POINTER,CONSTRUCTOR,EXCEPTION) \
     do { POINTER = new CONSTRUCTOR; \
       if (POINTER == 0) { errno = ENOMEM; ACE_THROW_INT (EXCEPTION); } \
     } while (0)
// The following ACE_NEW_THROW* macros are to be depricated soon.
#   define ACE_NEW_THROW(POINTER,CONSTRUCTOR,EXCEPTION) \
     do { POINTER = new CONSTRUCTOR; \
       if (POINTER == 0) { errno = ENOMEM; TAO_THROW (EXCEPTION); } \
     } while (0)
#   define ACE_NEW_THROW_RETURN(POINTER,CONSTRUCTOR,EXCEPTION,RET_VAL) \
     do { POINTER = new CONSTRUCTOR; \
        if (POINTER == 0)\
        { errno = ENOMEM; TAO_THROW_RETURN (EXCEPTION,RET_VAL); } \
     } while (0)
#   define ACE_NEW_TRY_THROW(POINTER,CONSTRUCTOR,EXCEPTION) \
     do { POINTER = new CONSTRUCTOR; \
       if (POINTER == 0) { errno = ENOMEM; TAO_TRY_THROW (EXCEPTION); } \
     } while (0)
# endif /* ACE_NEW_THROWS_EXCEPTIONS */

// Some useful abstrations for expressions involving
// ACE_Allocator.malloc ().  The difference between ACE_NEW_MALLOC*
// with ACE_ALLOCATOR* is that they call constructors also.

# define ACE_ALLOCATOR_RETURN(POINTER,ALLOCATOR,RET_VAL) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL; } \
   } while (0)
# define ACE_ALLOCATOR(POINTER,ALLOCATOR) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; return; } \
   } while (0)

# define ACE_NEW_MALLOC_RETURN(POINTER,ALLOCATOR,CONSTRUCTOR,RET_VAL) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL;} \
     else { new (POINTER) CONSTRUCTOR; } \
   } while (0)
# define ACE_NEW_MALLOC(POINTER,ALLOCATOR,CONSTRUCTOR) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; return;} \
     else { new (POINTER) CONSTRUCTOR; } \
   } while (0)

# define ACE_NOOP(x)

# define ACE_DES_NOFREE (POINTER,CLASS) POINTER->~CLASS ()
# define ACE_DES_FREE(POINTER,DEALLOCATOR,CLASS) \
   do { POINTER->~CLASS (); DEALLOCATOR (POINTER); } while (0)

# if defined (ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR)
#   define ACE_DES_NOFREE_TEMPLATE (POINTER,T_CLASS,T_PARAMETER) \
     POINTER->~ T_CLASS ()
#   define ACE_DES_FREE_TEMPLATE(POINTER,DEALLOCATOR,T_CLASS,T_PARAMETER) \
   do { POINTER->~ T_CLASS (); \
        DEALLOCATOR (POINTER); \
      } while (0)
#   define ACE_DES_FREE_TEMPLATE2(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2) \
  do { POINTER->~ T_CLASS (); \
       DEALLOCATOR (POINTER); \
     } while (0)
# else /* ! ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR */
#   define ACE_DES_NOFREE_TEMPLATE (POINTER,T_CLASS,T_PARAMETER) \
     POINTER -> T_CLASS T_PARAMETER ::~ T_CLASS ()

#   if defined (__Lynx__) && __LYNXOS_SDK_VERSION == 199701L
  // LynxOS 3.0.0's g++ has trouble with the real versions of these.
#     define ACE_DES_FREE_TEMPLATE(POINTER,DEALLOCATOR,T_CLASS,T_PARAMETER)
#     define ACE_DES_FREE_TEMPLATE2(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2)
#   else
#     define ACE_DES_FREE_TEMPLATE(POINTER,DEALLOCATOR,T_CLASS,T_PARAMETER) \
    do { POINTER-> T_CLASS T_PARAMETER ::~ T_CLASS (); \
         DEALLOCATOR (POINTER); \
       } while (0)
#     define ACE_DES_FREE_TEMPLATE2(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2) \
    do { POINTER-> T_CLASS <T_PARAM1, T_PARAM2> ::~ T_CLASS (); \
         DEALLOCATOR (POINTER); \
       } while (0)
#   endif /* defined (__Lynx__) && __LYNXOS_SDK_VERSION == 199701L */
# endif /* defined ! ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR */

# if defined (ACE_HAS_SIGNAL_SAFE_OS_CALLS)
// The following two macros ensure that system calls are properly
// restarted (if necessary) when interrupts occur.
#   define ACE_OSCALL(OP,TYPE,FAILVALUE,RESULT) \
  do \
    RESULT = (TYPE) OP; \
  while (RESULT == FAILVALUE && errno == EINTR && ACE_LOG_MSG->restart ())
#   define ACE_OSCALL_RETURN(OP,TYPE,FAILVALUE) \
  do { \
    TYPE ace_result_; \
    do \
      ace_result_ = (TYPE) OP; \
    while (ace_result_ == FAILVALUE && errno == EINTR && ACE_LOG_MSG->restart ()); \
    return ace_result_; \
  } while (0)
# elif defined (ACE_WIN32)
#   define ACE_OSCALL_RETURN(X,TYPE,FAILVALUE) \
  do \
    return (TYPE) X; \
  while (0)
#   define ACE_OSCALL(X,TYPE,FAILVALUE,RESULT) \
  do \
    RESULT = (TYPE) X; \
  while (0)
#   define ACE_WIN32CALL_RETURN(X,TYPE,FAILVALUE) \
  do { \
    TYPE ace_result_ = (TYPE) X; \
    if (ace_result_ == FAILVALUE) \
      errno = ::GetLastError (); \
    return ace_result_; \
  } while (0)
#   define ACE_WIN32CALL(X,TYPE,FAILVALUE,RESULT) \
  do { \
    RESULT = (TYPE) X; \
    if (RESULT == FAILVALUE) \
      errno = ::GetLastError (); \
  } while (0)
# else
#   define ACE_OSCALL_RETURN(OP,TYPE,FAILVALUE) do { TYPE ace_result_ = FAILVALUE; ace_result_ = ace_result_; return OP; } while (0)
#   define ACE_OSCALL(OP,TYPE,FAILVALUE,RESULT) do { RESULT = (TYPE) OP; } while (0)
# endif /* ACE_HAS_SIGNAL_SAFE_OS_CALLS */

# if defined (ACE_HAS_THR_C_FUNC)
// This is necessary to work around nasty problems with MVS C++.
extern "C" ACE_Export void ace_mutex_lock_cleanup_adapter (void *args);
#   define ACE_PTHREAD_CLEANUP_PUSH(A) pthread_cleanup_push (ace_mutex_lock_cleanup_adapter, (void *) A);
#   define ACE_PTHREAD_CLEANUP_POP(A) pthread_cleanup_pop(A)
# elif defined (ACE_HAS_PTHREADS) && !defined (ACE_LACKS_PTHREAD_CLEANUP)
#   define ACE_PTHREAD_CLEANUP_PUSH(A) pthread_cleanup_push (ACE_OS::mutex_lock_cleanup, (void *) A);
#   define ACE_PTHREAD_CLEANUP_POP(A) pthread_cleanup_pop(A)
# else
#   define ACE_PTHREAD_CLEANUP_PUSH(A)
#   define ACE_PTHREAD_CLEANUP_POP(A)
# endif /* ACE_HAS_THR_C_FUNC */

# if !defined (ACE_DEFAULT_MUTEX_A)
#   define ACE_DEFAULT_MUTEX_A "ACE_MUTEX"
# endif /* ACE_DEFAULT_MUTEX_A */

# if !defined (ACE_MAIN)
#   define ACE_MAIN main
# endif /* ! ACE_MAIN */

# if defined (ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER)
#   if !defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
#     define ACE_HAS_NONSTATIC_OBJECT_MANAGER
#   endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER */
# endif /* ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER */

# if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER) && !defined (ACE_HAS_WINCE) && !defined (ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER)
// Rename "main ()" on platforms that don't allow it to be called "main ()".
// Also, create an ACE_Object_Manager static instance in "main ()".
#   include "ace/Object_Manager.h"

// ACE_MAIN_OBJECT_MANAGER defines the ACE_Object_Manager(s) that will
// be instantiated on the stack of main ().  Note that it is only used
// when compiling main ():  its value does not affect the contents of
// ace/OS.o.
#   if defined (ACE_HAS_MINIMAL_ACE_OS)
#     define ACE_MAIN_OBJECT_MANAGER \
        ACE_OS_Object_Manager ace_os_object_manager;
#   else  /* ! ACE_HAS_MINIMAL_ACE_OS */
#     define ACE_MAIN_OBJECT_MANAGER \
        ACE_OS_Object_Manager ace_os_object_manager; \
        ACE_Object_Manager ace_object_manager;
#   endif /* ! ACE_HAS_MINIMAL_ACE_OS */

#   if defined (ACE_PSOSIM)
// PSOSIM root lacks the standard argc, argv command line parameters,
// create dummy argc and argv in the "real" main  and pass to "user" main.
// NOTE: ACE_MAIN must be defined to give the return type as well as the
// name of the entry point.
#     define main \
ace_main_i (int, char *[]);                /* forward declaration */ \
ACE_MAIN ()   /* user's entry point, e.g., "main" w/out argc, argv */ \
{ \
  int argc = 1;                            /* dummy arg count */ \
  char *argv[] = {"psosim"};               /* dummy arg list */ \
  ACE_MAIN_OBJECT_MANAGER \
  int ret_val = -1; /* assume the worst */ \
  if (ACE_PSOS_Time_t::init_simulator_time ()) /* init simulator time */ \
  { \
    ACE_ERROR((LM_ERROR, "init_simulator_time failed\n"));  /* report */ \
  } \
  else \
  { \
    ret_val = ace_main_i (argc, argv);   /* call user main, save result */ \
  } \
  ACE_OS::exit (ret_val);                /* pass code to simulator exit */ \
} \
int \
ace_main_i
#   elif defined (ACE_PSOS) && defined (ACE_PSOS_LACKS_ARGC_ARGV)
// PSOS root lacks the standard argc, argv command line parameters,
// create dummy argc and argv in the "real" main  and pass to "user" main.
// Ignore return value from user main as well.  NOTE: ACE_MAIN must be
// defined to give the return type as well as the name of the entry point
#     define main \
ace_main_i (int, char *[]);               /* forward declaration */ \
ACE_MAIN ()   /* user's entry point, e.g., "main" w/out argc, argv */ \
{ \
  int argc = 1;                           /* dummy arg count */ \
  char *argv[] = {"root"};                /* dummy arg list */ \
  ACE_MAIN_OBJECT_MANAGER \
  ace_main_i (argc, argv);                /* call user main, ignore result */ \
} \
int \
ace_main_i
#   else
#     define main \
ace_main_i (int, char *[]);                      /* forward declaration */ \
int \
ACE_MAIN (int argc, char *argv[])   /* user's entry point, e.g., "main" */ \
{ \
  ACE_MAIN_OBJECT_MANAGER \
  return ace_main_i (argc, argv);         /* what the user calls "main" */ \
} \
int \
ace_main_i
#   endif   /* ACE_PSOSIM */
# endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER && !ACE_HAS_WINCE && !ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER */

# if defined (ACE_HAS_WINCE)
#   include "ace/Object_Manager.h"
// We need to rename program entry name "main" with ace_ce_main here
// so that we can call it from CE's bridge class.

// I'll assume there'll only be DLL version of ACE on CE for now.
#   define main \
ace_main_i (int, ASYS_TCHAR *[]); /* forward declaration */ \
int \
ace_ce_main (int argc, ASYS_TCHAR *argv[])   /* user's entry point, e.g., "main" */ \
{ \
  return ace_main_i (argc, argv);         /* what the user calls "main" */ \
} \
int \
ace_main_i

class ACE_Export ACE_CE_Bridge
{
  // = TITLE
  //     This class bridges between ACE's default text output windows
  //     and the original ACE program.
  //
  // = DESCRIPTION
  //     As there is no such thing as text-based programs on Windows
  //     CE.  We need to create a windows to read the command prompt
  //     and bridge the output windows with the original ACE program
  //     entry point.  You'll need to link your program with
  //     "ace-windows.lib" for this to work.  You can refer to
  //     $ACE_ROOT/WindowsCE/Main for how I use a dialog box to run
  //     the original ACE programs.  This is certainly not the only
  //     way to use ACE in Windows programs.
public:
  ACE_CE_Bridge (void);
  // Default ctor.

  ACE_CE_Bridge (CWnd *, int notification, int idc);
  // Construct and set the default windows.

  ~ACE_CE_Bridge (void);
  // Default dtor.

  void set_window (CWnd *, int notification, int idc);
  // Specify which window to use.

  void set_self_default (void);
  // Set the default window.

  int notification (void);
  int idc (void);
  CWnd *window (void);
  // Access functions.

  static ACE_CE_Bridge *get_default_winbridge (void);
  // Get the reference of default ACE_CE_BRIDGE.

  int write_msg (LPCTSTR str);
  // Write a string to windows.

  int write_msg (CString *cs);
  // Write a CString to windows.  Notice that the CString object will
  // be freed by windows.
private:
  // @@ We should use a allocator here.

  CWinThread *control_;
  // The ace program that tied to this windows.

  CWnd *text_output_;
  // A pointer to the window that knows how to
  // handle ACE related messages.

  int notification_;
  // Notification of the window that receives WM_COMMAND when
  // outputing strings.

  int idc_;
  // IDC code of the window that receives WM_COMMAND when
  // outputing strings.

  ASYS_TCHAR *cmdline_;

  static ACE_CE_Bridge *default_text_bridge_;
  // A pointer to the default ACE_CE_BRIDGE obj.
};
# endif /* ACE_HAS_WINCE */

// This is used to indicate that a platform doesn't support a
// particular feature.
# if defined ACE_HAS_VERBOSE_NOTSUP
  // Print a console message with the file and line number of the
  // unsupported function.
#   define ACE_NOTSUP_RETURN(FAILVALUE) do { errno = ENOTSUP; ACE_OS::fprintf (stderr, ACE_TEXT ("ACE_NOTSUP: %s, line %d\n"), __FILE__, __LINE__); return FAILVALUE; } while (0)
# else /* ! ACE_HAS_VERBOSE_NOTSUP */
#   define ACE_NOTSUP_RETURN(FAILVALUE) do { errno = ENOTSUP ; return FAILVALUE; } while (0)
# endif /* ! ACE_HAS_VERBOSE_NOTSUP */

# if defined (ACE_HAS_GNUC_BROKEN_TEMPLATE_INLINE_FUNCTIONS)
#   define ACE_INLINE_FOR_GNUC ACE_INLINE
# else
#   define ACE_INLINE_FOR_GNUC
# endif /* ACE_HAS_GNUC_BROKEN_TEMPLATE_INLINE_FUNCTIONS */

# if defined (ACE_WIN32) && ! defined (ACE_HAS_WINCE)
typedef TRANSMIT_FILE_BUFFERS ACE_TRANSMIT_FILE_BUFFERS;
typedef LPTRANSMIT_FILE_BUFFERS ACE_LPTRANSMIT_FILE_BUFFERS;
typedef PTRANSMIT_FILE_BUFFERS ACE_PTRANSMIT_FILE_BUFFERS;

#   define ACE_INFINITE INFINITE
#   define ACE_STATUS_TIMEOUT STATUS_TIMEOUT
#   define ACE_WAIT_FAILED WAIT_FAILED
#   define ACE_WAIT_TIMEOUT WAIT_TIMEOUT
# else /* ACE_WIN32 */
struct ACE_TRANSMIT_FILE_BUFFERS
{
  void *Head;
  size_t HeadLength;
  void *Tail;
  size_t TailLength;
};
typedef ACE_TRANSMIT_FILE_BUFFERS* ACE_PTRANSMIT_FILE_BUFFERS;
typedef ACE_TRANSMIT_FILE_BUFFERS* ACE_LPTRANSMIT_FILE_BUFFERS;

#   if !defined (ACE_INFINITE)
#     define ACE_INFINITE LONG_MAX
#   endif /* ACE_INFINITE */
#   define ACE_STATUS_TIMEOUT LONG_MAX
#   define ACE_WAIT_FAILED LONG_MAX
#   define ACE_WAIT_TIMEOUT LONG_MAX
# endif /* ACE_WIN32 */

# if defined (UNICODE)

#   if !defined (ACE_DEFAULT_NAMESPACE_DIR)
#     define ACE_DEFAULT_NAMESPACE_DIR ACE_DEFAULT_NAMESPACE_DIR_W
#   endif /* ACE_DEFAULT_NAMESPACE_DIR */
#   if !defined (ACE_DEFAULT_LOCALNAME)
#     define ACE_DEFAULT_LOCALNAME ACE_DEFAULT_LOCALNAME_W
#   endif /* ACE_DEFAULT_LOCALNAME */
#   if !defined (ACE_DEFAULT_GLOBALNAME)
#     define ACE_DEFAULT_GLOBALNAME ACE_DEFAULT_GLOBALNAME_W
#   endif /* ACE_DEFAULT_GLOBALNAME */
#   if !defined (ACE_DIRECTORY_SEPARATOR_STR)
#     define ACE_DIRECTORY_SEPARATOR_STR ACE_DIRECTORY_SEPARATOR_STR_W
#   endif /* ACE_DIRECTORY_SEPARATOR_STR */
#   if !defined (ACE_DIRECTORY_SEPARATOR_CHAR)
#     define ACE_DIRECTORY_SEPARATOR_CHAR ACE_DIRECTORY_SEPARATOR_CHAR_W
#   endif /* ACE_DIRECTORY_SEPARATOR_CHAR */
#   if !defined (ACE_PLATFORM)
#     define ACE_PLATFORM ACE_PLATFORM_W
#   endif /* ACE_PLATFORM */
#   if !defined (ACE_PLATFORM_EXE_SUFFIX)
#     define ACE_PLATFORM_EXE_SUFFIX ACE_PLATFORM_EXE_SUFFIX_W
#   endif /* ACE_PLATFORM_EXE_SUFFIX */
#   if !defined (ACE_DEFAULT_MUTEX_W)
#     define ACE_DEFAULT_MUTEX_W L"ACE_MUTEX"
#   endif /* ACE_DEFAULT_MUTEX_W */
#   if !defined (ACE_DEFAULT_MUTEX)
#     define ACE_DEFAULT_MUTEX ACE_DEFAULT_MUTEX_W
#   endif /* ACE_DEFAULT_MUTEX */

# else

#   if !defined (ACE_DEFAULT_NAMESPACE_DIR)
#     define ACE_DEFAULT_NAMESPACE_DIR ACE_DEFAULT_NAMESPACE_DIR_A
#   endif /* ACE_DEFAULT_NAMESPACE_DIR */
#   if !defined (ACE_DEFAULT_LOCALNAME)
#     define ACE_DEFAULT_LOCALNAME ACE_DEFAULT_LOCALNAME_A
#   endif /* ACE_DEFAULT_LOCALNAME */
#   if !defined (ACE_DEFAULT_GLOBALNAME)
#     define ACE_DEFAULT_GLOBALNAME ACE_DEFAULT_GLOBALNAME_A
#   endif /* ACE_DEFAULT_GLOBALNAME */
#   if !defined (ACE_DIRECTORY_SEPARATOR_STR)
#     define ACE_DIRECTORY_SEPARATOR_STR ACE_DIRECTORY_SEPARATOR_STR_A
#   endif /* ACE_DIRECTORY_SEPARATOR_STR */
#   if !defined (ACE_DIRECTORY_SEPARATOR_CHAR)
#     define ACE_DIRECTORY_SEPARATOR_CHAR ACE_DIRECTORY_SEPARATOR_CHAR_A
#   endif /* ACE_DIRECTORY_SEPARATOR_CHAR */
#   if !defined (ACE_PLATFORM)
#     define ACE_PLATFORM ACE_PLATFORM_A
#   endif /* ACE_PLATFORM */
#   if !defined (ACE_PLATFORM_EXE_SUFFIX)
#     define ACE_PLATFORM_EXE_SUFFIX ACE_PLATFORM_EXE_SUFFIX_A
#   endif /* ACE_PLATFORM_EXE_SUFFIX */
#   if !defined (ACE_DEFAULT_MUTEX_W)
#     define ACE_DEFAULT_MUTEX_W "ACE_MUTEX"
#   endif /* ACE_DEFAULT_MUTEX_W */
#   if !defined (ACE_DEFAULT_MUTEX)
#     define ACE_DEFAULT_MUTEX ACE_DEFAULT_MUTEX_A
#   endif /* ACE_DEFAULT_MUTEX */
# endif /* UNICODE */

// Some ACE classes always use inline functions to maintain high
// performance, but some platforms have buggy inline function support.
// In this case, we don't use inline with them.
# if defined (ACE_LACKS_INLINE_FUNCTIONS)
#   if defined (ASYS_INLINE)
#     undef ASYS_INLINE
#   endif /* ASYS_INLINE */
#   define ASYS_INLINE
#   if defined (ACE_HAS_INLINED_OSCALLS)
#     undef ACE_HAS_INLINED_OSCALLS
#   endif /* ACE_HAS_INLINED_OSCALLS */
# else
#   define ASYS_INLINE inline
# endif /* ACE_LACKS_INLINE_FUNCTIONS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS.i"
# endif /* ACE_HAS_INLINED_OSCALLS */

# if !defined (ACE_HAS_MINIMAL_ACE_OS)
# include "ace/Trace.h"

// These need to come here to avoid problems with circular dependencies.
# include "ace/Log_Msg.h"
# endif /* ! ACE_HAS_MINIMAL_ACE_OS */

// The following are some insane macros that are useful in cases when
// one has to have a string in a certain format.  Both of these macros
// allow the user to create a temporary copy. If the user needs to
// keep this temporary string around, they must do some sort of strdup
// on the temporary string.
//
// The ACE_WIDE_STRING macro allows the user to create a temporary
// representation of an ASCII string as a WIDE string.
//
// The ACE_MULTIBYTE_STRING macro allows the user to create a
// temporary representation of a WIDE string as an ASCII string. Note
// that some data may be lost in this conversion.

# if defined (UNICODE)
#   define ACE_WIDE_STRING(ASCII_STRING) \
ACE_OS_CString (ASCII_STRING).wchar_rep ()
#   define ACE_MULTIBYTE_STRING(WIDE_STRING) \
ACE_OS_WString (WIDE_STRING).char_rep ()
#   define ACE_TEXT_STRING ACE_WString
#   if defined (ACE_HAS_MOSTLY_UNICODE_APIS)
#     define ASYS_MULTIBYTE_STRING(WIDE_STRING) WIDE_STRING
#     define ASYS_ONLY_WIDE_STRING(WIDE_STRING) WIDE_STRING
#     define ASYS_ONLY_MULTIBYTE_STRING(WIDE_STRING) \
ACE_OS_WString (WIDE_STRING).char_rep ()
#   else
#     define ASYS_MULTIBYTE_STRING(WIDE_STRING) \
ACE_OS_WString (WIDE_STRING).char_rep ()
#     define ASYS_ONLY_WIDE_STRING(ASCII_STRING) \
ACE_OS_CString (ASCII_STRING).wchar_rep ()
#     define ASYS_ONLY_MULTIBYTE_STRING(WIDE_STRING) WIDE_STRING
#   endif /* ACE_HAS_MOSTLY_UNICODE_APIS */
# else
#   define ACE_WIDE_STRING(ASCII_STRING) ASCII_STRING
#   define ACE_MULTIBYTE_STRING(WIDE_STRING) WIDE_STRING
#   define ACE_TEXT_STRING ACE_CString
#   define ASYS_MULTIBYTE_STRING(WIDE_STRING) WIDE_STRING
#   define ASYS_ONLY_WIDE_STRING(WIDE_STRING) WIDE_STRING
#   define ASYS_ONLY_MULTIBYTE_STRING(WIDE_STRING) WIDE_STRING
# endif /* UNICODE */

# if defined (ACE_HAS_MOSTLY_UNICODE_APIS)
#   define ASYS_WIDE_STRING(ASCII_STRING) ACE_OS_CString (ASCII_STRING).wchar_rep ()
# else
#   define ASYS_WIDE_STRING(ASCII_STRING) ASCII_STRING
# endif /* ACE_HAS_MOSTLY_UNICODE_APIS */

// Byte swapping macros to deal with differences between little endian
// and big endian machines.  Note that "long" here refers to 32 bit
// quantities.
# define ACE_SWAP_LONG(L) ((ACE_SWAP_WORD ((L) & 0xFFFF) << 16) \
            | ACE_SWAP_WORD(((L) >> 16) & 0xFFFF))
# define ACE_SWAP_WORD(L) ((((L) & 0x00FF) << 8) | (((L) & 0xFF00) >> 8))

# if defined (ACE_LITTLE_ENDIAN)
#   define ACE_HTONL(X) ACE_SWAP_LONG (X)
#   define ACE_NTOHL(X) ACE_SWAP_LONG (X)
# else
#   define ACE_HTONL(X) X
#   define ACE_NTOHL(X) X
# endif /* ACE_LITTLE_ENDIAN */

# if defined (ACE_LITTLE_ENDIAN)
#   define ACE_HTONS(x) ACE_SWAP_WORD(x)
#   define ACE_NTOHS(x) ACE_SWAP_WORD(x)
# else
#   define ACE_HTONS(x) x
#   define ACE_NTOHS(x) x
# endif /* ACE_LITTLE_ENDIAN */

# if defined (ACE_HAS_AIO_CALLS)
  // = Giving unique ACE scoped names for some important
  // RTSignal-Related constants. Becuase sometimes, different
  // platforms use different names for these constants.

  // Number of realtime signals provided in the system.
  // _POSIX_RTSIG_MAX is the upper limit on the number of real time
  // signals supported in a posix-4 compliant system.
#   if defined (_POSIX_RTSIG_MAX)
#     define ACE_RTSIG_MAX _POSIX_RTSIG_MAX
#   else /* not _POSIX_RTSIG_MAX */
  // POSIX-4 compilant system has to provide atleast 8 RT signals.
  // @@ Make sure the platform does *not* define this constant with
  // some other name. If yes, use that instead of 8.(Alex)
#     define ACE_RTSIG_MAX 8
#   endif /* _POSIX_RTSIG_MAX */

  // The signal used for all the Asynch_Operations.
#   define ACE_SIG_AIO SIGRTMIN

# endif /* ACE_HAS_AIO_CALLS */

  // Wrapping around wait status <wstat> macros for platforms that
  // lack them.

  // Evaluates to a non-zero value if status was returned for a child
  // process that terminated normally.
  // 0 means status wasn't returned.
#if !defined (WIFEXITED)
#   define WIFEXITED(stat) 0
#endif /* WIFEXITED */

  // If the  value  of  WIFEXITED(stat)  is non-zero,  this macro
  // evaluates to the exit  code  that  the  child process exit(3C),
  // or the value that the  child process returned from main.
  // Peaceful exit code is 0.
#if !defined (WEXITSTATUS)
#   define WEXITSTATUS(stat) 0
#endif /* WEXITSTATUS */

  // Evaluates  to  a  non-zero  value   if status  was  returned for
  // a child process  that  terminated  due   to   the receipt of a
  // signal.
  // 0 means status wasnt returned.
#if !defined (WIFSIGNALED)
#   define WIFSIGNALED(stat) 0
#endif /* WIFSIGNALED */

  // If the value of  WIFSIGNALED(stat)  is non-zero,  this macro
  // evaluates to the number of the signal that  caused  the
  // termination of the child process.
#if !defined (WTERMSIG)
#   define WTERMSIG(stat) 0
#endif /* WTERMSIG */

#if !defined (WIFSTOPPED)
#   define WIFSTOPPED(stat) 0
#endif /* WIFSTOPPED */

#if !defined (WSTOPSIG)
#   define WSTOPSIG(stat) 0
#endif /* WSTOPSIG */

#if !defined (WIFCONTINUED)
#   define WIFCONTINUED(stat) 0
#endif /* WIFCONTINUED */

#if !defined (WCOREDUMP)
#   define WCOREDUMP(stat) 0
#endif /* WCOREDUMP */

// Stuff used by the ACE CDR classes.
#if defined ACE_LITTLE_ENDIAN
#  define ACE_CDR_BYTE_ORDER 1
// little endian encapsulation byte order has value = 1
#else  /* ! ACE_LITTLE_ENDIAN */
#  define ACE_CDR_BYTE_ORDER 0
// big endian encapsulation byte order has value = 0
#endif /* ! ACE_LITTLE_ENDIAN */

// Default constants for ACE CDR performance optimizations.
#define ACE_DEFAULT_CDR_BUFSIZE 512
#define ACE_DEFAULT_CDR_EXP_GROWTH_MAX 4096
#define ACE_DEFAULT_CDR_LINEAR_GROWTH_CHUNK 4096
// Even though the strategy above minimizes copies in some cases it is
// more efficient just to copy the octet sequence, for instance, while
// enconding a "small" octet sequence in a buffer that has enough
// space. This parameter controls the default value for "small enough",
// but an ORB may use a different value, set from a command line option.
#define ACE_DEFAULT_CDR_MEMCPY_TRADEOFF 256

// In some environments it is useful to swap the bytes on write, for
// instance: a fast server can be feeding a lot of slow clients that
// happen to have the wrong byte order.
// This macro enables the functionality to support that, but we still
// need a way to activate it on a per-connection basis.
//
// #define ACE_ENABLE_SWAP_ON_WRITE

// In some environements we never need to swap bytes when reading, for
// instance embebbed systems (such as avionics) or homogenous
// networks.
// Setting this macro disables the capabilities to demarshall streams
// in the wrong byte order.
//
// #define ACE_DISABLE_SWAP_ON_READ

// For some applications it is important to optimize octet sequences
// and minimize the number of copies made of the sequence buffer.
// TAO supports this optimizations by sharing the CDR stream buffer
// and the octet sequences buffer via ACE_Message_Block's.
// This feature can be disabled for: debugging, performance
// comparisons, complaince checking (the octet sequences add an API to
// access the underlying message block).


// Typedefs and macros for efficient ACE CDR memory address
// boundary alignment

// Type for doing arithmetic on pointers ... as elsewhere, we assume
// that "unsigned" versions of a type are the same size as the
// "signed" version of the same type.

#if     ACE_SIZEOF_VOID_P == ACE_SIZEOF_INT
typedef u_int ptr_arith_t;

#elif   ACE_SIZEOF_VOID_P == ACE_SIZEOF_LONG
typedef u_long ptr_arith_t;

#elif   ACE_SIZEOF_VOID_P == ACE_SIZEOF_LONG_LONG
typedef u_long long ptr_arith_t;

#else
#       error "Can't find a suitable type for doing pointer arithmetic."
#endif /* error */

// Efficiently align "value" up to "alignment", knowing that all such
// boundaries are binary powers and that we're using two's complement
// arithmetic.

// Since the alignment is a power of two its binary representation is:
//    alignment      = 0...010...0
//
// hence
//
//    alignment - 1  = 0...001...1 = T1
//
// so the complement is:
//
//  ~(alignment - 1) = 1...110...0 = T2
//
// Notice that there is a multiple of <alignment> in the range
// [<value>,<value> + T1], also notice that if
//
// X = ( <value> + T1 ) & T2
//
// then
//
// <value> <= X <= <value> + T1
//
// because the & operator only changes the last bits, and since X is a
// multiple of <alignment> (its last bits are zero) we have found the
// multiple we wanted.
//

#define align_binary(ptr, alignment) \
    ((ptr + ((ptr_arith_t)((alignment)-1))) & (~((ptr_arith_t)((alignment)-1))))

// Efficiently round "ptr" up to an "alignment" boundary, knowing that
// all such boundaries are binary powers and that we're using two's
// complement arithmetic.
//
#define ptr_align_binary(ptr, alignment) \
        ((char *) align_binary (((ptr_arith_t) (ptr)), (alignment)))

#endif  /* ACE_OS_H */
