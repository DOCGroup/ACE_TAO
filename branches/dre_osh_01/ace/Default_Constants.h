// -*- C++ -*-

//=============================================================================
/**
 *  @file   Default_Constants.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  This one is split from the famous OS.h
 */
//=============================================================================

#ifndef ACE_DEFAULT_CONSTANTS_H
#define ACE_DEFAULT_CONSTANTS_H
#include "ace/pre.h"

// Included just keep compilers that see #pragma dierctive first
// happy.
#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


// Define the default constants for ACE.  Many of these are used for
// the ACE tests and applications.  You can change these values by
// defining the macros in your config.h file.
# if !defined (ACE_DEFAULT_CLOSE_ALL_HANDLES)
#   define ACE_DEFAULT_CLOSE_ALL_HANDLES 1
# endif /* ACE_DEFAULT_CLOSE_ALL_HANDLES */

// The maximum length for a fully qualified Internet name.
# if !defined(ACE_MAX_FULLY_QUALIFIED_NAME_LEN)
#   define ACE_MAX_FULLY_QUALIFIED_NAME_LEN 256
# endif /* ACE_MAX_FULLY_QUALIFIED_NAME_LEN */

#if !defined (ACE_DEFAULT_PAGEFILE_POOL_BASE)
#define ACE_DEFAULT_PAGEFILE_POOL_BASE (void *) 0
#endif /* ACE_DEFAULT_PAGEFILE_POOL_BASE */

#if !defined (ACE_DEFAULT_PAGEFILE_POOL_SIZE)
#define ACE_DEFAULT_PAGEFILE_POOL_SIZE (size_t) 0x01000000
#endif /* ACE_DEFAULT_PAGEFILE_POOL_SIZE */

#if !defined (ACE_DEFAULT_PAGEFILE_POOL_CHUNK)
#define ACE_DEFAULT_PAGEFILE_POOL_CHUNK (size_t) 0x00010000
#endif /* ACE_DEFAULT_PAGEFILE_POOL_CHUNK */

#if !defined (ACE_DEFAULT_PAGEFILE_POOL_NAME)
#define ACE_DEFAULT_PAGEFILE_POOL_NAME ACE_LIB_TEXT ("Default_ACE_Pagefile_Memory_Pool")
#endif /* ACE_DEFAULT_PAGEFILE_POOL_NAME */

#if !defined (ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY)
#define ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY 0
#endif /* ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY */

#if !defined (ACE_DEFAULT_SERVICE_REPOSITORY_SIZE)
#define ACE_DEFAULT_SERVICE_REPOSITORY_SIZE 1024
#endif /* ACE_DEFAULT_SERVICE_REPOSITORY_SIZE */

#if !defined (ACE_REACTOR_NOTIFICATION_ARRAY_SIZE)
#define ACE_REACTOR_NOTIFICATION_ARRAY_SIZE 1024
#endif /* ACE_REACTOR_NOTIFICATION_ARRAY_SIZE */

# if !defined (ACE_DEFAULT_TIMEOUT)
#   define ACE_DEFAULT_TIMEOUT 5
# endif /* ACE_DEFAULT_TIMEOUT */

# if !defined (ACE_DEFAULT_BACKLOG)
#   define ACE_DEFAULT_BACKLOG 5
# endif /* ACE_DEFAULT_BACKLOG */

# if !defined (ACE_DEFAULT_THREADS)
#   define ACE_DEFAULT_THREADS 1
# endif /* ACE_DEFAULT_THREADS */

// The following 3 defines are used in the IP multicast and broadcast tests.
# if !defined (ACE_DEFAULT_BROADCAST_PORT)
#   define ACE_DEFAULT_BROADCAST_PORT 20000
# endif /* ACE_DEFAULT_BROADCAST_PORT */

# if !defined (ACE_DEFAULT_MULTICAST_PORT)
#   define ACE_DEFAULT_MULTICAST_PORT 20001
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
#   define ACE_DEFAULT_SERVER_PORT 20002
# endif /* ACE_DEFAULT_SERVER_PORT */

# if !defined (ACE_DEFAULT_HTTP_PORT)
#   define ACE_DEFAULT_HTTP_PORT 80
# endif /* ACE_DEFAULT_HTTP_PORT */

# if !defined (ACE_DEFAULT_MAX_SOCKET_BUFSIZ)
#   define ACE_DEFAULT_MAX_SOCKET_BUFSIZ 65536
# endif /* ACE_DEFAULT_MAX_SOCKET_BUFSIZ */

# if !defined (ACE_DEFAULT_SERVER_PORT_STR)
#   define ACE_DEFAULT_SERVER_PORT_STR "20002"
# endif /* ACE_DEFAULT_SERVER_PORT_STR */

// Used for the Service_Directory test
# if !defined (ACE_DEFAULT_SERVICE_PORT)
#   define ACE_DEFAULT_SERVICE_PORT 20003
# endif /* ACE_DEFAULT_SERVICE_PORT */

// Used for the ACE_Thread_Spawn test
# if !defined (ACE_DEFAULT_THR_PORT    )
#   define ACE_DEFAULT_THR_PORT 20004
# endif /* ACE_DEFAULT_THR_PORT */

// Used for <SOCK_Connect::connect> tests
# if !defined (ACE_DEFAULT_LOCAL_PORT)
#   define ACE_DEFAULT_LOCAL_PORT 20005
# endif /* ACE_DEFAULT_LOCAL_PORT */

// Used for Connector tests
# if !defined (ACE_DEFAULT_LOCAL_PORT_STR)
#   define ACE_DEFAULT_LOCAL_PORT_STR "20005"
# endif /* ACE_DEFAULT_LOCAL_PORT_STR */

// Used for the name server.
# if !defined (ACE_DEFAULT_NAME_SERVER_PORT)
#   define ACE_DEFAULT_NAME_SERVER_PORT 20006
# endif /* ACE_DEFAULT_NAME_SERVER_PORT */

# if !defined (ACE_DEFAULT_NAME_SERVER_PORT_STR)
#   define ACE_DEFAULT_NAME_SERVER_PORT_STR "20006"
# endif /* ACE_DEFAULT_NAME_SERVER_PORT_STR */

// Used for the token server.
# if !defined (ACE_DEFAULT_TOKEN_SERVER_PORT)
#   define ACE_DEFAULT_TOKEN_SERVER_PORT 20007
# endif /* ACE_DEFAULT_TOKEN_SERVER_PORT */

# if !defined (ACE_DEFAULT_TOKEN_SERVER_PORT_STR)
#   define ACE_DEFAULT_TOKEN_SERVER_PORT_STR "20007"
# endif /* ACE_DEFAULT_TOKEN_SERVER_PORT_STR */

// Used for the logging server.
# if !defined (ACE_DEFAULT_LOGGING_SERVER_PORT)
#   define ACE_DEFAULT_LOGGING_SERVER_PORT 20008
# endif /* ACE_DEFAULT_LOGGING_SERVER_PORT */

# if !defined (ACE_DEFAULT_LOGGING_SERVER_PORT_STR)
#   define ACE_DEFAULT_LOGGING_SERVER_PORT_STR "20008"
# endif /* ACE_DEFAULT_LOGGING_SERVER_PORT_STR */

// Used for the logging server.
# if !defined (ACE_DEFAULT_THR_LOGGING_SERVER_PORT)
#   define ACE_DEFAULT_THR_LOGGING_SERVER_PORT 20008
# endif /* ACE_DEFAULT_THR_LOGGING_SERVER_PORT */

# if !defined (ACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR)
#   define ACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR "20008"
# endif /* ACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR */

// Used for the time server.
# if !defined (ACE_DEFAULT_TIME_SERVER_PORT)
#   define ACE_DEFAULT_TIME_SERVER_PORT 20009
# endif /* ACE_DEFAULT_TIME_SERVER_PORT */

# if !defined (ACE_DEFAULT_TIME_SERVER_PORT_STR)
#   define ACE_DEFAULT_TIME_SERVER_PORT_STR "20009"
# endif /* ACE_DEFAULT_TIME_SERVER_PORT_STR */

# if !defined (ACE_DEFAULT_TIME_SERVER_STR)
#   define ACE_DEFAULT_TIME_SERVER_STR "ACE_TS_TIME"
# endif /* ACE_DEFAULT_TIME_SERVER_STR */

// Used by the FIFO tests and the Client_Logging_Handler netsvc.
# if !defined (ACE_DEFAULT_RENDEZVOUS)
#   if defined (ACE_HAS_STREAM_PIPES)
#     define ACE_DEFAULT_RENDEZVOUS ACE_LIB_TEXT("/tmp/fifo.ace")
#   else
#     define ACE_DEFAULT_RENDEZVOUS ACE_LIB_TEXT("localhost:20010")
#   endif /* ACE_HAS_STREAM_PIPES */
# endif /* ACE_DEFAULT_RENDEZVOUS */

// Used for the UNIX syslog logging interface to ACE_Log_Msg.
# ifndef ACE_DEFAULT_SYSLOG_FACILITY
# define ACE_DEFAULT_SYSLOG_FACILITY LOG_USER
# endif /* ACE_DEFAULT_SYSLOG_FACILITY */

# if !defined (ACE_DEFAULT_LOGGER_KEY)

#     if defined (ACE_HAS_STREAM_PIPES)
#       define ACE_DEFAULT_LOGGER_KEY ACE_LIB_TEXT ("/tmp/server_daemon")
#     else
#       define ACE_DEFAULT_LOGGER_KEY ACE_LIB_TEXT ("localhost:20012")
#     endif /* ACE_HAS_STREAM_PIPES */
# endif /* ACE_DEFAULT_LOGGER_KEY */

// The way to specify the local host for loopback IP. This is usually
// "localhost" but it may need changing on some platforms.
# if !defined (ACE_LOCALHOST)
#   define ACE_LOCALHOST ACE_LIB_TEXT ("localhost")
# endif

# if !defined (ACE_DEFAULT_SERVER_HOST)
#   define ACE_DEFAULT_SERVER_HOST ACE_LOCALHOST
# endif /* ACE_DEFAULT_SERVER_HOST */

// Default shared memory key
# if !defined (ACE_DEFAULT_SHM_KEY)
#   define ACE_DEFAULT_SHM_KEY 1234
# endif /* ACE_DEFAULT_SHM_KEY */

// Default address for shared memory mapped files and SYSV shared memory
// (defaults to 64 M).
# if !defined (ACE_DEFAULT_BASE_ADDR)
#   define ACE_DEFAULT_BASE_ADDR ((char *) (64 * 1024 * 1024))
# endif /* ACE_DEFAULT_BASE_ADDR */

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
#   if defined (ACE_WIN32)
#     if defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 == 1)
#       define ACE_DEFAULT_FILE_PERMS (FILE_SHARE_READ | FILE_SHARE_WRITE | \
                                       FILE_SHARE_DELETE)
#     else
#       define ACE_DEFAULT_FILE_PERMS (FILE_SHARE_READ | FILE_SHARE_WRITE)
#     endif /* ACE_HAS_WINNT4 */
#   elif defined (VXWORKS)
#     define ACE_DEFAULT_FILE_PERMS (S_IRUSR | S_IWUSR| S_IRGRP| S_IROTH)
#   else
#     define ACE_DEFAULT_FILE_PERMS 0644
#   endif /* ACE_WIN32 */
# endif /* ACE_DEFAULT_FILE_PERMS */

// Default directory permissions.
# if !defined (ACE_DEFAULT_DIR_PERMS)
#   define ACE_DEFAULT_DIR_PERMS 0755
# endif /* ACE_DEFAULT_DIR_PERMS */

# if !defined (ACE_DEFAULT_TIMEPROBE_TABLE_SIZE)
#   define ACE_DEFAULT_TIMEPROBE_TABLE_SIZE 8 * 1024
# endif /* ACE_DEFAULT_TIMEPROBE_TABLE_SIZE */

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

// A free list which create more elements when there aren't enough
// elements.
# define ACE_FREE_LIST_WITH_POOL 1

// A simple free list which doen't allocate/deallocate elements.
# define ACE_PURE_FREE_LIST 2

# if defined (INTEGRITY)
# define ACE_MAX_USERID 32
# endif

# if defined (ACE_WIN32)

#define ACE_MAX_USERID 32

// This is necessary to work around bugs with Win32 non-blocking
// connects...
#   if !defined (ACE_NON_BLOCKING_BUG_DELAY)
#     define ACE_NON_BLOCKING_BUG_DELAY 35000
#   endif /* ACE_NON_BLOCKING_BUG_DELAY */
# endif /*ACE_WIN32*/

// Max size of an ACE Log Record data buffer.  This can be reset in
// the config.h file if you'd like to increase or decrease the size.
# if !defined (ACE_MAXLOGMSGLEN)
#   define ACE_MAXLOGMSGLEN 4 * 1024
# endif /* ACE_MAXLOGMSGLEN */

// Max size of an ACE Token.
# define ACE_MAXTOKENNAMELEN 40

// Max size of an ACE Token client ID.
# define ACE_MAXCLIENTIDLEN MAXHOSTNAMELEN + 20

/**
 * @name Default values to control CDR classes memory allocation strategies
 */
//@{

/// Control the initial size of all CDR buffers, application
/// developers may want to optimize this value to fit their request
/// size
#if !defined (ACE_DEFAULT_CDR_BUFSIZE)
#  define ACE_DEFAULT_CDR_BUFSIZE 512
#endif /* ACE_DEFAULT_CDR_BUFSIZE */

/// Stop exponential growth of CDR buffers to avoid overallocation
#if !defined (ACE_DEFAULT_CDR_EXP_GROWTH_MAX)
#  define ACE_DEFAULT_CDR_EXP_GROWTH_MAX 65536
#endif /* ACE_DEFAULT_CDR_EXP_GROWTH_MAX */

/// Control CDR buffer growth after maximum exponential growth is
/// reached
#if !defined (ACE_DEFAULT_CDR_LINEAR_GROWTH_CHUNK)
#  define ACE_DEFAULT_CDR_LINEAR_GROWTH_CHUNK 65536
#endif /* ACE_DEFAULT_CDR_LINEAR_GROWTH_CHUNK */
//@}

/// Control the zero-copy optimizations for octet sequences
/**
 * Large octet sequences can be sent without any copies by chaining
 * them in the list of message blocks that represent a single CDR
 * stream.  However, if the octet sequence is too small the zero copy
 * optimizations actually hurt performance.  Octet sequences smaller
 * than this value will be copied.
 */
#if !defined (ACE_DEFAULT_CDR_MEMCPY_TRADEOFF)
#define ACE_DEFAULT_CDR_MEMCPY_TRADEOFF 256
#endif /* ACE_DEFAULT_CDR_MEMCPY_TRADEOFF */

#if !defined (ACE_CONNECTOR_HANDLER_MAP_SIZE)
const unsigned int ACE_CONNECTOR_HANDLER_MAP_SIZE = 16;
#endif /*ACE_CONNECTOR_HANDLER_MAP_SIZE*/

#include "ace/post.h"
#endif /*ACE_DEFAULT_CONSTANTS_H*/
