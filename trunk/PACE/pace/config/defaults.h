/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    defaults.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_CONFIG_DEFAULTS_H
#define PACE_CONFIG_DEFAULTS_H


/* Define the default constants for PACE.  Many of these are used for
 * the PACE tests and applications.  You can change these values by
 * defining the macros in your config.h file. */

# if !defined (PACE_DEFAULT_TIMEOUT)
#   define PACE_DEFAULT_TIMEOUT 5
# endif /* PACE_DEFAULT_TIMEOUT */

# if !defined (PACE_DEFAULT_BACKLOG)
#   define PACE_DEFAULT_BACKLOG 5
# endif /* PACE_DEFAULT_BACKLOG */

# if !defined (PACE_DEFAULT_THREADS)
#   define PACE_DEFAULT_THREADS 1
# endif /* PACE_DEFAULT_THREADS */

/* The following 3 defines are used in the IP multicast and broadcast tests. */
# if !defined (PACE_DEFAULT_BROADCAST_PORT)
#   define PACE_DEFAULT_BROADCAST_PORT 10000
# endif /* PACE_DEFAULT_BROADCAST_PORT */

# if !defined (PACE_DEFAULT_MULTICAST_PORT)
#   define PACE_DEFAULT_MULTICAST_PORT 10001
# endif /* PACE_DEFAULT_MULTICAST_PORT */

# if !defined (PACE_DEFAULT_MULTICAST_ADDR)
/* This address MUST be within the range for host group addresses:
 * 224.0.0.0 to 239.255.255.255. */
#   define PACE_DEFAULT_MULTICAST_ADDR "224.9.9.2"
# endif /* PACE_DEFAULT_MULTICAST_ADDR */

/* Default port number for HTTP. */
# if !defined (PACE_DEFAULT_HTTP_SERVER_PORT)
#   define PACE_DEFAULT_HTTP_SERVER_PORT 80
# endif /* PACE_DEFAULT_HTTP_SERVER_PORT */

/* Used in many IPC_SAP tests */
# if !defined (PACE_DEFAULT_SERVER_PORT)
#   define PACE_DEFAULT_SERVER_PORT 10002
# endif /* PACE_DEFAULT_SERVER_PORT */

# if !defined (PACE_DEFAULT_HTTP_PORT)
#   define PACE_DEFAULT_HTTP_PORT 80
# endif /* PACE_DEFAULT_HTTP_PORT */

# if !defined (PACE_DEFAULT_MAX_SOCKET_BUFSIZ)
#   define PACE_DEFAULT_MAX_SOCKET_BUFSIZ 65536
# endif /* PACE_DEFAULT_MAX_SOCKET_BUFSIZ */

# if !defined (PACE_DEFAULT_SERVER_PORT_STR)
#   define PACE_DEFAULT_SERVER_PORT_STR "10002"
# endif /* PACE_DEFAULT_SERVER_PORT_STR */

/* Used for the Service_Directory test */
# if !defined (PACE_DEFAULT_SERVICE_PORT)
#   define PACE_DEFAULT_SERVICE_PORT 10003
# endif /* PACE_DEFAULT_SERVICE_PORT */

/* Used for the PACE_Thread_Spawn test */
# if !defined (PACE_DEFAULT_THR_PORT    )
#   define PACE_DEFAULT_THR_PORT 10004
# endif /* PACE_DEFAULT_THR_PORT */

/* Used for <SOCK_Connect::connect> tests */
# if !defined (PACE_DEFAULT_LOCAL_PORT)
#   define PACE_DEFAULT_LOCAL_PORT 10005
# endif /* PACE_DEFAULT_LOCAL_PORT */

/* Used for Connector tests */
# if !defined (PACE_DEFAULT_LOCAL_PORT_STR)
#   define PACE_DEFAULT_LOCAL_PORT_STR "10005"
# endif /* PACE_DEFAULT_LOCAL_PORT_STR */

/* Used for the name server. */
# if !defined (PACE_DEFAULT_NAME_SERVER_PORT)
#   define PACE_DEFAULT_NAME_SERVER_PORT 10006
# endif /* PACE_DEFAULT_NAME_SERVER_PORT */

# if !defined (PACE_DEFAULT_NAME_SERVER_PORT_STR)
#   define PACE_DEFAULT_NAME_SERVER_PORT_STR "10006"
# endif /* PACE_DEFAULT_NAME_SERVER_PORT_STR */

/* Used for the token server. */
# if !defined (PACE_DEFAULT_TOKEN_SERVER_PORT)
#   define PACE_DEFAULT_TOKEN_SERVER_PORT 10007
# endif /* PACE_DEFAULT_TOKEN_SERVER_PORT */

# if !defined (PACE_DEFAULT_TOKEN_SERVER_PORT_STR)
#   define PACE_DEFAULT_TOKEN_SERVER_PORT_STR "10007"
# endif /* PACE_DEFAULT_TOKEN_SERVER_PORT_STR */

/* Used for the logging server. */
# if !defined (PACE_DEFAULT_LOGGING_SERVER_PORT)
#   define PACE_DEFAULT_LOGGING_SERVER_PORT 10008
# endif /* PACE_DEFAULT_LOGGING_SERVER_PORT */

# if !defined (PACE_DEFAULT_LOGGING_SERVER_PORT_STR)
#   define PACE_DEFAULT_LOGGING_SERVER_PORT_STR "10008"
# endif /* PACE_DEFAULT_LOGGING_SERVER_PORT_STR */

/* Used for the logging server. */
# if !defined (PACE_DEFAULT_THR_LOGGING_SERVER_PORT)
#   define PACE_DEFAULT_THR_LOGGING_SERVER_PORT 10008
# endif /* PACE_DEFAULT_THR_LOGGING_SERVER_PORT */

# if !defined (PACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR)
#   define PACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR "10008"
# endif /* PACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR */

/* Used for the time server. */
# if !defined (PACE_DEFAULT_TIME_SERVER_PORT)
#   define PACE_DEFAULT_TIME_SERVER_PORT 10009
# endif /* PACE_DEFAULT_TIME_SERVER_PORT */

# if !defined (PACE_DEFAULT_TIME_SERVER_PORT_STR)
#   define PACE_DEFAULT_TIME_SERVER_PORT_STR "10009"
# endif /* PACE_DEFAULT_TIME_SERVER_PORT_STR */

# if !defined (PACE_DEFAULT_TIME_SERVER_STR)
#   define PACE_DEFAULT_TIME_SERVER_STR "PACE_TS_TIME"
# endif /* PACE_DEFAULT_TIME_SERVER_STR */

/* Used by the FIFO tests and the Client_Logging_Handler netsvc. */
# if !defined (PACE_DEFAULT_RENDEZVOUS)
#   if defined (PACE_HAS_STREAM_PIPES)
#     define PACE_DEFAULT_RENDEZVOUS "/tmp/fifo.PACE"
#   else
#     define PACE_DEFAULT_RENDEZVOUS "localhost:10010"
#   endif /* PACE_HAS_STREAM_PIPES */
# endif /* PACE_DEFAULT_RENDEZVOUS */

# if !defined (PACE_DEFAULT_LOGGER_KEY)
#   if defined (PACE_HAS_UNICODE) && defined (UNICODE)
#     if defined (PACE_HAS_STREAM_PIPES)
#       define PACE_DEFAULT_LOGGER_KEY L"/tmp/server_daemon"
#     else
#       define PACE_DEFAULT_LOGGER_KEY L"localhost:10012"
#     endif /* PACE_HAS_STREAM_PIPES */
#   else /* PACE_HAS_UNICODE */
#     if defined (PACE_HAS_STREAM_PIPES)
#       define PACE_DEFAULT_LOGGER_KEY "/tmp/server_daemon"
#     else
#       define PACE_DEFAULT_LOGGER_KEY "localhost:10012"
#     endif /* PACE_HAS_STREAM_PIPES */
#   endif /* PACE_HAS_UNICODE && UNICODE */
# endif /* PACE_DEFAULT_LOGGER_KEY */

/* The way to specify the local host for loopback IP. This is usually
 * "localhost" but it may need changing on some platforms. */
# if !defined (PACE_LOCALHOST)
#   define PACE_LOCALHOST ASYS_TEXT("localhost")
# endif

# if !defined (PACE_DEFAULT_SERVER_HOST)
#   define PACE_DEFAULT_SERVER_HOST PACE_LOCALHOST
# endif /* PACE_DEFAULT_SERVER_HOST */

/* Default shared memory key */
# if !defined (PACE_DEFAULT_SHM_KEY)
#   define PACE_DEFAULT_SHM_KEY 1234
# endif /* PACE_DEFAULT_SHM_KEY */

/* Default segment size used by SYSV shared memory (128 K) */
# if !defined (PACE_DEFAULT_SEGMENT_SIZE)
#   define PACE_DEFAULT_SEGMENT_SIZE 1024 * 128
# endif /* PACE_DEFAULT_SEGMENT_SIZE */

/* Maximum number of SYSV shared memory segments
 * (does anyone know how to figure out the right values?!) */
# if !defined (PACE_DEFAULT_MAX_SEGMENTS)
#   define PACE_DEFAULT_MAX_SEGMENTS 6
# endif /* PACE_DEFAULT_MAX_SEGMENTS */

/* Name of the map that's stored in shared memory. */
# if !defined (PACE_NAME_SERVER_MAP)
#   define PACE_NAME_SERVER_MAP "Name Server Map"
# endif /* PACE_NAME_SERVER_MAP */

/* Default file permissions. */
# if !defined (PACE_DEFAULT_FILE_PERMS)
#   define PACE_DEFAULT_FILE_PERMS 0666
# endif /* PACE_DEFAULT_FILE_PERMS */

/* Default directory permissions. */
# if !defined (PACE_DEFAULT_DIR_PERMS)
#   define PACE_DEFAULT_DIR_PERMS 0777
# endif /* PACE_DEFAULT_DIR_PERMS */

/* Default size of the PACE Reactor. */
# if !defined (PACE_DEFAULT_SELECT_REACTOR_SIZE)
#   define PACE_DEFAULT_SELECT_REACTOR_SIZE FD_SETSIZE
# endif /* PACE_DEFAULT_SELECT_REACTOR_SIZE */

# if !defined (PACE_DEFAULT_TIMEPROBE_TABLE_SIZE)
#   define PACE_DEFAULT_TIMEPROBE_TABLE_SIZE 8 * 1024
# endif /* PACE_DEFAULT_TIMEPROBE_TABLE_SIZE */

/* Default size of the PACE Map_Manager. */
# if !defined (PACE_DEFAULT_MAP_SIZE)
#   define PACE_DEFAULT_MAP_SIZE 1024
# endif /* PACE_DEFAULT_MAP_SIZE */

/* Defaults for PACE Timer Wheel */
# if !defined (PACE_DEFAULT_TIMER_WHEEL_SIZE)
#   define PACE_DEFAULT_TIMER_WHEEL_SIZE 1024
# endif /* PACE_DEFAULT_TIMER_WHEEL_SIZE */

# if !defined (PACE_DEFAULT_TIMER_WHEEL_RESOLUTION)
#   define PACE_DEFAULT_TIMER_WHEEL_RESOLUTION 100
# endif /* PACE_DEFAULT_TIMER_WHEEL_RESOLUTION */

/* Default size for PACE Timer Hash table */
# if !defined (PACE_DEFAULT_TIMER_HASH_TABLE_SIZE)
#   define PACE_DEFAULT_TIMER_HASH_TABLE_SIZE 1024
# endif /* PACE_DEFAULT_TIMER_HASH_TABLE_SIZE */

/* Defaults for the PACE Free List */
# if !defined (PACE_DEFAULT_FREE_LIST_PREALLOC)
#   define PACE_DEFAULT_FREE_LIST_PREALLOC 0
# endif /* PACE_DEFAULT_FREE_LIST_PREALLOC */

# if !defined (PACE_DEFAULT_FREE_LIST_LWM)
#   define PACE_DEFAULT_FREE_LIST_LWM 0
# endif /* PACE_DEFAULT_FREE_LIST_LWM */

# if !defined (PACE_DEFAULT_FREE_LIST_HWM)
#   define PACE_DEFAULT_FREE_LIST_HWM 25000
# endif /* PACE_DEFAULT_FREE_LIST_HWM */

# if !defined (PACE_DEFAULT_FREE_LIST_INC)
#   define PACE_DEFAULT_FREE_LIST_INC 100
# endif /* PACE_DEFAULT_FREE_LIST_INC */

# if !defined (PACE_UNIQUE_NAME_LEN)
#   define PACE_UNIQUE_NAME_LEN 100
# endif /* PACE_UNIQUE_NAME_LEN */

# if !defined (PACE_MAX_DGRAM_SIZE)
  /* This is just a guess.  8k is the normal limit on
   * most machines because that's what NFS expects. */
#   define PACE_MAX_DGRAM_SIZE 8192
# endif /* PACE_MAX_DGRAM_SIZE */

# if !defined (PACE_DEFAULT_ARGV_BUFSIZ)
#   define PACE_DEFAULT_ARGV_BUFSIZ 1024 * 4
# endif /* PACE_DEFAULT_ARGV_BUFSIZ */

# if !defined (PACE_DEFAULT_PAGEFILE_POOL_BASE)
#   define PACE_DEFAULT_PAGEFILE_POOL_BASE (void *) 0
# endif /* PACE_DEFAULT_PAGEFILE_POOL_BASE */

# if !defined (PACE_DEFAULT_PAGEFILE_POOL_SIZE)
#   define PACE_DEFAULT_PAGEFILE_POOL_SIZE (size_t) 0x01000000
# endif /* PACE_DEFAULT_PAGEFILE_POOL_SIZE */

# if !defined (PACE_DEFAULT_PAGEFILE_POOL_CHUNK)
#   define PACE_DEFAULT_PAGEFILE_POOL_CHUNK (size_t) 0x00010000
# endif /* PACE_DEFAULT_PAGEFILE_POOL_CHUNK */

# if !defined (PACE_DEFAULT_PAGEFILE_POOL_NAME)
#   define PACE_DEFAULT_PAGEFILE_POOL_NAME ACE_TEXT ("Default_ACE_Pagefile_Memory_Pool")
# endif /* PACE_DEFAULT_PAGEFILE_POOL_NAME */

# if !defined (PACE_DEFAULT_MESSAGE_BLOCK_PRIORITY)
#   define PACE_DEFAULT_MESSAGE_BLOCK_PRIORITY 0
# endif /* PACE_DEFAULT_MESSAGE_BLOCK_PRIORITY */

# if !defined (PACE_DEFAULT_SERVICE_REPOSITORY_SIZE)
#   define PACE_DEFAULT_SERVICE_REPOSITORY_SIZE 1024
# endif /* PACE_DEFAULT_SERVICE_REPOSITORY_SIZE */

# if !defined (PACE_REACTOR_NOTIFICATION_ARRAY_SIZE)
#   define PACE_REACTOR_NOTIFICATION_ARRAY_SIZE 1024
# endif /* PACE_REACTOR_NOTIFICATION_ARRAY_SIZE */

# if !defined (PACE_DEFAULT_CLOSE_ALL_HANDLES)
#   define PACE_DEFAULT_CLOSE_ALL_HANDLES 1
# endif /* PACE_DEFAULT_CLOSE_ALL_HANDLES */


#endif /* PACE_CONFIG_DEFAULTS_H */
