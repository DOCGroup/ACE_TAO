/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    OS.h
//
// = AUTHOR
//    Doug Schmidt <schmidt@cs.wustl.edu>, Jesper S. M|ller
//    <stophph@diku.dk>, and a cast of thousands...
// 
// ============================================================================

#if !defined (ACE_OS_H)
#define ACE_OS_H

// Define the default constants for ACE.  Many of these are used for
// the ACE tests and applications.  You may want to change some of
// these to correspond to your environment.

#define ACE_DEFAULT_TIMEOUT 5
#define ACE_DEFAULT_THREADS 1

// The following 3 defines are used in the IP multicast and broadcast tests.
#define ACE_DEFAULT_BROADCAST_PORT 10000
#define ACE_DEFAULT_MULTICAST_PORT 10001
#define ACE_DEFAULT_MULTICAST_ADDR "224.9.9.2"

// Used in many IPC_SAP tests
#define ACE_DEFAULT_SERVER_PORT 10002  

// Used in Acceptor tests
#define ACE_DEFAULT_SERVER_PORT_STR "10002"  

// Used for the Service_Directory test
#define ACE_DEFAULT_SERVICE_PORT 10003 

// Used for the ACE_Thread_Spawn test
#define ACE_DEFAULT_THR_PORT     10004 

// Used for SOCK_Connect::connect() tests
#define ACE_DEFAULT_LOCAL_PORT 10005  

// Used for Connector tests
#define ACE_DEFAULT_LOCAL_PORT_STR "10005" 

// Used for the name server.
#define ACE_DEFAULT_NAME_SERVER_PORT 10006
#define ACE_DEFAULT_NAME_SERVER_PORT_STR "10006"

// Used for the token server.
#define ACE_DEFAULT_TOKEN_SERVER_PORT 10007
#define ACE_DEFAULT_TOKEN_SERVER_PORT_STR "10007"

// Used for the logging server.
#define ACE_DEFAULT_LOGGING_SERVER_PORT 10008
#define ACE_DEFAULT_LOGGING_SERVER_PORT_STR "10008"

// Used for the logging server.
#define ACE_DEFAULT_THR_LOGGING_SERVER_PORT 10008
#define ACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR "10008"

// Used for the time server.
#define ACE_DEFAULT_TIME_SERVER_PORT 10010
#define ACE_DEFAULT_TIME_SERVER_PORT_STR "10010"
#define ACE_DEFAULT_TIME_SERVER_STR "ACE_TS_TIME"

#define ACE_DEFAULT_SERVER_HOST "localhost"

// Default shared memory key
#define ACE_DEFAULT_SHM_KEY 1234 

// Default address for shared memory mapped files and SYSV shared memory
// (defaults to 64 M).
#define ACE_DEFAULT_BASE_ADDR ((char *) (64 * 1024 * 1024)) 

// Default segment size used by SYSV shared memory (128 K)
#define ACE_DEFAULT_SEGMENT_SIZE 1024 * 128

// Maximum number of SYSV shared memory segments 
// (does anyone know how to figure out the right values?!)
#define ACE_DEFAULT_MAX_SEGMENTS 6

// Used by the FIFO tests.
#define ACE_DEFAULT_RENDEZVOUS "/tmp/fifo.ace"

// Name of the map that's stored in shared memory.
#define ACE_NAME_SERVER_MAP "Name Server Map"

// Default file permissions.
#define ACE_DEFAULT_FILE_PERMS 0666

// Default directory permissions.
#define ACE_DEFAULT_DIR_PERMS 0777

// Default size of the ACE Reactor.
#define ACE_DEFAULT_REACTOR_SIZE FD_SETSIZE

// Default size of the ACE Map_Manager.
#define ACE_DEFAULT_MAP_SIZE 1024

// Here are all ACE-specific global declarations needed throughout
// ACE.

// Helpful dump macros.
#define ACE_BEGIN_DUMP "\n====\n(%P|%t|%x)"
#define ACE_END_DUMP "====\n"

// This is used to indicate that a platform doesn't support a
// particular feature.
#define ACE_NOTSUP_RETURN(FAILVALUE) do { errno = ENOTSUP ; return FAILVALUE; } while (0)

#if defined (ACE_NDEBUG)
#define ACE_DB(X)
#else
#define ACE_DB(X) X
#endif /* ACE_NDEBUG */

// 10 millisecond fudge factor to account for Solaris timers...
#define ACE_TIMER_SKEW 1000 * 10

// Increase the range of "address families".
#define AF_SPIPE (AF_MAX + 1)
#define AF_FILE (AF_MAX + 2)
#define AF_DEV (AF_MAX + 3)
#define AF_UPIPE (AF_SPIPE)

// Turn a number into a string.
#define ACE_ITOA(X) #X

// Create a string of a server address with a "host:port" format.
#define ACE_SERVER_ADDRESS(H,P) H":"P

// A couple useful inline functions for checking whether bits are 
// enabled or disabled. 

// Efficiently returns the least power of two >= X...
#define ACE_POW(X) ((!X)?1:(X-=1,X|=X>>1,X|=X>>2,X|=X>>4,X|=X>>8,X|=X>>16,(++X)))
#define ACE_EVEN(NUM) (((NUM) & 1) == 0)
#define ACE_ODD(NUM) (((NUM) & 1) == 1)
#define ACE_BIT_ENABLED(WORD, BIT) (((WORD) & (BIT)) != 0)
#define ACE_BIT_DISABLED(WORD, BIT) (((WORD) & (BIT)) == 0)
#define ACE_SET_BITS(WORD, BITS) (WORD |= (BITS))
#define ACE_CLR_BITS(WORD, BITS) (WORD &= ~(BITS))

// These hooks enable ACE to have all dynamic memory management
// automatically handled on a per-object basis.

#if defined (ACE_HAS_ALLOC_HOOKS)
#define ACE_ALLOC_HOOK_DECLARE \
  void *operator new (size_t bytes); \
  void operator delete (void *ptr);

// Note that these are just place holders for now.  They'll
// be replaced by the ACE_Malloc stuff shortly...
#define ACE_ALLOC_HOOK_DEFINE(CLASS) \
  void *CLASS::operator new (size_t bytes) { return ::new char[bytes]; } \
  void CLASS::operator delete (void *ptr) { delete (ptr); }
#else
#define ACE_ALLOC_HOOK_DECLARE struct __Ace {} // Just need a dummy...
#define ACE_ALLOC_HOOK_DEFINE(CLASS) 
#endif /* ACE_HAS_ALLOC_HOOKS */

#if defined (VXWORKS)

typedef int key_t;
#include /**/ <vxWorks.h>
#endif /* VXWORKS */

#include "ace/Time_Value.h"

#if defined (ACE_HAS_USING_KEYWORD)
#define	ACE_USING using
#else
#define ACE_USING
#endif /* ACE_HAS_USING_KEYWORD */

// The following is necessary since many C++ compilers don't support
// typedef'd types inside of classes used as formal template
// arguments... ;-(.  Luckily, using the C++ preprocessor I can hide
// most of this nastiness!

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)

// Handle ACE_Message_Queue.
#define ACE_SYNCH_1 class _ACE_SYNCH
#define ACE_SYNCH_2 _ACE_SYNCH
#define ACE_SYNCH_MUTEX _ACE_SYNCH::MUTEX
#define ACE_SYNCH_CONDITION _ACE_SYNCH::CONDITION

// Handle ACE_Malloc*
#define ACE_MEM_POOL_1 class _ACE_MEM_POOL
#define ACE_MEM_POOL_2 _ACE_MEM_POOL
#define ACE_MEM_POOL _ACE_MEM_POOL
#define ACE_MEM_POOL_OPTIONS _ACE_MEM_POOL::OPTIONS

// Handle ACE_Svc_Handler
#define ACE_PEER_STREAM_1 class _ACE_PEER_STREAM
#define ACE_PEER_STREAM_2 _ACE_PEER_STREAM
#define ACE_PEER_STREAM _ACE_PEER_STREAM
#define ACE_PEER_STREAM_ADDR _ACE_PEER_STREAM::PEER_ADDR

// Handle ACE_Acceptor
#define ACE_PEER_ACCEPTOR_1 class _ACE_PEER_ACCEPTOR
#define ACE_PEER_ACCEPTOR_2 _ACE_PEER_ACCEPTOR
#define ACE_PEER_ACCEPTOR _ACE_PEER_ACCEPTOR
#define ACE_PEER_ACCEPTOR_ADDR _ACE_PEER_ACCEPTOR::PEER_ADDR

// Handle ACE_SOCK_*
#define ACE_SOCK_ACCEPTOR ACE_SOCK_Acceptor
#define ACE_SOCK_CONNECTOR ACE_SOCK_Connector
#define ACE_SOCK_STREAM ACE_SOCK_Stream

// Handle ACE_TLI_*
#define ACE_TLI_ACCEPTOR ACE_TLI_Acceptor
#define ACE_TLI_CONNECTOR ACE_TLI_Connector
#define ACE_TLI_STREAM ACE_TLI_Stream

// Handle ACE_SPIPE_*
#define ACE_SPIPE_ACCEPTOR ACE_SPIPE_Acceptor
#define ACE_SPIPE_CONNECTOR ACE_SPIPE_Connector
#define ACE_SPIPE_STREAM ACE_SPIPE_Stream

// Handle ACE_UPIPE_*
#define ACE_UPIPE_ACCEPTOR ACE_UPIPE_Acceptor
#define ACE_UPIPE_CONNECTOR ACE_UPIPE_Connector
#define ACE_UPIPE_STREAM ACE_UPIPE_Stream

// Handle ACE_*_Memory_Pool.
#define ACE_MMAP_MEMORY_POOL ACE_MMAP_Memory_Pool
#define ACE_LITE_MMAP_MEMORY_POOL ACE_Lite_MMAP_Memory_Pool
#define ACE_SBRK_MEMORY_POOL ACE_Sbrk_Memory_Pool
#define ACE_SHARED_MEMORY_POOL ACE_Shared_Memory_Pool
#define ACE_LOCAL_MEMORY_POOL ACE_Local_Memory_Pool

#else /* TEMPLATES are broken */

// Handle ACE_Message_Queue.
#define ACE_SYNCH_1 class _ACE_SYNCH_MUTEX, class _ACE_SYNCH_CONDITION
#define ACE_SYNCH_2 _ACE_SYNCH_MUTEX, _ACE_SYNCH_CONDITION
#define ACE_SYNCH_MUTEX _ACE_SYNCH_MUTEX
#define ACE_SYNCH_CONDITION _ACE_SYNCH_CONDITION

// Handle ACE_Malloc*
#define ACE_MEM_POOL_1 class _ACE_MEM_POOL, class _ACE_MEM_POOL_OPTIONS
#define ACE_MEM_POOL_2 _ACE_MEM_POOL, _ACE_MEM_POOL_OPTIONS
#define ACE_MEM_POOL _ACE_MEM_POOL
#define ACE_MEM_POOL_OPTIONS _ACE_MEM_POOL_OPTIONS

// Handle ACE_Svc_Handler
#define ACE_PEER_STREAM_1 class _ACE_PEER_STREAM, class _ACE_PEER_ADDR
#define ACE_PEER_STREAM_2 _ACE_PEER_STREAM, _ACE_PEER_ADDR
#define ACE_PEER_STREAM _ACE_PEER_STREAM
#define ACE_PEER_STREAM_ADDR _ACE_PEER_ADDR

// Handle ACE_Acceptor
#define ACE_PEER_ACCEPTOR_1 class _ACE_PEER_ACCEPTOR, class _ACE_PEER_ADDR
#define ACE_PEER_ACCEPTOR_2 _ACE_PEER_ACCEPTOR, _ACE_PEER_ADDR
#define ACE_PEER_ACCEPTOR _ACE_PEER_ACCEPTOR
#define ACE_PEER_ACCEPTOR_ADDR _ACE_PEER_ADDR

// Handle ACE_SOCK_*
#define ACE_SOCK_ACCEPTOR ACE_SOCK_Acceptor, ACE_INET_Addr
#define ACE_SOCK_CONNECTOR ACE_SOCK_Connector, ACE_INET_Addr
#define ACE_SOCK_STREAM ACE_SOCK_Stream, ACE_INET_Addr

// Handle ACE_TLI_*
#define ACE_TLI_ACCEPTOR ACE_TLI_Acceptor, ACE_INET_Addr
#define ACE_TLI_CONNECTOR ACE_TLI_Connector, ACE_INET_Addr
#define ACE_TLI_STREAM ACE_TLI_Stream, ACE_INET_Addr

// Handle ACE_SPIPE_*
#define ACE_SPIPE_ACCEPTOR ACE_SPIPE_Acceptor, ACE_SPIPE_Addr
#define ACE_SPIPE_CONNECTOR ACE_SPIPE_Connector, ACE_SPIPE_Addr
#define ACE_SPIPE_STREAM ACE_SPIPE_Stream, ACE_SPIPE_Addr

// Handle ACE_UPIPE_*
#define ACE_UPIPE_ACCEPTOR ACE_UPIPE_Acceptor, ACE_SPIPE_Addr
#define ACE_UPIPE_CONNECTOR ACE_UPIPE_Connector, ACE_SPIPE_Addr
#define ACE_UPIPE_STREAM ACE_UPIPE_Stream, ACE_SPIPE_Addr

// Handle ACE_*_Memory_Pool.
#define ACE_MMAP_MEMORY_POOL ACE_MMAP_Memory_Pool, ACE_MMAP_Memory_Pool_Options
#define ACE_LITE_MMAP_MEMORY_POOL ACE_Lite_MMAP_Memory_Pool, ACE_MMAP_Memory_Pool_Options
#define ACE_SBRK_MEMORY_POOL ACE_Sbrk_Memory_Pool, ACE_Sbrk_Memory_Pool_Options
#define ACE_SHARED_MEMORY_POOL ACE_Shared_Memory_Pool, ACE_Shared_Memory_Pool_Options
#define ACE_LOCAL_MEMORY_POOL ACE_Local_Memory_Pool, ACE_Local_Memory_Pool_Options

#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

// For Win32 compatibility...
#if !defined (ACE_WSOCK_VERSION)
#define ACE_WSOCK_VERSION 0, 0
#endif /* ACE_WSOCK_VERSION */

#if defined (ACE_HAS_BROKEN_CTIME)
#undef ctime
#endif /* ACE_HAS_BROKEN_CTIME */

// Static service macros
#define ACE_STATIC_SVC_DECLARE(X) extern ACE_Static_Svc_Descriptor ace_svc_desc_##X ;
#define ACE_STATIC_SVC_DEFINE(X, NAME, TYPE, FN, FLAGS, ACTIVE) \
ACE_Static_Svc_Descriptor ace_svc_desc_##X = { NAME, TYPE, FN, FLAGS, ACTIVE };
#define ACE_STATIC_SVC_REQUIRE(X)\
class ACE_Static_Svc_##X {\
public:\
        ACE_Static_Svc_##X() { ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_##X); }\
};\
static ACE_Static_Svc_##X ace_static_svc_##X;

// Dynamic/static service macros.
#define ACE_SVC_FACTORY_DECLARE(X) extern "C" ACE_Svc_Export ACE_Service_Object *_make_##X (void);
#define ACE_SVC_INVOKE(X) _make_##X ()
#define ACE_SVC_NAME(X) _make_##X
#define ACE_SVC_FACTORY_DEFINE(X) extern "C" ACE_Service_Object *_make_##X () { ACE_TRACE (#X); return new X; }

#if !(defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE))
#define ACE_TSS_TYPE(T) T
#define ACE_TSS_GET(I, T) (I)
#else
#define ACE_TSS_TYPE(T) ACE_TSS< T >
#if defined (ACE_HAS_BROKEN_CONVERSIONS)
#define ACE_TSS_GET(I, T) (*(I))
#else
#define ACE_TSS_GET(I, T) ((I)->operator T * ())
#endif /* ACE_HAS_BROKEN_CONVERSIONS */
#endif /* !(defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)) */

#if defined (ACE_LACKS_MODE_MASKS)
// MODE MASKS

// the following macros are for POSIX conformance.

#define S_IRWXU 00700           // read, write, execute: owner.
#define S_IRUSR 00400           // read permission: owner.
#define S_IWUSR 00200           // write permission: owner.
#define S_IXUSR 00100           // execute permission: owner.
#define S_IRWXG 00070           // read, write, execute: group.
#define S_IRGRP 00040           // read permission: group.
#define S_IWGRP 00020           // write permission: group.
#define S_IXGRP 00010           // execute permission: group.
#define S_IRWXO 00007           // read, write, execute: other.
#define S_IROTH 00004           // read permission: other.
#define S_IWOTH 00002           // write permission: other.
#define S_IXOTH 00001           // execute permission: other.

#endif /* ACE_LACKS_MODE_MASKS */

#if defined (ACE_LACKS_SEMBUF_T)
struct sembuf 
{
  u_short sem_num; // semaphore # 
  short sem_op; // semaphore operation 
  short sem_flg; // operation flags 
};
#endif /* ACE_LACKS_SEMBUF_T */

#if defined (ACE_LACKS_UTSNAME_T)
#define _SYS_NMLN 257
struct utsname 
{
  char sysname[_SYS_NMLN];
  char nodename[_SYS_NMLN];
  char release[_SYS_NMLN];
  char version[_SYS_NMLN];
  char machine[_SYS_NMLN];
};
#endif /* ACE_LACKS_UTSNAME_T */

#if defined (ACE_HAS_H_ERRNO)
void herror (const char *str);
#endif /* ACE_HAS_H_ERRNO */

#if defined (ACE_LACKS_MSGBUF_T)
struct msgbuf {};
#endif /* ACE_LACKS_MSGBUF_T */

#if defined (ACE_LACKS_STRRECVFD)
struct strrecvfd {};
#endif /* ACE_LACKS_STRRECVFD */

#if defined (ACE_HAS_PROC_FS)
#include /**/ <sys/procfs.h>
#endif /* ACE_HAS_PROC_FD */

#if defined (ACE_HAS_UNICODE)
#include /**/ <wchar.h>
#endif /* ACE_HAS_UNICODE */

#if defined (ACE_HAS_BROKEN_WRITEV)
typedef struct iovec ACE_WRITEV_TYPE;
#else
typedef const struct iovec ACE_WRITEV_TYPE;
#endif /* ACE_HAS_BROKEN_WRITEV */
 
#if defined (ACE_HAS_BROKEN_SETRLIMIT)
typedef struct rlimit ACE_SETRLIMIT_TYPE;
#else
typedef const struct rlimit ACE_SETRLIMIT_TYPE;
#endif /* ACE_HAS_BROKEN_SETRLIMIT */

#if !defined (ACE_HAS_STRBUF_T)
struct strbuf
{
  int maxlen; // no. of bytes in buffer.
  int len;    // no. of bytes returned.
  void *buf;  // pointer to data.
};
#endif /* ACE_HAS_STRBUF_T */

struct ACE_Export ACE_Str_Buf
  // = TITLE
  //     Simple wrapper for STREAM pipes strbuf.
{
  int maxlen;                 
  // Number of bytes in buffer.

  int len;                    
  // Number of bytes returned.

  void *buf;              
  // Pointer to data.

  // = Initialization method
  ACE_Str_Buf (void *b = 0, int l = 0, int max = 0);
  // Constructor.
};

#if defined (ACE_MT_SAFE)
#define ACE_MT(X) X
#if !defined (_REENTRANT)
#define _REENTRANT
#endif /* _REENTRANT */
#else
#define ACE_MT(X) 
#endif /* ACE_MT_SAFE */

// These are the various states a thread managed by the
// <Thread_Manager> can be in.
enum ACE_Thread_State
{
  ACE_THR_IDLE, 
  // Uninitialized.

  ACE_THR_SPAWNED, 
  // Created but not yet running.

  ACE_THR_RUNNING,
  // Thread is active (naturally, we don't know if it's actually
  // *running* because we aren't the scheduler...).

  ACE_THR_SUSPENDED,
  // Thread is suspended.

  ACE_THR_CANCELLED,
  // Thread has been cancelled (which is an indiction that it needs to
  // terminate...).

  ACE_THR_TERMINATED
  // Thread has shutdown, but the slot in the thread manager hasn't
  // been reclaimed yet.
};

// Convenient macro for testing for deadlock, as well as for detecting
// when mutexes fail.
#define ACE_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN) \
  ACE_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) return RETURN;
#define ACE_WRITE_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN) \
  ACE_Write_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) return RETURN;
#define ACE_READ_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN) \
  ACE_Read_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) return RETURN;
#define ACE_GUARD(MUTEX,OBJ,LOCK) \
  ACE_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) return;

#if defined (ACE_HAS_POSIX_SEM)
#include /**/ <semaphore.h>
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
#endif /* ACE_HAS_POSIX_SEM */

struct cancel_state
{
  int cancelstate;
  // e.g., PTHREAD_CANCEL_ENABLE, PTHREAD_CANCEL_DISABLE,
  // PTHREAD_CANCELED.

  int canceltype;
  // e.g., PTHREAD_CANCEL_DEFERRED and PTHREAD_CANCEL_ASYNCHRONOUS.
};

#if defined (ACE_HAS_THREADS)

#if defined (ACE_HAS_STHREADS)
#include /**/ <synch.h>
#include /**/ <thread.h>
#endif /* ACE_HAS_STHREADS */

#if defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)
// Definitions for mapping POSIX pthreads onto Solaris threads.

#if defined (ACE_HAS_FSU_PTHREADS)
#define PTHREAD_DETACHED        0x1
#define PTHREAD_SCOPE_SYSTEM    0x2
#define PTHREAD_INHERIT_SCHED   0x4
#define PTHREAD_NOFLOAT         0x8
#define PTHREAD_CREATE_UNDETACHED       0
#define PTHREAD_CREATE_DETACHED         PTHREAD_DETACHED
#define PTHREAD_CREATE_JOINABLE         0
#define PTHREAD_SCOPE_PROCESS           0
#define PTHREAD_EXPLICIT_SCHED          0
#define PTHREAD_MIN_PRIORITY            0
#define PTHREAD_MAX_PRIORITY            126
#endif /* ACE_HAS_FSU_PTHREADS */

#if defined (ACE_HAS_SETKIND_NP)
#define PRIORITY_MAX                    PTHREAD_MAX_PRIORITY
#endif /* ACE_HAS_SETKIND_NP */

#if !defined (ACE_HAS_TID_T)
typedef pthread_t tid_t;
#endif /* ACE_HAS_TID_T */

// Typedefs to help compatibility with Windows NT and Pthreads.
#if defined (ACE_HAS_PTHREAD_T)
typedef pthread_t ACE_hthread_t;
#else /* ACE_HAS_PTHREAD_T */
typedef tid_t ACE_hthread_t;
#endif /* ACE_HAS_PTHREAD_T */

// Make it easier to write portable thread code.
typedef pthread_t ACE_thread_t;
typedef pthread_key_t ACE_thread_key_t;
typedef pthread_mutex_t ACE_mutex_t;
typedef pthread_cond_t ACE_cond_t;
typedef pthread_mutex_t ACE_thread_mutex_t;

#if !defined (PTHREAD_CANCEL_DISABLE)
#define PTHREAD_CANCEL_DISABLE      0
#endif /* PTHREAD_CANCEL_DISABLE */

#if !defined (PTHREAD_CANCEL_ENABLE)
#define PTHREAD_CANCEL_ENABLE       0
#endif /* PTHREAD_CANCEL_ENABLE */

#if !defined (PTHREAD_CANCEL_DEFERRED)
#define PTHREAD_CANCEL_DEFERRED     0
#endif /* PTHREAD_CANCEL_DEFERRED */

#if !defined (PTHREAD_CANCEL_ASYNCHRONOUS)
#define PTHREAD_CANCEL_ASYNCHRONOUS 0
#endif /* PTHREAD_CANCEL_ASYNCHRONOUS */

#define THR_CANCEL_DISABLE      PTHREAD_CANCEL_DISABLE
#define THR_CANCEL_ENABLE       PTHREAD_CANCEL_ENABLE
#define THR_CANCEL_DEFERRED     PTHREAD_CANCEL_DEFERRED
#define THR_CANCEL_ASYNCHRONOUS PTHREAD_CANCEL_ASYNCHRONOUS

#if !defined (PTHREAD_CREATE_JOINABLE)
#if defined (PTHREAD_CREATE_UNDETACHED)
#define PTHREAD_CREATE_JOINABLE PTHREAD_CREATE_UNDETACHED
#else
#define PTHREAD_CREATE_JOINABLE 0
#endif /* PTHREAD_CREATE_UNDETACHED */
#endif /* PTHREAD_CREATE_JOINABLE */

#if !defined (PTHREAD_CREATE_DETACHED)
#define PTHREAD_CREATE_DETACHED 1
#endif /* PTHREAD_CREATE_DETACHED */

#if !defined (PTHREAD_PROCESS_PRIVATE)
#if defined (PTHREAD_MUTEXTYPE_FAST)
#define PTHREAD_PROCESS_PRIVATE PTHREAD_MUTEXTYPE_FAST
#else
#define PTHREAD_PROCESS_PRIVATE 0
#endif /* PTHREAD_MUTEXTYPE_FAST */
#endif /* PTHREAD_PROCESS_PRIVATE */

#if !defined (PTHREAD_PROCESS_SHARED)
#if defined (PTHREAD_MUTEXTYPE_FAST)
#define PTHREAD_PROCESS_SHARED PTHREAD_MUTEXTYPE_FAST
#else
#define PTHREAD_PROCESS_SHARED 0
#endif /* PTHREAD_MUTEXTYPE_FAST */
#endif /* PTHREAD_PROCESS_SHARED */

#if defined (ACE_HAS_DCETHREADS)
#if defined (PTHREAD_PROCESS_PRIVATE)
#define USYNC_THREAD    PTHREAD_PROCESS_PRIVATE
#else
#define USYNC_THREAD    MUTEX_NONRECURSIVE_NP
#endif /* PTHREAD_PROCESS_PRIVATE */

#if defined (PTHREAD_PROCESS_SHARED)
#define USYNC_PROCESS   PTHREAD_PROCESS_SHARED
#else
#define USYNC_PROCESS   MUTEX_NONRECURSIVE_NP
#endif /* PTHREAD_PROCESS_SHARED */
#elif !defined (ACE_HAS_STHREADS)
#define USYNC_THREAD PTHREAD_PROCESS_PRIVATE
#define USYNC_PROCESS PTHREAD_PROCESS_SHARED
#endif /* ACE_HAS_DCETHREADS */

#define THR_BOUND               0x00000001
#define THR_NEW_LWP             0x00000002
#define THR_DETACHED            0x00000040
#define THR_SUSPENDED           0x00000080
#define THR_DAEMON              0x00000100
#define THR_JOINABLE            0x00010000
#define THR_SCHED_FIFO          0x00020000
#define THR_SCHED_RR            0x00040000
#define THR_SCHED_DEFAULT       0x00080000
#if defined (ACE_HAS_IRIX62_THREADS)
#define THR_SCOPE_SYSTEM        0x00100000
#else
#define THR_SCOPE_SYSTEM        THR_BOUND
#endif /* ACE_HAS_IRIX62_THREADS */
#define THR_SCOPE_PROCESS       0x00200000
#define THR_INHERIT_SCHED       0x00400000
#define THR_EXPLICIT_SCHED      0x00800000
#define THR_USE_AFX             0x01000000

#if !defined (ACE_HAS_STHREADS)
#if !defined (ACE_HAS_POSIX_SEM)
// This is used to implement semaphores for POSIX pthreads, but
// without POSIX semaphores.  It is different than the POSIX sem_t.
struct ACE_sema_t
{
  ACE_mutex_t lock_;
  // Serialize access to internal state.

  ACE_cond_t count_nonzero_;
  // Block until there are no waiters.

  u_long count_;
  // Number of waiters.
};
#endif /* !ACE_HAS_POSIX_SEM */

// This is used to implement readers/writer locks for POSIX pthreads.
struct ACE_rwlock_t
{
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
};
#else
// If we are on Solaris we can just reuse the existing implementations
// of these synchronization types.
typedef rwlock_t ACE_rwlock_t;
#if !defined (ACE_HAS_POSIX_SEM)
typedef sema_t ACE_sema_t;
#endif /* !ACE_HAS_POSIX_SEM */
#endif /* !ACE_HAS_STHREADS */
#elif defined (ACE_HAS_STHREADS)
// Typedefs to help compatibility with Windows NT and Pthreads.
typedef thread_t ACE_thread_t;
typedef thread_key_t ACE_thread_key_t;
typedef mutex_t ACE_mutex_t;
typedef rwlock_t ACE_rwlock_t;
#if !defined (ACE_HAS_POSIX_SEM)
typedef sema_t ACE_sema_t;
#endif /* !ACE_HAS_POSIX_SEM */
typedef cond_t ACE_cond_t;
typedef ACE_thread_t ACE_hthread_t;
typedef ACE_mutex_t ACE_thread_mutex_t;

#define THR_CANCEL_DISABLE      0
#define THR_CANCEL_ENABLE       0
#define THR_CANCEL_DEFERRED     0
#define THR_CANCEL_ASYNCHRONOUS 0
#elif defined (VXWORKS)
// For mutex implementation using mutual-exclusion semaphores (which
// can be taken recursively).
#include /**/ <semLib.h>    
#include /**/ <taskLib.h>

// task options:  the other options are either obsolete, internal, or for
// Fortran or Ada support
#define VX_UNBREAKABLE	  	0x0002	/* breakpoints ignored */
#define VX_FP_TASK	   	0x0008	/* floating point coprocessor */
#define VX_PRIVATE_ENV 		0x0080	/* private environment support */
#define VX_NO_STACK_FILL	0x0100	/* do not stack fill for checkstack () */

#define THR_CANCEL_DISABLE      0
#define THR_CANCEL_ENABLE       0
#define THR_CANCEL_DEFERRED     0
#define THR_CANCEL_ASYNCHRONOUS 0
#define THR_BOUND               0
#define THR_NEW_LWP             0
#define THR_DETACHED            0
#define THR_SUSPENDED           0
#define THR_DAEMON              0
#define THR_JOINABLE            0
#define THR_SCHED_FIFO          0
#define THR_SCHED_RR            0
#define THR_SCHED_DEFAULT       0
#define USYNC_THREAD            0
#define USYNC_PROCESS           1 // it's all global on VxWorks (without MMU
                                  // option)

typedef SEM_ID ACE_mutex_t;
// implement ACE_thread_mutex_t with ACE_mutex_t sinces there's just one process . . .
typedef ACE_mutex_t ACE_thread_mutex_t;
#if !defined (ACE_HAS_POSIX_SEM)
// although ACE_HAS_POSIX_SEM is assumed for VxWorks
typedef semaphore * ACE_sema_t;
#endif /* !ACE_HAS_POSIX_SEM */
typedef char * ACE_thread_t;
typedef int ACE_hthread_t;
typedef int ACE_thread_key_t;

struct ACE_cond_t
  // = TITLE
  //   This structure is used to implement condition variables on VxWorks
  //
  // = DESCRIPTION
  //   At the current time, this stuff only works for threads
  //   within the same process, which there's only one of on VxWorks.
{
  long waiters_;
  // Number of waiting threads.

  ACE_sema_t sema_;
  // Queue up threads waiting for the condition to become signaled.
};

struct ACE_rwlock_t
  // = TITLE 
  //   This is used to implement readers/writer locks on VxWorks
  //
  // = DESCRIPTION
  //   At the current time, this stuff only works for threads
  //   within the same process, which there's only one of on VxWorks.
{
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
};
#elif defined (ACE_HAS_WTHREADS)
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
typedef HANDLE ACE_sema_t;

struct ACE_cond_t
  // = TITLE
  //   This structure is used to implement condition variables on NT.
  //
  // = DESCRIPTION
  //   At the current time, this stuff only works for threads
  //   within the same process.
{
  DWORD waiters_;
  // Number of waiting threads.

  ACE_sema_t sema_;
  // Queue up threads waiting for the condition to become signaled.
};

struct ACE_rwlock_t
  // = TITLE 
  //   This is used to implement readers/writer locks on NT.
  //
  // = DESCRIPTION
  //   At the current time, this stuff only works for threads
  //   within the same process.
{
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
};

// These need to be different values, neither of which can be 0...
#define USYNC_THREAD 1
#define USYNC_PROCESS 2

#define THR_CANCEL_DISABLE      0
#define THR_CANCEL_ENABLE       0
#define THR_CANCEL_DEFERRED     0
#define THR_CANCEL_ASYNCHRONOUS 0
#define THR_DETACHED    0       // ?? ignore in most places 
#define THR_BOUND       0       // ?? ignore in most places
#define THR_NEW_LWP     0       // ?? ignore in most places
#define THR_SUSPENDED   CREATE_SUSPENDED
#endif /* ACE_HAS_DCETHREADS || ACE_HAS_PTHREADS */
#else /* !ACE_HAS_THREADS, i.e., the OS/platform doesn't support threading. */
// Give these things some reasonable value...
#define THR_CANCEL_DISABLE      0
#define THR_CANCEL_ENABLE       0
#define THR_CANCEL_DEFERRED     0
#define THR_CANCEL_ASYNCHRONOUS 0
#define THR_DETACHED    0       // ?? ignore in most places 
#define THR_BOUND       0       // ?? ignore in most places
#define THR_NEW_LWP     0       // ?? ignore in most places
#define THR_SUSPENDED   0       // ?? ignore in most places
#define USYNC_THREAD 0
#define USYNC_PROCESS 0
// These are dummies needed for class OS.h
typedef int ACE_cond_t;
typedef int ACE_mutex_t;
typedef int ACE_thread_mutex_t;
#if !defined (ACE_HAS_POSIX_SEM)
typedef int ACE_sema_t;
#endif /* !ACE_HAS_POSIX_SEM */
typedef int ACE_rwlock_t;
typedef int ACE_thread_t;
typedef int ACE_hthread_t;
typedef int ACE_thread_key_t;
#endif /* ACE_HAS_THREADS */

#include /**/ <sys/types.h>
#include /**/ <assert.h>
#include /**/ <sys/stat.h>
#include /**/ <limits.h>
#include /**/ <stdio.h>
#include "ace/stdcpp.h"
#include /**/ <new.h>
#include /**/ <ctype.h>
#include /**/ <signal.h>
#include /**/ <string.h>
#include /**/ <stdarg.h>
#include /**/ <fcntl.h>
#include /**/ <errno.h>
#include /**/ <stdlib.h>

// This must come after signal.h is #included.
#if defined (SCO)
#define SIGIO SIGPOLL
#undef sigemptyset
#undef sigfillset
#undef sigaddset
#undef sigdelset
#undef sigismember
#endif /* SCO */

#if defined (ACE_HAS_BROKEN_SENDMSG)
typedef struct msghdr ACE_SENDMSG_TYPE;
#else
typedef const struct msghdr ACE_SENDMSG_TYPE;
#endif /* ACE_HAS_BROKEN_SENDMSG */

#if defined (ACE_HAS_BROKEN_RANDR)
// The SunOS 5.x version of rand_r is inconsistent with the header files...
typedef unsigned int ACE_RANDR_TYPE;
extern "C" int rand_r (ACE_RANDR_TYPE seed);
#else
typedef unsigned int *ACE_RANDR_TYPE;
#endif /* ACE_HAS_BROKEN_RANDR */

#if !defined (ACE_HAS_RTLD_LAZY_V)
#define RTLD_LAZY 1
#endif /* !ACE_HAS_RTLD_LAZY_V */

#if defined (ACE_HAS_UTIME)
#include /**/ <utime.h>
#endif /* ACE_HAS_UTIME */

#if !defined (ACE_HAS_MSG) && !defined (SCO)
struct msghdr {};
#endif /* ACE_HAS_MSG */

#if !defined (ACE_HAS_HI_RES_TIMER)
typedef int hrtime_t;
#endif /* ACE_HAS_HI_RES_TIMER */

#if !defined (ACE_HAS_SIG_ATOMIC_T)
typedef int sig_atomic_t;
#endif /* !ACE_HAS_SIG_ATOMIC_T */

#if !defined (ACE_HAS_SSIZE_T)
typedef int ssize_t;
#endif /* ACE_HAS_SSIZE_T */

#if defined (ACE_HAS_OLD_MALLOC)
typedef char *ACE_MALLOC_T;
#else
typedef void *ACE_MALLOC_T;
#endif /* ACE_HAS_OLD_MALLOC */

#if defined (ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES)
// Prototypes for both signal() and struct sigaction are consistent..
#if defined (ACE_HAS_SIG_C_FUNC)
extern "C" {
#endif /* ACE_HAS_SIG_C_FUNC */
typedef void (*ACE_SignalHandler)(int);
typedef void (*ACE_SignalHandlerV)(int);
#if defined (ACE_HAS_SIG_C_FUNC)
}
#endif /* ACE_HAS_SIG_C_FUNC */
#elif defined (ACE_HAS_IRIX_53_SIGNALS)
typedef void (*ACE_SignalHandler)(...);
typedef void (*ACE_SignalHandlerV)(...);
#elif defined (ACE_HAS_SPARCWORKS_401_SIGNALS)
typedef void (*ACE_SignalHandler)(int, ...);
typedef void (*ACE_SignalHandlerV)(int,...);
#elif defined (ACE_HAS_SUNOS4_SIGNAL_T)
typedef void (*ACE_SignalHandler)(...);
typedef void (*ACE_SignalHandlerV)(...);
#elif defined (ACE_HAS_SVR4_SIGNAL_T)
// SVR4 Signals are inconsistent (e.g., see struct sigaction)..
typedef void (*ACE_SignalHandler)(int);
#if !defined (m88k)     //  with SVR4_SIGNAL_T
typedef void (*ACE_SignalHandlerV)(void);
#else
typedef void (*ACE_SignalHandlerV)(int);
#endif  //  m88k        //  with SVR4_SIGNAL_T
#elif defined (ACE_WIN32)
typedef void (__cdecl *ACE_SignalHandler)(int); 
typedef void (__cdecl *ACE_SignalHandlerV)(int);
#elif defined (ACE_HAS_UNIXWARE_SVR4_SIGNAL_T)
typedef void (*ACE_SignalHandler)(int);
typedef void (*ACE_SignalHandlerV)(...);
#else /* This is necessary for some older broken version of cfront */
#define ACE_SignalHandler SIG_PF
typedef void (*ACE_SignalHandlerV)(...);
#endif /* ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES */

#if defined (ACE_WIN32)
// Turn off warnings for /W4
// To resume any of these warning: #pragma warning(default: 4xxx)
// which should be placed after these defines
#ifndef ALL_WARNINGS
// #pragma warning(disable: 4101)  // unreferenced local variable
#pragma warning(disable: 4127)  // constant expression for TRACE/ASSERT
#pragma warning(disable: 4134)  // message map member fxn casts
#pragma warning(disable: 4511)  // private copy constructors are good to have
#pragma warning(disable: 4512)  // private operator= are good to have
#pragma warning(disable: 4514)  // unreferenced inlines are common
#pragma warning(disable: 4710)  // private constructors are disallowed
#pragma warning(disable: 4705)  // statement has no effect in optimized code
// #pragma warning(disable: 4701)  // local variable *may* be used without init
// #pragma warning(disable: 4702)  // unreachable code caused by optimizations
#pragma warning(disable: 4791)  // loss of debugging info in retail version
// #pragma warning(disable: 4204)  // non-constant aggregate initializer
#pragma warning(disable: 4275)  // deriving exported class from non-exported
#pragma warning(disable: 4251)  // using non-exported as public in exported
#pragma warning(disable: 4786)  // identifier was truncated to '255' characters in the browser information
#pragma warning(disable: 4097)  // typedef-name used as synonym for class-name
#endif //!ALL_WARNINGS
// We're on WinNT or Win95
#define ACE_PLATFORM "Win32"
#define ACE_PLATFORM_EXE_SUFFIX ".exe"

// STRICT type checking in WINDOWS.H enhances type safety for Windows
// programs by using distinct types to represent all the different
// HANDLES in Windows. So for example, STRICT prevents you from
// mistakenly passing an HPEN to a routine expecting an HBITMAP.
// Note that we only use this if we 
#if defined (ACE_HAS_STRICT)
#if !defined (STRICT)	// may already be defined
#define STRICT
#endif /* !STRICT */
#endif /* ACE_HAS_STRICT */

#include /**/ <sys/timeb.h>

// The following 3 defines are used by the ACE Name Server...
#define ACE_DEFAULT_NAMESPACE_DIR_W L"C:\\temp"
#define ACE_DEFAULT_NAMESPACE_DIR_A "C:\\temp"
#define ACE_DEFAULT_LOCALNAME "\\localnames"
#define ACE_DEFAULT_GLOBALNAME "\\globalnames"

// Used for ACE_MMAP_Memory_Pool
#define ACE_DEFAULT_BACKING_STORE __TEXT ("C:\\temp\\ace-malloc-XXXXXX")

// Used for logging
#define ACE_DEFAULT_LOGFILE "C:\\temp\\logfile"

// Used for dynamic linking
#define ACE_DEFAULT_SVC_CONF ".\\svc.conf"

// The following are #defines and #includes that are specific to
// WIN32.

#define ACE_STDIN GetStdHandle (STD_INPUT_HANDLE)
#define ACE_STDOUT GetStdHandle (STD_OUTPUT_HANDLE)
#define ACE_STDERR GetStdHandle (STD_ERROR_HANDLE)

// Default semaphore key and mutex name
#define ACE_DEFAULT_SEM_KEY "ACE_SEM_KEY"
#define ACE_INVALID_SEM_KEY 0

#define ACE_SEH_TRY __try
#define ACE_SEH_EXCEPT(X) __except(X)

// The "null" device on Win32.
#define ACE_DEV_NULL "nul"

// Define the pathname separator characters for Win32 (ugh).
#define ACE_DIRECTORY_SEPARATOR_STR_A "\\"
#define ACE_DIRECTORY_SEPARATOR_STR_W L"\\"
#define ACE_DIRECTORY_SEPARATOR_CHAR_A '\\'
#define ACE_DIRECTORY_SEPARATOR_CHAR_W L'\\'
#define ACE_LD_SEARCH_PATH "PATH"
#define ACE_LD_SEARCH_PATH_SEPARATOR_STR ";"
#define ACE_LOGGER_KEY __TEXT ("\\temp\\server_daemon")
#define ACE_DLL_SUFFIX ".dll"
#define ACE_DLL_PREFIX ""

// This will help until we figure out everything:
#define NFDBITS 32 // only used in unused functions...
// These two may be used for internal flags soon:
#define MAP_PRIVATE 1
#define MAP_SHARED  2
#define MAP_FIXED   4

#define RUSAGE_SELF 1

struct shmaddr { };
struct shmid_ds { };
struct msqid_ds {};

// Fake the UNIX rusage structure.  Perhaps we can add more to this
// later on?
struct rusage
{
  FILETIME ru_utime;
  FILETIME ru_stime;
};

// Win32 doesn't use numeric values to name its semaphores, it uses
// strings!
typedef char *key_t;

// MMAP flags
#define PROT_READ PAGE_READONLY
#define PROT_WRITE PAGE_READWRITE
#define PROT_RDWR PAGE_READWRITE
/* If we can find suitable use for these flags, here they are:
PAGE_WRITECOPY
PAGE_EXECUTE
PAGE_EXECUTE_READ
PAGE_EXECUTE_READWRITE
PAGE_EXECUTE_WRITECOPY
PAGE_GUARD
PAGE_NOACCESS
PAGE_NOCACHE  */

#if defined (ACE_HAS_WINSOCK2)
#include "ace/ws2tcpip.h"
#endif /* ACE_HAS_WINSOCK2 */

#define MAXHOSTNAMELEN  256

// error code mapping
#define ETIME                   ERROR_SEM_TIMEOUT
#define EWOULDBLOCK             WSAEWOULDBLOCK
#define EINPROGRESS             WSAEINPROGRESS
#define EALREADY                WSAEALREADY
#define ENOTSOCK                WSAENOTSOCK
#define EDESTADDRREQ            WSAEDESTADDRREQ
#define EMSGSIZE                WSAEMSGSIZE
#define EPROTOTYPE              WSAEPROTOTYPE
#define ENOPROTOOPT             WSAENOPROTOOPT
#define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
#define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
#define EOPNOTSUPP              WSAEOPNOTSUPP
#define EPFNOSUPPORT            WSAEPFNOSUPPORT
#define EAFNOSUPPORT            WSAEAFNOSUPPORT
#define EADDRINUSE              WSAEADDRINUSE
#define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
#define ENETDOWN                WSAENETDOWN
#define ENETUNREACH             WSAENETUNREACH
#define ENETRESET               WSAENETRESET
#define ECONNABORTED            WSAECONNABORTED
#define ECONNRESET              WSAECONNRESET
#define ENOBUFS                 WSAENOBUFS
#define EISCONN                 WSAEISCONN
#define ENOTCONN                WSAENOTCONN
#define ESHUTDOWN               WSAESHUTDOWN
#define ETOOMANYREFS            WSAETOOMANYREFS
#define ETIMEDOUT               WSAETIMEDOUT
#define ECONNREFUSED            WSAECONNREFUSED
#define ELOOP                   WSAELOOP
#define EHOSTDOWN               WSAEHOSTDOWN
#define EHOSTUNREACH            WSAEHOSTUNREACH
#define EPROCLIM                WSAEPROCLIM
#define EUSERS                  WSAEUSERS
#define EDQUOT                  WSAEDQUOT
#define ESTALE                  WSAESTALE
#define EREMOTE                 WSAEREMOTE
// Grrr! These two are already defined by the horrible 'standard'
// library.  
// #define ENAMETOOLONG            WSAENAMETOOLONG 
// #define ENOTEMPTY               WSAENOTEMPTY

#include /**/ <time.h>
#include /**/ <direct.h>
#include /**/ <process.h>
#include /**/ <io.h>

typedef OVERLAPPED ACE_OVERLAPPED;
typedef DWORD ACE_thread_t;
typedef HANDLE ACE_hthread_t;
typedef long pid_t;
typedef DWORD ACE_thread_key_t;
typedef DWORD nlink_t;

// 64-bit quad-word definitions
#if !defined (_MSC_VER) /* Borland? */
typedef uint64 ACE_QWORD;
inline ACE_QWORD ACE_MAKE_QWORD (DWORD lo, DWORD hi) { return unit64 (lo, hi); }
inline DWORD ACE_LOW_DWORD  (ACE_QWORD q) { return q.LowPart; }
inline DWORD ACE_HIGH_DWORD (ACE_QWORD q) { return q.HighPart; }
#else
typedef unsigned __int64  ACE_QWORD;
inline ACE_QWORD ACE_MAKE_QWORD (DWORD lo, DWORD hi) { return ACE_QWORD (lo) | (ACE_QWORD (hi) << 32); }
inline DWORD ACE_LOW_DWORD  (ACE_QWORD q) { return (DWORD) q; }
inline DWORD ACE_HIGH_DWORD (ACE_QWORD q) { return (DWORD) (q >> 32); }
#endif /* !defined (_MSC_VER) */

// Win32 dummies to help compilation.

typedef void *sigset_t;    // Who knows?
typedef int mode_t;
typedef int uid_t;
typedef int gid_t;
typedef int hrtime_t;
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
struct flock { }; // not used with Win32 locking...

// Wrapper for NT Events.
typedef HANDLE ACE_event_t;

// This is for file descriptors.
typedef HANDLE ACE_HANDLE;

// For Win32 compatibility. 
typedef SOCKET ACE_SOCKET;

#define ACE_INVALID_HANDLE INVALID_HANDLE_VALUE
#define ACE_SYSCALL_FAILED 0xFFFFFFFF

// Needed to map calls to NT transparently.
#define MS_ASYNC 0
#define MS_INVALIDATE 0

// Reliance on CRT - I don't really like this.

#define O_NDELAY    0
#define MAXPATHLEN  _MAX_PATH
#define MAXNAMLEN   _MAX_FNAME
#define EADDRINUSE WSAEADDRINUSE

// Undefined structs becomes undeclared overloads with MSVC++ 2.0
// Thus we need to resort to this for unsupported system calls.

struct sigaction 
{
  int sa_flags;
  ACE_SignalHandlerV sa_handler;
  sigset_t sa_mask;
};

struct iovec
{
  char *iov_base; // data to be read/written
  size_t iov_len; // byte count to read/write
};

#else /* !defined (ACE_WIN32) */

// We're some kind of UNIX...
#define ACE_PLATFORM "UNIX"
#define ACE_PLATFORM_EXE_SUFFIX ""

typedef const char *LPCTSTR;
typedef char *LPTSTR;
typedef char TCHAR;

#if (defined (ACE_HAS_UNICODE) && (defined (UNICODE)))
#define __TEXT(STRING) L##STRING
#else
#define __TEXT(STRING) STRING 
#endif /* UNICODE && ACE_HAS_UNICODE */

#if defined (m88k)
#define RUSAGE_SELF 1
#endif  //  m88k

// Default semaphore key
#define ACE_DEFAULT_SEM_KEY 1234
#define ACE_INVALID_SEM_KEY -1

// Define the pathname separator characters for UNIX.
#define ACE_DIRECTORY_SEPARATOR_STR_A "/"
#define ACE_DIRECTORY_SEPARATOR_CHAR_A '/'
#if defined (ACE_HAS_UNICODE)
#define ACE_DIRECTORY_SEPARATOR_STR_W L"/"
#define ACE_DIRECTORY_SEPARATOR_CHAR_W L'/'
#else
#define ACE_DIRECTORY_SEPARATOR_STR_W "/"
#define ACE_DIRECTORY_SEPARATOR_CHAR_W '/'
#endif /* ACE_HAS_UNICODE */
#define ACE_LD_SEARCH_PATH "LD_LIBRARY_PATH"
#define ACE_LD_SEARCH_PATH_SEPARATOR_STR ":"
#define ACE_LOGGER_KEY "/tmp/server_daemon"
#define ACE_DLL_SUFFIX ".so"
#define ACE_DLL_PREFIX "lib"

// The following 3 defines are used by the ACE Name Server...
#define ACE_DEFAULT_NAMESPACE_DIR_A "/tmp"
#if defined (ACE_HAS_UNICODE)
#define ACE_DEFAULT_NAMESPACE_DIR_W L"/tmp"
#else
#define ACE_DEFAULT_NAMESPACE_DIR_W "/tmp"
#endif /* ACE_HAS_UNICODE */
#define ACE_DEFAULT_LOCALNAME "/localnames"
#define ACE_DEFAULT_GLOBALNAME "/globalnames"

// Used for ACE_MMAP_Memory_Pool
#define ACE_DEFAULT_BACKING_STORE "/tmp/ace-malloc-XXXXXX" 

// Used for logging
#define ACE_DEFAULT_LOGFILE "/tmp/logfile"

// Used for dynamic linking.
#define ACE_DEFAULT_SVC_CONF "./svc.conf"

// The following are #defines and #includes that are specific to UNIX.

#define ACE_STDIN 0
#define ACE_STDOUT 1
#define ACE_STDERR 2

// Be consistent with Winsock naming
#define ACE_INVALID_HANDLE -1
#define ACE_SYSCALL_FAILED -1

#define ACE_SEH_TRY
#define ACE_SEH_EXCEPT(X)

// The "null" device on UNIX.
#define ACE_DEV_NULL "/dev/null"

// Wrapper for NT events on UNIX.
struct ACE_event_t
{  
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
typedef int LPSECURITY_ATTRIBUTES;
#define GENERIC_READ 0
#define FILE_SHARE_READ 0
#define OPEN_EXISTING 0
#define FILE_ATTRIBUTE_NORMAL 0
#define MAXIMUM_WAIT_OBJECTS 0
#define FILE_FLAG_OVERLAPPED 0

#if defined (ACE_HAS_BROKEN_IF_HEADER)
struct ifafilt;
#endif
#include /**/ <sys/socket.h>
extern "C" {
#if !defined (VXWORKS)
#include /**/ <netdb.h>
#endif /* VXWORKS */
#include /**/ <net/if.h>
#include /**/ <netinet/in.h>
#include /**/ <arpa/inet.h>
}
#if defined (VXWORKS)
#include /**/ <sys/times.h>
#else
#include /**/ <sys/uio.h>
#include /**/ <sys/ipc.h>
#include /**/ <sys/shm.h>
#include /**/ <sys/sem.h>
#include /**/ <sys/file.h>
#include /**/ <sys/time.h>
#include /**/ <sys/resource.h>
#include /**/ <sys/wait.h>
#include /**/ <pwd.h>
#endif /* VXWORKS */
#include /**/ <sys/ioctl.h>
#include /**/ <dirent.h>
#include /**/ <sys/utsname.h>
#include /**/ <sys/stat.h>
#include /**/ <unistd.h>

#if !defined (ACE_LACKS_PARAM_H)
#include /**/ <sys/param.h>
#endif /* ACE_LACKS_PARAM_H */

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS) && !defined (VXWORKS)
#include /**/ <sys/un.h>
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#if defined (ACE_HAS_SIGINFO_T)
#if !defined (ACE_LACKS_SIGINFO_H)
#include /**/ <siginfo.h>
#endif /* ACE_LACKS_SIGINFO_H */
#if !defined (ACE_LACKS_UCONTEXT_H)
#include /**/ <ucontext.h>
#endif /* ACE_LACKS_UCONTEXT_H */
#endif /* ACE_HAS_SIGINFO_T */

#if defined (ACE_HAS_POLL)
#include /**/ <poll.h>
#endif /* ACE_HAS_POLL */

#if defined (ACE_HAS_STREAMS)
#if defined (AIX)
#if !defined (_XOPEN_EXTENDED_SOURCE)
#define _XOPEN_EXTENDED_SOURCE
#endif /* !_XOPEN_EXTENDED_SOURCE */
#include /**/ <stropts.h>
#undef _XOPEN_EXTENDED_SOURCE
#else
#include /**/ <stropts.h>
#endif /* AIX */
#endif /* ACE_HAS_STREAMS */

#if defined (ACE_LACKS_T_ERRNO)
extern int t_errno;
#endif /* ACE_LACKS_T_ERRNO */

#if !defined (ACE_HAS_SIGWAIT)
extern "C" int sigwait (sigset_t *set);
#endif /* ACE_HAS_SIGWAIT */

#if defined (ACE_HAS_SELECT_H)
#include /**/ <sys/select.h>
#endif /* ACE_HAS_SELECT_H */

#if defined (ACE_HAS_ALLOCA_H)
#include /**/ <alloca.h>
#endif /* ACE_HAS_ALLOCA_H */

#if defined (ACE_HAS_TIUSER_H)
#include /**/ <tiuser.h> 
#endif /* ACE_HAS_TIUSER_H */

#if defined (ACE_HAS_SVR4_DYNAMIC_LINKING)
#include /**/ <dlfcn.h>
#endif /* ACE_HAS_SVR4_DYNAMIC_LINKING */

#if defined (ACE_HAS_SOCKIO_H)
#include /**/ <sys/sockio.h>
#endif /* ACE_HAS_SOCKIO_ */

// There must be a better way to do this...
#if !defined (RLIMIT_NOFILE)
#if defined (Linux) || defined (AIX) || defined (SCO)
#if defined (RLIMIT_OFILE)
#define RLIMIT_NOFILE RLIMIT_OFILE
#else
#define RLIMIT_NOFILE 200
#endif /* RLIMIT_OFILE */
#endif /* defined (Linux) || defined (AIX) || defined (SCO) */
#endif /* RLIMIT_NOFILE */

#if !defined (ACE_HAS_TLI_PROTOTYPES)
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
  int t_rcv(int fildes, char *buf, unsigned nbytes, int *flags);
  int t_rcvconnect(int fildes, struct t_call *call);
  int t_rcvdis(int fildes, struct t_discon *discon);
  int t_rcvrel(int fildes);
  int t_rcvudata(int fildes, struct t_unitdata *unitdata, int *flags);
  int t_rcvuderr(int fildes, struct t_uderr *uderr);
  int t_snd(int fildes, char *buf, unsigned nbytes, int flags);
  int t_snddis(int fildes, struct t_call *call);
  int t_sndrel(int fildes);
  int t_sndudata(int fildes, struct t_unitdata *unitdata);
  int t_sync(int fildes);
  int t_unbind(int fildes);
}
#endif /* !ACE_HAS_TLI_PROTOTYPES */

// IRIX5 defines bzero() in this odd file...
#if defined (ACE_HAS_BSTRING)
#include /**/ <bstring.h>
#endif /* ACE_HAS_BSTRING */

// AIX defines bzero() in this odd file...
#if defined (ACE_HAS_STRINGS)
#include /**/ <strings.h>
#endif /* ACE_HAS_STRINGS */

#if defined (ACE_HAS_TERM_IOCTLS)
#include /**/ <sys/termios.h>
#endif /* ACE_HAS_TERM_IOCTLS */

#if defined (ACE_LACKS_MMAP)
#define PROT_READ 0
#define PROT_WRITE 0
#define PROT_EXEC 0
#define PROT_NONE 0
#define PROT_RDWR 0
#define MAP_PRIVATE 0
#define MAP_SHARED 0
#define MAP_FIXED 0
#endif /* ACE_LACKS_MMAP */

// Fixes a problem with HP/UX.
#if defined (ACE_HAS_BROKEN_MMAP_H)
extern "C" 
{
#include /**/ <sys/mman.h>
}
#elif !defined (ACE_LACKS_MMAP)
#include /**/ <sys/mman.h>
#endif /* ACE_HAS_BROKEN_MMAP_H */

// OSF1 has problems with sys/msg.h and C++...
#if defined (ACE_HAS_BROKEN_MSG_H)
#define _KERNEL
#endif /* ACE_HAS_BROKEN_MSG_H */
#if !defined (VXWORKS)
#include /**/ <sys/msg.h>
#endif /* VXWORKS */
#if defined (ACE_HAS_BROKEN_MSG_H)
#undef _KERNEL
#endif /* ACE_HAS_BROKEN_MSG_H */

#if defined (ACE_LACKS_SYSV_MSQ_PROTOS)
extern "C"
{
  int msgget (key_t, int);
  int msgrcv (int, void *, size_t, long, int);
  int msgsnd (int, const void *, size_t, int);
  int msgctl (int, int, struct msqid_ds *);
}
#endif /* ACE_LACKS_SYSV_MSQ_PROTOS */
#endif /* ACE_WIN32 */

#if defined (VXWORKS)

#if defined (ACE_HAS_GREENHILLS_SOCKETS)  
#include /**/ <hostLib.h>
#include /**/ <ioLib.h>
#include /**/ <selectLib.h>
#include /**/ <sigLib.h>
#include /**/ <sockLib.h>

extern "C" 
struct sockaddr_un {
  short sun_family;    // AF_UNIX.
  char  sun_path[108]; // path name.
};
#endif /* ACE_HAS_GREENHILLS_SOCKETS */

#define MAXPATHLEN  1024
#define MAXNAMLEN   255
#define NSIG _NSIGS
#endif /* VXWORKS */

#if defined (ACE_SELECT_USES_INT)
typedef int ACE_FD_SET_TYPE;
#else
typedef fd_set ACE_FD_SET_TYPE;
#endif /* ACE_SELECT_USES_INT */

#if !defined (MAXNAMELEN)
#if defined (FILENAME_MAX)
#define MAXNAMELEN FILENAME_MAX
#else
#define MAXNAMELEN 256
#endif /* FILENAME_MAX */
#endif /* MAXNAMELEN */

// This one exists only to please Service_Config.h and
// Service_Manager.cpp
#if !defined (SIGHUP)
#define SIGHUP 1
#endif /* SIGHUP */

#if !defined (SIGQUIT)
#define SIGQUIT 3
#endif /* SIGQUIT */

#if !defined (SIGPIPE)
#define SIGPIPE 13
#endif /* SIGPIPE      */

#if !defined (O_NONBLOCK)
#define O_NONBLOCK  1
#endif /* O_NONBLOCK  */

#if !defined (SIG_BLOCK)
#define SIG_BLOCK   1
#endif /* SIG_BLOCK   */

#if !defined (SIG_UNBLOCK)
#define SIG_UNBLOCK 2
#endif /* SIG_UNBLOCK */

#if !defined (SIG_SETMASK)
#define SIG_SETMASK 3
#endif /* SIG_SETMASK */

#if !defined (IPC_CREAT)
#define IPC_CREAT 0
#endif /* IPC_CREAT */

#if !defined (IPC_NOWAIT)
#define IPC_NOWAIT 0
#endif /* IPC_NOWAIT */

#if !defined (IPC_RMID)
#define IPC_RMID 0
#endif /* IPC_RMID */

#if !defined (IPC_EXCL)
#define IPC_EXCL 0
#endif /* IPC_EXCL */

#if !defined (IPC_PRIVATE)
#define IPC_PRIVATE ACE_INVALID_SEM_KEY 
#endif /* IPC_PRIVATE */

#if !defined (IPC_STAT)
#define IPC_STAT 0
#endif /* IPC_STAT */

#if !defined (GETVAL)
#define GETVAL 0
#endif /* GETVAL */

#if !defined (F_GETFL)
#define F_GETFL 0
#endif /* F_GETFL */

#if !defined (SETVAL)
#define SETVAL 0
#endif /* SETVAL */

#if !defined (GETALL)
#define GETALL 0
#endif /* GETALL */

#if !defined (SETALL)
#define SETALL 0
#endif /* SETALL */

#if !defined (SEM_UNDO)
#define SEM_UNDO 0
#endif /* SEM_UNDO */

// Why is this defined?  It must be a std C library symbol.
#if !defined (NSIG)
#define NSIG 1
#endif /* NSIG */

#if !defined (R_OK)
#define R_OK    04      // Test for Read permission.
#endif /* R_OK */

#if !defined (W_OK)
#define W_OK    02      // Test for Write permission.
#endif /* W_OK */

#if !defined (X_OK)
#define X_OK    01      // Test for eXecute permission.
#endif /* X_OK */

#if !defined (F_OK)
#define F_OK    0       // Test for existence of File.
#endif /* F_OK */

#if !defined (EIDRM)
#define EIDRM 0
#endif /* !EIDRM */

#if !defined (ENOTSUP)
#define ENOTSUP ENOSYS  // Operation not supported      .
#endif /* !ENOTSUP */

#if !defined (EDEADLK)
#define EDEADLK 1000 // Some large number....
#endif /* !ENOTSUP */

#if !defined (MS_SYNC)
#define MS_SYNC 0x0
#endif /* !MS_SYNC */

#if !defined (PIPE_BUF)
#define PIPE_BUF 5120
#endif /* PIPE_BUF */

#if !defined (PROT_RDWR)
#define PROT_RDWR (PROT_READ|PROT_WRITE)
#endif /* PROT_RDWR */

#if defined (ACE_HAS_POSIX_NONBLOCK)
#define ACE_NONBLOCK O_NONBLOCK
#else
#define ACE_NONBLOCK O_NDELAY
#endif /* ACE_HAS_POSIX_NONBLOCK */

#define LOCALNAME 0
#define REMOTENAME 1

#if defined (ACE_HAS_64BIT_LONGS)
// Necessary to support the Alphas, which have 64 bit longs and 32 bit
// ints...
typedef u_int ACE_UINT32;
#else
typedef u_long ACE_UINT32;
#endif /* ACE_HAS_64BIT_LONGS */

#if !defined (ETIMEDOUT) && defined (ETIME)
#define ETIMEDOUT ETIME
#endif /* ETIMEDOUT */

#if !defined (ETIME) && defined (ETIMEDOUT)
#define ETIME ETIMEDOUT
#endif /* ETIMED */

// Note that this assumes shorts are 16 bits.
typedef u_short ACE_USHORT16;

#if defined (ACE_HAS_STRUCT_NETDB_DATA)
typedef char ACE_HOSTENT_DATA[sizeof(struct hostent_data)];
typedef char ACE_SERVENT_DATA[sizeof(struct servent_data)];
typedef char ACE_PROTOENT_DATA[sizeof(struct protoent_data)];
#else
#if !defined ACE_HOSTENT_DATA_SIZE
#define ACE_HOSTENT_DATA_SIZE (4*1024)
#endif /*ACE_HOSTENT_DATA_SIZE */
#if !defined ACE_SERVENT_DATA_SIZE
#define ACE_SERVENT_DATA_SIZE (4*1024)
#endif /*ACE_SERVENT_DATA_SIZE */
#if !defined ACE_PROTOENT_DATA_SIZE
#define ACE_PROTOENT_DATA_SIZE (2*1024)
#endif /*ACE_PROTOENT_DATA_SIZE */
typedef char ACE_HOSTENT_DATA[ACE_HOSTENT_DATA_SIZE];
typedef char ACE_SERVENT_DATA[ACE_SERVENT_DATA_SIZE];
typedef char ACE_PROTOENT_DATA[ACE_PROTOENT_DATA_SIZE];
#endif /* ACE_HAS_STRUCT_NETDB_DATA */

#if !defined (ACE_HAS_SEMUN)
union semun 
{
  int val; // value for SETVAL 
  struct semid_ds *buf; // buffer for IPC_STAT & IPC_SET 
  u_short *array; // array for GETALL & SETALL 
};
#endif /* !ACE_HAS_SEMUN */

// Max size of an ACE Token.
#define ACE_MAXTOKENNAMELEN 40

// Max size of an ACE Token client ID.
#define ACE_MAXCLIENTIDLEN MAXHOSTNAMELEN + 20

// Create some useful typedefs.
typedef const char **SYS_SIGLIST;
// This is for C++ static methods.
#if defined (VXWORKS)
typedef FUNCPTR ACE_THR_FUNC;  // where typedef int (*FUNCPTR) (...)
#else
typedef void *(*ACE_THR_FUNC)(void *);
#endif /* VXWORKS */

#if defined (ACE_HAS_THR_C_DEST)
// Needed for frigging MVS C++...
extern "C" {
typedef void (*ACE_THR_DEST)(void *);
}
#else
typedef void (*ACE_THR_DEST)(void *);
#endif /* ACE_HAS_THR_C_DEST */

extern "C"
{
#if defined (ACE_WIN32)
typedef unsigned (__stdcall *ACE_THR_C_FUNC) (void*);
#elif defined (VXWORKS)
typedef FUNCPTR ACE_THR_C_FUNC;  // where typedef int (*FUNCPTR) (...)
#else
typedef void *(*ACE_THR_C_FUNC)(void *);
#endif /* ACE_WIN32 */
}

#if !defined (MAP_FAILED)
#define MAP_FAILED ((void *) -1)
#elif defined (ACE_HAS_LONG_MAP_FAILED)
#undef MAP_FAILED
#define MAP_FAILED ((void *) -1L)
#endif /* MAP_FAILED */

#if defined (ACE_HAS_CHARPTR_DL)
typedef char * ACE_DL_TYPE;
#if defined (ACE_HAS_UNICODE)
typedef wchar_t * ACE_WIDE_DL_TYPE;
#endif /* ACE_HAS_UNICODE */
#else
typedef const char * ACE_DL_TYPE;
#if defined (ACE_HAS_UNICODE)
typedef const wchar_t * ACE_WIDE_DL_TYPE;
#endif /* ACE_HAS_UNICODE */
#endif /* ACE_HAS_CHARPTR_DL */

#if !defined (ACE_HAS_SIGINFO_T)
struct ACE_Export siginfo_t
{
  siginfo_t (ACE_HANDLE handle);

  ACE_HANDLE si_handle_;
  // Win32 HANDLE that has become signaled.
};
#endif /* ACE_HAS_SIGINFO_T */

#if !defined (ACE_HAS_UCONTEXT_T)
typedef int ucontext_t;
#endif /* ACE_HAS_UCONTEXT_T */

#if !defined (SA_SIGINFO)
#define SA_SIGINFO 0
#endif /* SA_SIGINFO */

#if !defined (SA_RESTART)
#define SA_RESTART 0
#endif /* SA_RESTART */

#if defined (ACE_HAS_TIMOD_H)
#include /**/ <sys/timod.h>
#elif defined (ACE_HAS_OSF_TIMOD_H)
#include /**/ <tli/timod.h>
#endif /* ACE_HAS_TIMOD_H */

#if defined (ACE_HAS_BROKEN_T_ERRNO)
#undef t_errno
#endif /* ACE_HAS_BROKEN_T_ERRNO */

// Type of the extended signal handler.
typedef void (*ACE_Sig_Handler_Ex) (int, siginfo_t *siginfo, ucontext_t *ucontext);

// If the xti.h file redefines the function names, do it now, else
// when the function definitions are encountered, they won't match the
// declaration here.

#if defined (ACE_REDEFINES_XTI_FUNCTIONS)
#include /**/ <xti.h>
#ifdef UNIXWARE         // They apparantly forgot one...
extern "C" int _xti_error(char *);
#endif /* UNIXWARE */
#endif /* ACE_REDEFINES_XTI_FUNCTIONS */

class ACE_Export ACE_OS
  // = TITLE
  //     This class defines an operating system independent
  //     programming API that shields developers from non-portable
  //     aspects of writing efficient system programs on Win32, POSIX,
  //     and other versions of UNIX.
  //
  // = DESCRIPTION
  //     This class encapsulates all the differences between various
  //     versions of UNIX and WIN32!  The other components in
  //     ACE are programmed to use only the methods in this class,
  //     which makes it *much* easier to move ACE to a new platform.
  //     The methods in this class also automatically restart when 
  //     interrupts occur during system calls (assuming that the
  //     ACE_Log_Msg::restart() flag is enabled).
{
public:
  struct flock_t
    // = TITLE
    //     OS file locking structure.
  {
    void dump (void) const;
  // Dump state of the object.

#if defined (ACE_WIN32)
    ACE_OVERLAPPED overlapped_;
#else
    struct flock lock_;
#endif /* ACE_WIN32 */

    ACE_HANDLE handle_;
    // Handle to the underlying file.
  };

  // = A set of wrappers for miscellaneous operations.
  static int atoi (const char *s);
  static char *getenv (const char *symbol);
  static int getopt (int argc, char *const *argv, const char *optstring); 
  static long sysconf (int);

  // = A set of wrappers for condition variables.
  static int cond_broadcast (ACE_cond_t *cv);
  static int cond_destroy (ACE_cond_t *cv);
  static int cond_init (ACE_cond_t *cv, int type = USYNC_THREAD, LPCTSTR name = 0, void *arg = 0);
  static int cond_signal (ACE_cond_t *cv);
  static int cond_timedwait (ACE_cond_t *cv, ACE_mutex_t *m, ACE_Time_Value *);
  static int cond_wait (ACE_cond_t *cv, ACE_mutex_t *m);
#if defined (ACE_WIN32) && defined (ACE_HAS_WTHREADS)
  static int cond_timedwait (ACE_cond_t *cv, ACE_thread_mutex_t *m, ACE_Time_Value *);
  static int cond_wait (ACE_cond_t *cv, ACE_thread_mutex_t *m);
#endif /* ACE_WIN32 && ACE_HAS_WTHREADS */

  // = A set of wrappers for determining config info.
  static LPTSTR cuserid (LPTSTR user, size_t maxlen = 32);
  static int uname (struct utsname *name);
  static long sysinfo (int cmd, char *buf, long count);
  static int hostname (char *name, size_t maxnamelen);

  // = A set of wrappers for explicit dynamic linking.
  static int dlclose (void *handle);
  static char *dlerror (void);
  static void *dlopen (ACE_DL_TYPE filename, int mode);
  static void *dlsym (void *handle, ACE_DL_TYPE symbol);

  // = A set of wrappers for stdio file operations.
  static int last_error (void);
  static void last_error (int);
  static int fclose (FILE *fp);
  static int fcntl (ACE_HANDLE handle, int cmd, int val = 0);
  static int fdetach (const char *file);
  static FILE *fdopen (ACE_HANDLE handle, const char *mode);
  static char *fgets (char *buf, int size, FILE *fp);
  static int fflush (FILE *fp);
  static FILE *fopen (const char *filename, const char *mode);
  static int fprintf (FILE *fp, const char *format, ...);
  static size_t fread (void *ptr, size_t size, size_t nelems, FILE
                       *fp); 
  static int fstat (ACE_HANDLE, struct stat *);
  static int ftruncate (ACE_HANDLE, off_t);
  static size_t fwrite (const void *ptr, size_t size, size_t nitems,
                        FILE *fp); 
  static char *gets (char *str);
  static void perror (const char *s);
  static int printf (const char *format, ...);
  static int puts (const char *s);
  static void rewind (FILE *fp);
  static int sprintf (char *buf, const char *format, ...);

  // = A set of wrappers for file locks.
  static int flock_init (ACE_OS::flock_t *lock, int flags = 0, 
                         LPCTSTR name = 0, mode_t perms = 0);
  static int flock_destroy (ACE_OS::flock_t *lock);
  static int flock_rdlock (ACE_OS::flock_t *lock, short whence = 0, 
                           off_t start = 0, off_t len = 0);
  static int flock_tryrdlock (ACE_OS::flock_t *lock, short whence = 0, 
                              off_t start = 0, off_t len = 0);
  static int flock_trywrlock (ACE_OS::flock_t *lock, short whence = 0, 
                              off_t start = 0, off_t len = 0);
  static int flock_unlock (ACE_OS::flock_t *lock, short whence = 0, 
                           off_t start = 0, off_t len = 0);
  static int flock_wrlock (ACE_OS::flock_t *lock, short whence = 0, 
                           off_t start = 0, off_t len = 0);

  // = A set of wrappers for low-level process operations.
  static int execl (const char *path, const char *arg0, ...);
  static int execle (const char *path, const char *arg0, ...);
  static int execlp (const char *file, const char *arg0, ...);
  static int execv (const char *path, char *const argv[]);
  static int execvp (const char *file, char *const argv[]);
  static int execve (const char *path, char *const argv[], char *const envp[]);
  static void _exit (int status = 0);
  static void exit (int status = 0);
  static pid_t fork (void);
  static pid_t fork_exec (char *argv[]);
  // Forks and exec's a process in a manner that works on Solaris and
  // NT.  argv[0] must be the full path name to the executable.
  static gid_t getgid (void);
  static pid_t getpid (void);
  static uid_t getuid (void);
  static pid_t setsid (void);
  static int system (const char *s);
  static pid_t wait (int * = 0);
  static pid_t waitpid (pid_t, int * = 0, int = 0);

  // = A set of wrappers for timers and resource stats.
  static u_int alarm (u_int delay);
  static hrtime_t gethrtime (void);
  static ACE_Time_Value gettimeofday (void);
  static int getrusage (int who, struct rusage *rusage);
  static int getrlimit (int resource, struct rlimit *rl);
  static int setrlimit (int resource, ACE_SETRLIMIT_TYPE *rl);
  static int sleep (u_int seconds);
  static int sleep (const ACE_Time_Value &tv);

#if defined (ACE_HAS_BROKEN_R_ROUTINES)
#undef ctime_r
#undef asctime_r
#undef rand_r   
#endif /* ACE_HAS_BROKEN_R_ROUTINES */

  // = A set of wrappers for operations on time.
  static time_t time (time_t *tloc);
  static struct tm *localtime (const time_t *clock);
  static struct tm *localtime_r (const time_t *clock, struct tm *res);
  static char *asctime (const struct tm *tm);
  static char *ctime (const time_t *t);
  static char *ctime_r (const time_t *clock, char *buf, int buflen);
  static char *asctime_r (const struct tm *tm, char *buf, int buflen);

  // = A set of wrappers for memory managment.
  static void *sbrk (int brk);
  static void *malloc (size_t);
  static void *realloc (void *, size_t);
  static void free (void *);

  // = A set of wrappers for memory copying operations.
  static int memcmp (const void *s, const void *t, size_t len);
  static void *memcpy (void *s, const void *t, size_t len);
  static void *memset (void *s, int c, size_t len);

  // = A set of wrappers for System V message queues.
  static int msgctl (int msqid, int cmd, struct msqid_ds *);
  static int msgget (key_t key, int msgflg);
  static int msgrcv (int int_id, void *buf, size_t len, 
                     long type, int flags); 
  static int msgsnd (int int_id, const void *buf, size_t len, int
                     flags); 

  // = A set of wrappers for memory mapped files.
  static int madvise (caddr_t addr, size_t len, int advice);
  static void *mmap (void *addr, size_t len, int prot, int flags,
                     ACE_HANDLE handle, off_t off = 0,
                     ACE_HANDLE *file_mapping = 0);
  static int mprotect (void *addr, size_t len, int prot);
  static int msync (void *addr, size_t len, int sync);
  static int munmap (void *addr, size_t len);

  // = A set of wrappers for mutex locks.
  static int mutex_init (ACE_mutex_t *m, int type = USYNC_THREAD,
                         LPCTSTR name = 0, void *arg = 0);
  static int mutex_destroy (ACE_mutex_t *m);
  static int mutex_lock (ACE_mutex_t *m);
  static int mutex_trylock (ACE_mutex_t *m);
  static int mutex_unlock (ACE_mutex_t *m);


  // = A set of wrappers for mutex locks that only work within a
  // single process.  
  static int thread_mutex_init (ACE_thread_mutex_t *m, int type = USYNC_THREAD,
                                LPCTSTR name = 0, void *arg = 0);
  static int thread_mutex_destroy (ACE_thread_mutex_t *m);
  static int thread_mutex_lock (ACE_thread_mutex_t *m);
  static int thread_mutex_trylock (ACE_thread_mutex_t *m);
  static int thread_mutex_unlock (ACE_thread_mutex_t *m);

  // = A set of wrappers for low-level file operations.
  static int access (const char *path, int amode);
  static int close (ACE_HANDLE handle);
  static ACE_HANDLE creat (LPCTSTR filename, mode_t mode);
  static ACE_HANDLE dup (ACE_HANDLE handle);
  static int dup2 (ACE_HANDLE oldfd, ACE_HANDLE newfd);
  static int fattach (int handle, const char *path);
  static long filesize (ACE_HANDLE handle);
  static int getmsg (ACE_HANDLE handle, struct strbuf *ctl, struct strbuf
                     *data, int *flags); 
  static getpmsg (ACE_HANDLE handle, struct strbuf *ctl, struct strbuf
                  *data, int *band, int *flags); 
  static int ioctl (ACE_HANDLE handle, int cmd, void * = 0);
  static int isastream (ACE_HANDLE handle);
  static int isatty (ACE_HANDLE handle);
  static off_t lseek (ACE_HANDLE handle, off_t offset, int whence);
  static ACE_HANDLE open (const char *filename, int mode, int perms = 0);
  static int putmsg (ACE_HANDLE handle, const struct strbuf *ctl, const
                     struct strbuf *data, int flags); 
  static putpmsg (ACE_HANDLE handle, const struct strbuf *ctl, const
                  struct strbuf *data, int band, int flags); 
  static ssize_t read (ACE_HANDLE handle, void *buf, size_t len); 
  static ssize_t read (ACE_HANDLE handle, void *buf, size_t len, ACE_OVERLAPPED *); 
  static ssize_t readv (ACE_HANDLE handle, struct iovec *iov, int iovlen); 
  static int recvmsg (ACE_HANDLE handle, struct msghdr *msg, int flags);
  static int sendmsg (ACE_HANDLE handle, ACE_SENDMSG_TYPE *msg, int flags); 
  static ssize_t write (ACE_HANDLE handle, const void *buf, size_t nbyte);
  static ssize_t write (ACE_HANDLE handle, const void *buf, size_t nbyte, ACE_OVERLAPPED *);
  static int writev (ACE_HANDLE handle, ACE_WRITEV_TYPE *iov, int iovcnt);

  // = A set of wrappers for event demultiplexing and IPC.
  static int select (int width, fd_set *rfds, fd_set *wfds, fd_set *efds, const ACE_Time_Value *tv = 0);
  static int select (int width, fd_set *rfds, fd_set *wfds, fd_set *efds, const ACE_Time_Value &tv);
  static int poll (struct pollfd *pollfds, u_long len, ACE_Time_Value *tv = 0);
  static int poll (struct pollfd *pollfds, u_long len, const ACE_Time_Value &tv); 
  static int pipe (ACE_HANDLE handles[]);

  // = A set of wrappers for directory operations.
  static int chdir (const char *path);
  static int mkdir (const char *path, mode_t mode = ACE_DEFAULT_DIR_PERMS);
  static int mkfifo (const char *file, mode_t mode = ACE_DEFAULT_FILE_PERMS);
  static char *mktemp (char *t);
  static char *getcwd (char *, size_t);
  static mode_t umask (mode_t cmask);
  static int unlink (const char *path);

  // = A set of wrappers for random number operations.
  static int rand (void);
  static int rand_r (ACE_RANDR_TYPE seed);
  static void srand (u_int seed);

  // = A set of wrappers for readers/writer locks.
  static int rwlock_init (ACE_rwlock_t *rw, int type = USYNC_THREAD,
                          LPCTSTR name = 0, void *arg = 0);
  static int rwlock_destroy (ACE_rwlock_t *rw);
  static int rw_rdlock (ACE_rwlock_t *rw);
  static int rw_tryrdlock (ACE_rwlock_t *rw);
  static int rw_trywrlock (ACE_rwlock_t *rw);
  static int rw_unlock (ACE_rwlock_t *rw);
  static int rw_wrlock (ACE_rwlock_t *rw);

  // = A set of wrappers for auto-reset and manuaevents.
  static int event_init (ACE_event_t *event,
                         int manual_reset = 0, 
                         int initial_state = 0,
                         int type = USYNC_THREAD, 
                         LPCTSTR name = 0,
                         void *arg = 0);
  static int event_destroy (ACE_event_t *event);
  static int event_wait (ACE_event_t *event);
  static int event_timedwait (ACE_event_t *event, 
                              ACE_Time_Value *timeout);
  static int event_signal (ACE_event_t *event);
  static int event_pulse (ACE_event_t *event);
  static int event_reset (ACE_event_t *event);

  // = A set of wrappers for semaphores.
  static int sema_destroy (ACE_sema_t *s);
  static int sema_init (ACE_sema_t *s, u_int count, int type = USYNC_THREAD,
                        LPCTSTR name = 0, void *arg = 0,
                        int max = 0x7fffffff);
  static int sema_post (ACE_sema_t *s);
  static int sema_trywait (ACE_sema_t *s);
  static int sema_wait (ACE_sema_t *s);

  // = A set of wrappers for System V semaphores.
  static int semctl (int int_id, int semnum, int cmd, semun);
  static int semget (key_t key, int nsems, int flags);
  static int semop (int int_id, struct sembuf *sops, size_t nsops); 

  // = A set of wrappers for System V shared memory.
  static void *shmat (int int_id, void *shmaddr, int shmflg);
  static int shmctl (int int_id, int cmd, struct shmid_ds *buf);
  static int shmdt (void *shmaddr);
  static int shmget (key_t key, int size, int flags);

  // = A set of wrappers for Signals.
  static int kill (pid_t pid, int signum);
  static int sigaction (int signum, const struct sigaction *nsa,
                        struct sigaction *osa); 
  static int sigaddset (sigset_t *s, int signum);
  static int sigdelset (sigset_t *s, int signum);
  static int sigemptyset (sigset_t *s);
  static int sigfillset (sigset_t *s);
  static int sigismember (sigset_t *s, int signum);
  static ACE_SignalHandler signal (int signum, ACE_SignalHandler);
  static int sigprocmask (int how, const sigset_t *nsp, sigset_t *osp); 

  // = A set of wrappers for sockets.
  static ACE_HANDLE accept (ACE_HANDLE handle, struct sockaddr *addr,
                            int *addrlen);
  static int bind (ACE_HANDLE s, struct sockaddr *name, int namelen);
  static int connect (ACE_HANDLE handle, struct sockaddr *addr, int
                      addrlen); 
  static int closesocket (ACE_HANDLE s);
  static struct hostent *gethostbyaddr (const char *addr, int length,
                                        int type); 
  static struct hostent *gethostbyname (const char *name);
  static struct hostent *gethostbyaddr_r (const char *addr, int length,
                                          int type, struct hostent *result,
                                          ACE_HOSTENT_DATA buffer,
                                          int *h_errnop);
  static struct hostent *gethostbyname_r (const char *name, struct
                                          hostent *result, ACE_HOSTENT_DATA buffer,
                                          int *h_errnop);
  static int getpeername (ACE_HANDLE handle, struct sockaddr *addr,
                          int *addrlen);
  static struct protoent *getprotobyname (const char *name);
  static struct protoent *getprotobyname_r (const char *name,
                                            struct protoent *result, 
                                            ACE_PROTOENT_DATA buffer);
  static struct protoent *getprotobynumber (int proto);
  static struct protoent *getprotobynumber_r (int proto,
                                              struct protoent *result, 
                                              ACE_PROTOENT_DATA buffer);
  static struct servent *getservbyname (const char *svc, const char
                                        *proto); 
  static struct servent *getservbyname_r (const char *svc, const char *proto, 
                                          struct servent *result, 
                                          ACE_SERVENT_DATA buf);
  static int getsockname (ACE_HANDLE handle, struct sockaddr *addr,
                          int *addrlen);
  static int getsockopt (ACE_HANDLE handle, int level, int optname, char
                         *optval, int *optlen);
  static long inet_addr (const char *name);
  static char *inet_ntoa (const struct in_addr addr);
  static int inet_aton (const char *strptr, struct in_addr *addr);

  static int listen (ACE_HANDLE handle, int backlog);
  static int recv (ACE_HANDLE handle, char *buf, int len, int flags = 0);
  static int recvfrom (ACE_HANDLE handle, char *buf, int len, int flags,
                       struct sockaddr *addr, int *addrlen);
  static int send (ACE_HANDLE handle, const char *buf, int len, int
                   flags = 0);
  static int sendto (ACE_HANDLE handle, const char *buf, int len, int
                     flags, const struct sockaddr *addr, int addrlen);
  static int setsockopt (ACE_HANDLE handle, int level, int optname, 
                         const char *optval, int optlen);
  static int shutdown (ACE_HANDLE handle, int how);
  static ACE_HANDLE socket (int domain, int type, int proto);
  static int socketpair (int domain, int type, int protocol,
                         ACE_HANDLE sv[2]); 
  static int socket_init (int version_high = 1, int version_low = 1);
  // Initialize WinSock before first use (e.g., when a DLL is first
  // loaded or the first use of a socket() call.

  static int socket_fini (void);
  // Finialize WinSock after last use (e.g., when a DLL is unloaded).

  // = A set of wrappers for regular expressions.
  static char *compile (const char *instring, char *expbuf, char
                        *endbuf); 
  static int step (const char *str, char *expbuf);

  // = A set of wrappers for non-UNICODE string operations.
  static int strcasecmp (const char *s, const char *t);
  static char *strcat (char *s, const char *t);
  static char *strchr (const char *s, int c);
  static char *strrchr (const char *s, int c);
  static int strcmp (const char *s, const char *t);
  static char *strcpy (char *s, const char *t);
  static size_t strspn(const char *s1, const char *s2);
  static char *strstr (const char *s, const char *t);
  static char *strdup (const char *s);
  static size_t strlen (const char *s);
  static int strncmp (const char *s, const char *t, size_t len);
  static char *strncpy (char *s, const char *t, size_t len);
  static char *strtok (char *s, const char *tokens);
  static char *strtok_r (char *s, const char *tokens, char **lasts);
  static long strtol (const char *s, char **ptr, int base);

#if defined (ACE_HAS_UNICODE)
  // = A set of wrappers for UNICODE string operations.
  static wchar_t *strcat (wchar_t *s, const wchar_t *t);
  static wchar_t *strchr (const wchar_t *s, int c);
  static wchar_t *strrchr (const wchar_t *s, int c);
  static int strcmp (const wchar_t *s, const wchar_t *t);
  static wchar_t *strcpy (wchar_t *s, const wchar_t *t);
  static size_t strlen (const wchar_t *s);
  static int strncmp (const wchar_t *s, const wchar_t *t, size_t len);
  static wchar_t *strncpy (wchar_t *s, const wchar_t *t, size_t len);
  static wchar_t *strtok (wchar_t *s, const wchar_t *tokens);
  static long strtol (const wchar_t *s, wchar_t **ptr, int base);
  //static int isspace (wint_t c);

#if defined (ACE_WIN32)
  static wchar_t *strstr (const wchar_t *s, const wchar_t *t);
  static wchar_t *strdup (const wchar_t *s);
  static int sprintf (wchar_t *buf, const wchar_t *format, ...);

  static int access (const wchar_t *path, int amode);
  static FILE *fopen (const wchar_t *filename, const wchar_t *mode);
  static wchar_t *getenv (const wchar_t *symbol);
  static int system (const wchar_t *s);
  static int hostname (wchar_t *name, size_t maxnamelen);
  static ACE_HANDLE open (const wchar_t *filename, int mode, int perms = 0);
  static int unlink (const wchar_t *path);
  static void *dlopen (ACE_WIDE_DL_TYPE filename, int mode);
  static wchar_t *mktemp (wchar_t *t);
  static int mkdir (const wchar_t *path, mode_t mode = ACE_DEFAULT_DIR_PERMS);

#endif /* ACE_WIN32 */
#endif /* ACE_HAS_UNICODE */

  // = A set of wrappers for TLI.
  static int t_accept (ACE_HANDLE fildes, int resfd, struct t_call
                       *call); 
  static char *t_alloc (ACE_HANDLE fildes, int struct_type, int
                        fields); 
  static int t_bind (ACE_HANDLE fildes, struct t_bind *req, struct
                     t_bind *ret); 
  static int t_close (ACE_HANDLE fildes);
  static int t_connect(int fildes, struct t_call *sndcall,
                       struct t_call *rcvcall);
  static void t_error (char *errmsg);
  static int t_free (char *ptr, int struct_type);
  static int t_getinfo (ACE_HANDLE fildes, struct t_info *info);
  static int t_getname (ACE_HANDLE fildes, struct netbuf *namep, int
                        type); 
  static int t_getstate (ACE_HANDLE fildes);
  static int t_listen (ACE_HANDLE fildes, struct t_call *call);
  static int t_look (ACE_HANDLE fildes);
  static int t_open (char *path, int oflag, struct t_info *info);
  static int t_optmgmt (ACE_HANDLE fildes, struct t_optmgmt *req,
                        struct t_optmgmt *ret); 
  static int t_rcv (ACE_HANDLE fildes, char *buf, unsigned nbytes, int
                    *flags); 
  static int t_rcvdis (ACE_HANDLE fildes, struct t_discon *discon);
  static int t_rcvrel (ACE_HANDLE fildes);
  static int t_rcvudata (ACE_HANDLE fildes, struct t_unitdata
                         *unitdata, int *flags); 
  static int t_rcvuderr (ACE_HANDLE fildes, struct t_uderr *uderr); 
  static int t_snd (ACE_HANDLE fildes, char *buf, unsigned nbytes, int
                    flags); 
  static int t_snddis (ACE_HANDLE fildes, struct t_call *call);
  static int t_sndrel (ACE_HANDLE fildes);
  static int t_sync (ACE_HANDLE fildes);
  static int t_unbind (ACE_HANDLE fildes);

  // = A set of wrappers for threads.
  static int thr_continue (ACE_hthread_t target_thread);
  static int thr_create (ACE_THR_FUNC,
                         void *args, 
                         long flags, 
                         ACE_thread_t *thr_id, 
                         ACE_hthread_t *t_handle = 0,
                         u_int priority = 0,
                         void *stack = 0,
                         size_t stacksize = 0);
  static int thr_cmp (ACE_hthread_t t1, ACE_hthread_t t2);
  static int thr_equal (ACE_thread_t t1, ACE_thread_t t2);
  static void thr_exit (void *status = 0);
  static int thr_getconcurrency (void);
  static int thr_getprio (ACE_hthread_t thr_id, int *prio);
  static int thr_getspecific (ACE_thread_key_t key, void **data);
  static int thr_join (ACE_hthread_t waiter_id, void **status); 
  static int thr_join (ACE_thread_t waiter_id, ACE_thread_t *thr_id, void **status); 
  static int thr_keyfree (ACE_thread_key_t key);
  static int thr_key_detach (void *inst);
  static int thr_keycreate (ACE_thread_key_t *key, ACE_THR_DEST, void *inst = 0);
  static int thr_key_used (ACE_thread_key_t key);
  static int thr_kill (ACE_thread_t thr_id, int signum);
  static size_t thr_min_stack (void);
  static ACE_thread_t thr_self (void);
  static void thr_self (ACE_hthread_t &);
  static int thr_setconcurrency (int hint);
  static int thr_setprio (ACE_hthread_t thr_id, int prio);
  static int thr_setspecific (ACE_thread_key_t key, void *data);
  static int thr_sigsetmask (int how, const sigset_t *nsm, sigset_t *osm);
  static int thr_suspend (ACE_hthread_t target_thread);
  static int thr_setcancelstate (int new_state, int *old_state);
  static int thr_setcanceltype (int new_type, int *old_type);
  static int thr_cancel (ACE_thread_t t_id);
  static int sigwait (sigset_t *set, int *sig = 0);
  static void thr_testcancel (void);
  static void thr_yield (void);

  static ACE_thread_t NULL_thread;
  // This is necessary to deal with POSIX pthreads and their use of
  // structures for thread ids.

  static ACE_hthread_t NULL_hthread;
  // This is necessary to deal with POSIX pthreads and their use of
  // structures for thread handles.

  static ACE_thread_key_t NULL_key;
  // This is necessary to deal with POSIX pthreads and their use of
  // structures for TSS keys.

#if defined (ACE_WIN32)
  static int socket_initialized_;
  // Keeps track of whether we've already initialized WinSock...
#endif /* ACE_WIN32 */

  static void mutex_lock_cleanup (void *mutex);
  // Handle asynchronous thread cancellation cleanup.

private:
  ACE_OS (void);
  // Ensure we can't define an instance of this class.
};

#include "ace/Trace.h"

// These need to come here to avoid problems with circular dependencies.
#include "ace/Log_Msg.h"

// A useful abstraction for expressions involving operator new since
// we can change memory allocation error handling policies (e.g.,
// depending on whether ANSI/ISO exception handling semantics are
// being used). 

#if 0
#define ACE_NEW(POINTER,CONSTRUCTOR) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return; } \
     else if (ACE_LOG_MSG->op_status () == -1) { \
     int ace_error = ACE_LOG_MSG->errnum (); \
     delete POINTER; POINTER = 0; \
     ACE_LOG_MSG->op_status (-1); \
     errno = ace_error; return; \
   } } while (0)
#define ACE_NEW_RETURN(POINTER,CONSTRUCTOR,RET_VAL) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL; } \
     else if (ACE_LOG_MSG->op_status () == -1) { \
     int ace_error = ACE_LOG_MSG->errnum (); \
     delete POINTER; POINTER = 0; \
     ACE_LOG_MSG->op_status (-1); \
     errno = ace_error; return RET_VAL; \
   } } while (0)
#endif /* 0 */

#define ACE_NEW_RETURN(POINTER,CONSTRUCTOR,RET_VAL) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL; } \
     } while (0)
#define ACE_NEW(POINTER,CONSTRUCTOR) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return; } \
     } while (0)

#define ACE_DEFAULT_MUTEX_A "ACE_MUTEX"

#if defined (UNICODE)

#define ACE_DEFAULT_NAMESPACE_DIR ACE_DEFAULT_NAMESPACE_DIR_W
#define ACE_DIRECTORY_SEPARATOR_STR ACE_DIRECTORY_SEPARATOR_STR_W
#define ACE_DIRECTORY_SEPARATOR_CHAR ACE_DIRECTORY_SEPARATOR_CHAR_W

#define ACE_DEFAULT_MUTEX_W L"ACE_MUTEX"
#define ACE_DEFAULT_MUTEX ACE_DEFAULT_MUTEX_W

#else

#define ACE_DEFAULT_NAMESPACE_DIR ACE_DEFAULT_NAMESPACE_DIR_A
#define ACE_DIRECTORY_SEPARATOR_STR ACE_DIRECTORY_SEPARATOR_STR_A
#define ACE_DIRECTORY_SEPARATOR_CHAR ACE_DIRECTORY_SEPARATOR_CHAR_A

#define ACE_DEFAULT_MUTEX_W "ACE_MUTEX"
#define ACE_DEFAULT_MUTEX ACE_DEFAULT_MUTEX_A

#endif /* UNICODE */

#if defined (UNICODE)
#include "ace/SString.h"
#define ACE_WIDE_STRING(ASCII) \
ACE_WString (ASCII).fast_rep ()
#else
#define ACE_WIDE_STRING(ASCII) ASCII
#endif /* UNICODE */

#if defined (ACE_HAS_INLINED_OSCALLS)
#if defined (ACE_INLINE)
#undef ACE_INLINE
#endif /* ACE_INLINE */
#define ACE_INLINE inline
#include "ace/OS.i"
#endif /* ACE_HAS_INLINED_OSCALLS */

#endif  /* ACE_OS_H */
