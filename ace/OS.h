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

// This file should be a link to the platform/compiler-specific
// configuration file (e.g., config-sunos5-sunc++-4.x.h).  
#include "ace/config.h"

#if defined (ACE_HAS_4_4BSD_SENDMSG_RECVMSG)
// control message size to pass a file descriptor
#define ACE_BSD_CONTROL_MSG_LEN sizeof (struct cmsghdr) + sizeof (ACE_HANDLE)
#endif /* ACE_HAS_4_4BSD_SENDMSG_RECVMSG */

// Define the default constants for ACE.  Many of these are used for
// the ACE tests and applications.  You can change these values by
// defining the macros in your config.h file.

#if !defined (ACE_DEFAULT_TIMEOUT)
#define ACE_DEFAULT_TIMEOUT 5
#endif /* ACE_DEFAULT_TIMEOUT */

#if !defined (ACE_DEFAULT_THREADS)
#define ACE_DEFAULT_THREADS 1
#endif /* ACE_DEFAULT_THREADS */

// The following 3 defines are used in the IP multicast and broadcast tests.
#if !defined (ACE_DEFAULT_BROADCAST_PORT)
#define ACE_DEFAULT_BROADCAST_PORT 10000
#endif /* ACE_DEFAULT_BROADCAST_PORT */

#if !defined (ACE_DEFAULT_MULTICAST_PORT)
#define ACE_DEFAULT_MULTICAST_PORT 10001
#endif /* ACE_DEFAULT_MULTICAST_PORT */

#if !defined (ACE_DEFAULT_MULTICAST_ADDR)
#define ACE_DEFAULT_MULTICAST_ADDR "224.9.9.2"
#endif /* ACE_DEFAULT_MULTICAST_ADDR */

// Default port number for HTTP.
#if !defined (ACE_DEFAULT_HTTP_SERVER_PORT)
#define ACE_DEFAULT_HTTP_SERVER_PORT 80
#endif /* ACE_DEFAULT_HTTP_SERVER_PORT */

// Used in many IPC_SAP tests
#if !defined (ACE_DEFAULT_SERVER_PORT)
#define ACE_DEFAULT_SERVER_PORT 10002  
#endif /* ACE_DEFAULT_SERVER_PORT */

// Used in Acceptor tests
#if !defined (ACE_DEFAULT_SERVER_PORT_STR)
#define ACE_DEFAULT_SERVER_PORT_STR "10002"  
#endif /* ACE_DEFAULT_SERVER_PORT_STR */

// Used for the Service_Directory test
#if !defined (ACE_DEFAULT_SERVICE_PORT)
#define ACE_DEFAULT_SERVICE_PORT 10003 
#endif /* ACE_DEFAULT_SERVICE_PORT */

// Used for the ACE_Thread_Spawn test
#if !defined (ACE_DEFAULT_THR_PORT    )
#define ACE_DEFAULT_THR_PORT 10004 
#endif /* ACE_DEFAULT_THR_PORT */

// Used for SOCK_Connect::connect() tests
#if !defined (ACE_DEFAULT_LOCAL_PORT)
#define ACE_DEFAULT_LOCAL_PORT 10005  
#endif /* ACE_DEFAULT_LOCAL_PORT */

// Used for Connector tests
#if !defined (ACE_DEFAULT_LOCAL_PORT_STR)
#define ACE_DEFAULT_LOCAL_PORT_STR "10005" 
#endif /* ACE_DEFAULT_LOCAL_PORT_STR */

// Used for the name server.
#if !defined (ACE_DEFAULT_NAME_SERVER_PORT)
#define ACE_DEFAULT_NAME_SERVER_PORT 10006
#endif /* ACE_DEFAULT_NAME_SERVER_PORT */

#if !defined (ACE_DEFAULT_NAME_SERVER_PORT_STR)
#define ACE_DEFAULT_NAME_SERVER_PORT_STR "10006"
#endif /* ACE_DEFAULT_NAME_SERVER_PORT_STR */

// Used for the token server.
#if !defined (ACE_DEFAULT_TOKEN_SERVER_PORT)
#define ACE_DEFAULT_TOKEN_SERVER_PORT 10007
#endif /* ACE_DEFAULT_TOKEN_SERVER_PORT */

#if !defined (ACE_DEFAULT_TOKEN_SERVER_PORT_STR)
#define ACE_DEFAULT_TOKEN_SERVER_PORT_STR "10007"
#endif /* ACE_DEFAULT_TOKEN_SERVER_PORT_STR */

// Used for the logging server.
#if !defined (ACE_DEFAULT_LOGGING_SERVER_PORT)
#define ACE_DEFAULT_LOGGING_SERVER_PORT 10008
#endif /* ACE_DEFAULT_LOGGING_SERVER_PORT */

#if !defined (ACE_DEFAULT_LOGGING_SERVER_PORT_STR)
#define ACE_DEFAULT_LOGGING_SERVER_PORT_STR "10008"
#endif /* ACE_DEFAULT_LOGGING_SERVER_PORT_STR */

// Used for the logging server.
#if !defined (ACE_DEFAULT_THR_LOGGING_SERVER_PORT)
#define ACE_DEFAULT_THR_LOGGING_SERVER_PORT 10008
#endif /* ACE_DEFAULT_THR_LOGGING_SERVER_PORT */

#if !defined (ACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR)
#define ACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR "10008"
#endif /* ACE_DEFAULT_THR_LOGGING_SERVER_PORT_STR */

// Used for the gateway server.
#if !defined (ACE_DEFAULT_GATEWAY_SERVER_PORT)
#define ACE_DEFAULT_GATEWAY_SERVER_PORT 10009
#endif /* ACE_DEFAULT_GATEWAY_SERVER_PORT */

#if !defined (ACE_DEFAULT_GATEWAY_SERVER_PORT_STR)
#define ACE_DEFAULT_GATEWAY_SERVER_PORT_STR "10009"
#endif /* ACE_DEFAULT_GATEWAY_SERVER_PORT_STR */

// Used for the peer server.
#if !defined (ACE_DEFAULT_PEER_SERVER_PORT)
#define ACE_DEFAULT_PEER_SERVER_PORT 10010
#endif /* ACE_DEFAULT_PEER_SERVER_PORT */

#if !defined (ACE_DEFAULT_PEER_SERVER_PORT_STR)
#define ACE_DEFAULT_PEER_SERVER_PORT_STR "10010"
#endif /* ACE_DEFAULT_PEER_SERVER_PORT_STR */

// Used for the time server.
#if !defined (ACE_DEFAULT_TIME_SERVER_PORT)
#define ACE_DEFAULT_TIME_SERVER_PORT 10011
#endif /* ACE_DEFAULT_TIME_SERVER_PORT */

#if !defined (ACE_DEFAULT_TIME_SERVER_PORT_STR)
#define ACE_DEFAULT_TIME_SERVER_PORT_STR "10011"
#endif /* ACE_DEFAULT_TIME_SERVER_PORT_STR */

#if !defined (ACE_DEFAULT_TIME_SERVER_STR)
#define ACE_DEFAULT_TIME_SERVER_STR "ACE_TS_TIME"
#endif /* ACE_DEFAULT_TIME_SERVER_STR */

#if !defined (ACE_DEFAULT_SERVER_HOST)
#define ACE_DEFAULT_SERVER_HOST "localhost"
#endif /* ACE_DEFAULT_SERVER_HOST */

// Default shared memory key
#if !defined (ACE_DEFAULT_SHM_KEY)
#define ACE_DEFAULT_SHM_KEY 1234 
#endif /* ACE_DEFAULT_SHM_KEY */

// Default segment size used by SYSV shared memory (128 K)
#if !defined (ACE_DEFAULT_SEGMENT_SIZE)
#define ACE_DEFAULT_SEGMENT_SIZE 1024 * 128
#endif /* ACE_DEFAULT_SEGMENT_SIZE */

// Maximum number of SYSV shared memory segments 
// (does anyone know how to figure out the right values?!)
#if !defined (ACE_DEFAULT_MAX_SEGMENTS)
#define ACE_DEFAULT_MAX_SEGMENTS 6
#endif /* ACE_DEFAULT_MAX_SEGMENTS */

// Used by the FIFO tests.
#if !defined (ACE_DEFAULT_RENDEZVOUS)
#define ACE_DEFAULT_RENDEZVOUS "/tmp/fifo.ace"
#endif /* ACE_DEFAULT_RENDEZVOUS */

// Name of the map that's stored in shared memory.
#if !defined (ACE_NAME_SERVER_MAP)
#define ACE_NAME_SERVER_MAP "Name Server Map"
#endif /* ACE_NAME_SERVER_MAP */

// Default file permissions.
#if !defined (ACE_DEFAULT_FILE_PERMS)
#define ACE_DEFAULT_FILE_PERMS 0666
#endif /* ACE_DEFAULT_FILE_PERMS */

// Default directory permissions.
#if !defined (ACE_DEFAULT_DIR_PERMS)
#define ACE_DEFAULT_DIR_PERMS 0777
#endif /* ACE_DEFAULT_DIR_PERMS */

// Default size of the ACE Reactor.
#if !defined (ACE_DEFAULT_REACTOR_SIZE)
#define ACE_DEFAULT_REACTOR_SIZE FD_SETSIZE
#endif /* ACE_DEFAULT_REACTOR_SIZE */

// Default size of the ACE Map_Manager.
#if !defined (ACE_DEFAULT_MAP_SIZE)
#define ACE_DEFAULT_MAP_SIZE 1024
#endif /* ACE_DEFAULT_MAP_SIZE */

// Defaults for ACE Timer Wheel
#if !defined (ACE_DEFAULT_TIMER_WHEEL_SIZE)
#define ACE_DEFAULT_TIMER_WHEEL_SIZE 1024
#endif /* ACE_DEFAULT_TIMER_WHEEL_SIZE */

#if !defined (ACE_DEFAULT_TIMER_WHEEL_RESOLUTION)
#define ACE_DEFAULT_TIMER_WHEEL_RESOLUTION 100
#endif /* ACE_DEFAULT_TIMER_WHEEL_RESOLUTION */

// Default size for ACE Timer Hash table
#if !defined (ACE_DEFAULT_TIMER_HASH_TABLE_SIZE)
#define ACE_DEFAULT_TIMER_HASH_TABLE_SIZE 1024
#endif /* ACE_DEFAULT_TIMER_HASH_TABLE_SIZE */

// Defaults for the ACE Free List
#if !defined (ACE_DEFAULT_FREE_LIST_PREALLOC)
#define ACE_DEFAULT_FREE_LIST_PREALLOC 0
#endif /* ACE_DEFAULT_FREE_LIST_PREALLOC */

#if !defined (ACE_DEFAULT_FREE_LIST_LWM)
#define ACE_DEFAULT_FREE_LIST_LWM 0
#endif /* ACE_DEFAULT_FREE_LIST_LWM */

#if !defined (ACE_DEFAULT_FREE_LIST_HWM)
#define ACE_DEFAULT_FREE_LIST_HWM 25000
#endif /* ACE_DEFAULT_FREE_LIST_HWM */

#if !defined (ACE_DEFAULT_FREE_LIST_INC)
#define ACE_DEFAULT_FREE_LIST_INC 100
#endif /* ACE_DEFAULT_FREE_LIST_INC */

#if !defined (ACE_UNIQUE_NAME_LEN)
#define ACE_UNIQUE_NAME_LEN 100
#endif /* ACE_UNIQUE_NAME_LEN */

// Here are all ACE-specific global declarations needed throughout
// ACE.

// Helpful dump macros.
#define ACE_BEGIN_DUMP "\n====\n(%P|%t|%x)"
#define ACE_END_DUMP "====\n"

// A free list which create more elements when there aren't enough
// elements.
#define ACE_FREE_LIST_WITH_POOL 1

// A simple free list which doen't allocate/deallocate elements.
#define ACE_PURE_FREE_LIST 2

// This is used to indicate that a platform doesn't support a
// particular feature.
#define ACE_NOTSUP_RETURN(FAILVALUE) do { errno = ENOTSUP ; return FAILVALUE; } while (0)

#if defined (ACE_NDEBUG)
#define ACE_DB(X)
#else
#define ACE_DB(X) X
#endif /* ACE_NDEBUG */

// ACE_NO_HEAP_CHECK macro can be used to suppress false report of
// memory leaks. It turns off the built-in heap checking until the
// block is left. The old state will then be restored Only used for
// Win32 (in the moment).
#if defined (ACE_WIN32)
#if defined (_DEBUG)
class ACE_No_Heap_Check 
{
public:
  ACE_No_Heap_Check (void) 
    : old_state (_CrtSetDbgFlag (_CRTDBG_REPORT_FLAG))
  { _CrtSetDbgFlag (old_state & ~_CRTDBG_ALLOC_MEM_DF);}
  ~ACE_No_Heap_Check (void) { _CrtSetDbgFlag (old_state);}
private:
  int old_state;
};
#define ACE_NO_HEAP_CHECK ACE_No_Heap_Check ____no_heap;
#else /* !_DEBUG*/
#define ACE_NO_HEAP_CHECK
#endif /* _DEBUG*/
#else /* !ACE_WIN32 */
#define ACE_NO_HEAP_CHECK
#endif /* ACE_WIN32 */

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
#define ACE_POW(X) (((X) == 0)?1:(X-=1,X|=X>>1,X|=X>>2,X|=X>>4,X|=X>>8,X|=X>>16,(++X)))
#define ACE_EVEN(NUM) (((NUM) & 1) == 0)
#define ACE_ODD(NUM) (((NUM) & 1) == 1)
#define ACE_BIT_ENABLED(WORD, BIT) (((WORD) & (BIT)) != 0)
#define ACE_BIT_DISABLED(WORD, BIT) (((WORD) & (BIT)) == 0)
#define ACE_BIT_STRICTLY_ENABLED(WORD, BIT) (((WORD) & (BIT)) == BIT)
#define ACE_BIT_CMP_MASK(WORD, BIT, MASK) (((WORD) & (BIT)) == MASK)
#define ACE_SET_BITS(WORD, BITS) (WORD |= (BITS))
#define ACE_CLR_BITS(WORD, BITS) (WORD &= ~(BITS))
#define ACE_MAX(x,y) (((x)>(y))?(x):(y))
#define ACE_MIN(x,y) (((x)<(y))?(x):(y))

// Keep the compiler from complaining about parameters which are not used.
#if defined (ghs) || (__GNUC__) || defined (__hpux) || defined (__sgi)
// Some compilers complain about "statement with no effect" with (a).
// This eliminates the warnings, and no code is generated for the null
// conditional statement.  NOTE: that may only be true if -O is enabled,
// such as with GreenHills (ghs) 1.8.8.
#define ACE_UNUSED_ARG(a) {if (&a) /* null */ ;}
#else
#define ACE_UNUSED_ARG(a) (a)
#endif /* ghs */

#if defined(__sgi) || defined(VXWORKS)
#define ACE_NOTREACHED(a)
#else
#define ACE_NOTREACHED(a) a
#endif

#if defined (ACE_REQUIRES_FUNC_DEFINITIONS)
  // Provide a null definition for the function.  Not pleasant.
# define ACE_UNIMPLEMENTED_FUNC(f) f {}
#else
# define ACE_UNIMPLEMENTED_FUNC(f) f;
#endif /* ACE_REQUIRES_FUNC_DEFINITIONS */

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
#define ACE_ALLOC_HOOK_DECLARE struct __Ace {} /* Just need a dummy... */
#define ACE_ALLOC_HOOK_DEFINE(CLASS) 
#endif /* ACE_HAS_ALLOC_HOOKS */

#if defined (ACE_LACKS_KEY_T)
#if defined (ACE_WIN32)
// Win32 doesn't use numeric values to name its semaphores, it uses
// strings!
typedef char *key_t;
#else
typedef int key_t;
#endif /* ACE_WIN32 */
#endif /* ACE_LACKS_KEY_T */

#if defined (VXWORKS)
  #if defined (ghs)
    // GreenHills 1.8.8 needs the stdarg.h #include before the #include of
    // vxWorks.h.
    // Also, be sure that these #includes come _after_ the key_t typedef, and
    // before the #include of time.h.
    #include /**/ <stdarg.h>
  #endif /* ghs */

  #include /**/ <vxWorks.h>
#endif /* VXWORKS */


///////////////////////////////////////////
//                                       //
// NOTE: Please do not add any #includes //
//       before this point.  On VxWorks, //
//       vxWorks.h must be #included     //
//       first!                          //
//                                       //
///////////////////////////////////////////


#if defined (ACE_HAS_CHARPTR_SPRINTF)
#define ACE_SPRINTF_ADAPTER(X) ::strlen (X)
#else
#define ACE_SPRINTF_ADAPTER(X) X
#endif /* ACE_HAS_CHARPTR_SPRINTF */

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#if !defined (ACE_HAS_INLINED_OSCALLS)
#define ACE_HAS_INLINED_OSCALLS
#endif /* !ACE_HAS_INLINED_OSCALLS */
#else 
#define ACE_INLINE
#endif /* __ACE_INLINE__ */

// Default address for shared memory mapped files and SYSV shared memory
// (defaults to 64 M).
#if !defined (ACE_DEFAULT_BASE_ADDR)
#define ACE_DEFAULT_BASE_ADDR ((char *) (64 * 1024 * 1024)) 
#endif /* ACE_DEFAULT_BASE_ADDR */

// This fudge factor can be overriden for timers that need it, such as on
// Solaris, by defining the ACE_TIMER_SKEW symbol in the appropriate config
// header.
#if !defined (ACE_TIMER_SKEW)
#define ACE_TIMER_SKEW 0
#endif /* ACE_TIMER_SKEW */

// Nasty macro stuff to account for Microsoft Win32 DLL nonsense.  We
// use these macros so that we don't end up with ACE software
// hard-coded to Microsoft proprietary extensions to C++.

#if defined (ACE_HAS_DLL) && (ACE_HAS_DLL == 1)
#if defined (ACE_BUILD_DLL)
#if !defined (_MSC_VER) /* Mark classes as exported, Borland. */
#define ACE_Export _export
#else /* Microsoft: */
#define ACE_Export __declspec (dllexport)
#endif /* !_MSC_VER */
#else /* Using the DLL. */
#if !defined _MSC_VER
#define ACE_Export _import
#else
#define ACE_Export  __declspec (dllimport)
#endif /* !_MSC_VER */
#endif /* ACE_BUILD_DLL */
 
#else /* We're not building a DLL! */
#define ACE_Export 
#endif /* ACE_HAS_DLL */

#if defined (ACE_HAS_SVC_DLL) && (ACE_HAS_SVC_DLL == 1)
#if defined (ACE_BUILD_SVC_DLL)
#if !defined (_MSC_VER) /* Mark classes as exported, Borland. */
#define ACE_Svc_Export _export
#else /* Microsoft: */
#define ACE_Svc_Export __declspec (dllexport)
#endif /* !_MSC_VER */
#else /* Using the DLL. */
#if !defined _MSC_VER
#define ACE_Svc_Export _import
#else
#define ACE_Svc_Export  __declspec (dllimport)
#endif /* !_MSC_VER */
#endif /* ACE_BUILD_DLL || ACE_BUILD_SVC_DLL */
 
#else /* We're not building a DLL! */
#define ACE_Svc_Export 
#endif /* ACE_HAS_SVC_DLL */

// This needs to go here *first* to avoid problems with AIX.
// Just to be safe we'll do it with pthreads, too -- jwr
#if defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)
extern "C" {
#include /**/ <pthread.h>
#if defined (DIGITAL_UNIX)
#define pthread_self __pthread_self
extern "C" pthread_t pthread_self (void);
#endif /* DIGITAL_UNIX */
}
#endif /* ACE_HAS_DCETHREADS */

#if (ACE_NTRACE == 1)
#define ACE_TRACE(X)
#else
#define ACE_TRACE(X) ACE_Trace ____ (X, __LINE__, __FILE__)
#endif /* ACE_NTRACE */

#include /**/ <time.h>
#if defined (ACE_NEEDS_SYSTIME_H)
// Some platforms may need to include this, but I suspect that most
// will get it from <time.h>
#if defined (VXWORKS)
#include /**/ <sys/times.h>
#else
#include /**/ <sys/time.h>
#endif /* VXWORKS */
#endif /* ACE_NEEDS_SYSTIME_H */

#if !defined (ACE_HAS_POSIX_TIME)
// Definition per POSIX.
typedef struct timespec 
{		
  time_t tv_sec; // Seconds 
  long tv_nsec;	// Nanoseconds
} timespec_t;
#elif defined (ACE_HAS_BROKEN_POSIX_TIME)
// OSF/1 defines struct timespec in <sys/timers.h> - Tom Marrs
#include /**/ <sys/timers.h>
#endif /* !ACE_HAS_POSIX_TIME */

#if defined(ACE_LACKS_TIMESPEC_T)
typedef struct timespec timespec_t;
#endif /* ACE_LACKS_TIMESPEC_T */

#if !defined (ACE_HAS_CLOCK_GETTIME) && !defined (_CLOCKID_T)
typedef int clockid_t;
#if !defined (CLOCK_REALTIME)
#define	CLOCK_REALTIME 0
#endif /* CLOCK_REALTIME */
#endif /* ! ACE_HAS_CLOCK_GETTIME && ! _CLOCKID_T */

class ACE_Export ACE_Time_Value
  // = TITLE
  //     Operations on "timeval" structures.
  //
  // = DESCRIPTION
  //     This class centralizes all the time-related processing in
  //     ACE.  These timers are typically used in conjunction with
  //     lower-level OS mechanisms like <select>, <poll>, or
  //     <cond_timedwait>.  ACE_Time_Value help make the use of these
  //     mechanisms portable across OS platforms,
{
public:
  // = Useful constants.
  static const ACE_Time_Value zero;
  // Constant "0".

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

#if defined (ACE_WIN32)
  ACE_Time_Value (const FILETIME &ft);
  //  Initializes the ACE_Time_Value object from a Win32 FILETIME
#endif /* ACE_WIN32 */

  void set (long sec, long usec);
  // Construct a <Time_Value> from two <long>s.

  void set (double d);
  // Construct a <Time_Value> from a <double>, which is assumed to be
  // in second format, with any remainder treated as microseconds.

  void set (const timeval &t);
  // Construct a <Time_Value> from a <timeval>.

  void set (const timespec_t &t);
  // Initializes the <Time_Value> object from a <timespec_t>.

#if defined (ACE_WIN32)
  void set (const FILETIME &ft);
  //  Initializes the <Time_Value> object from a <timespec_t>. 
#endif /* ACE_WIN32 */

  long msec (void) const;
  // Converts from <Time_Value> format into milli-seconds format.

  void msec (long);
  // Converts from milli-seconds format into <Time_Value> format.

  operator timespec_t () const;
  // Returns the value of the object as a <timespec_t>. 

  operator timeval () const;
  // Returns the value of the object as a <timeval>.

  operator timeval *() const;
  // Returns a pointer to the object as a <timeval>.

#if defined (ACE_WIN32)
  operator FILETIME () const;
  // Returns the value of the object as a Win32 FILETIME.
#endif /* ACE_WIN32 */

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

private:
  void normalize (void);
  // Put the timevalue into a canonical form.

  timeval tv_;
  // Store the values as a <timeval>.
};

class ACE_Export ACE_Countdown_Time
  // = TITLE
  //     Keeps track of the amount of elapsed time.
  //
  // = DESCRIPTION
  //     This class has a side-effect on the <max_wait_time> -- every
  //     time the <stop> method is called the <max_wait_time> is
  //     updated.
{
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

#if defined (ACE_HAS_USING_KEYWORD)
#define	ACE_USING using
#else
#define ACE_USING
#endif /* ACE_HAS_USING_KEYWORD */

#if defined (ACE_HAS_TYPENAME_KEYWORD)
#define ACE_TYPENAME typename
#else
#define ACE_TYPENAME
#endif /* ACE_HAS_TYPENAME_KEYWORD */

// The following is necessary since many C++ compilers don't support
// typedef'd types inside of classes used as formal template
// arguments... ;-(.  Luckily, using the C++ preprocessor I can hide
// most of this nastiness!

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)

// Handle ACE_Message_Queue.
#define ACE_SYNCH_1 class _ACE_SYNCH
#define ACE_SYNCH_2 _ACE_SYNCH
#define ACE_SYNCH_MUTEX_T ACE_TYPENAME _ACE_SYNCH::MUTEX
#define ACE_SYNCH_CONDITION_T ACE_TYPENAME _ACE_SYNCH::CONDITION
#define ACE_SYNCH_SEMAPHORE_T ACE_TYPENAME _ACE_SYNCH::SEMAPHORE

// Handle ACE_Malloc*
#define ACE_MEM_POOL_1 class _ACE_MEM_POOL
#define ACE_MEM_POOL_2 _ACE_MEM_POOL
#define ACE_MEM_POOL _ACE_MEM_POOL
#define ACE_MEM_POOL_OPTIONS ACE_TYPENAME _ACE_MEM_POOL::OPTIONS

// Handle ACE_Svc_Handler
#define ACE_PEER_STREAM_1 class _ACE_PEER_STREAM
#define ACE_PEER_STREAM_2 _ACE_PEER_STREAM
#define ACE_PEER_STREAM _ACE_PEER_STREAM
#define ACE_PEER_STREAM_ADDR ACE_TYPENAME _ACE_PEER_STREAM::PEER_ADDR

// Handle ACE_Acceptor
#define ACE_PEER_ACCEPTOR_1 class _ACE_PEER_ACCEPTOR
#define ACE_PEER_ACCEPTOR_2 _ACE_PEER_ACCEPTOR
#define ACE_PEER_ACCEPTOR _ACE_PEER_ACCEPTOR
#define ACE_PEER_ACCEPTOR_ADDR ACE_TYPENAME _ACE_PEER_ACCEPTOR::PEER_ADDR

// Handle ACE_Connector
#define ACE_PEER_CONNECTOR_1 class _ACE_PEER_CONNECTOR
#define ACE_PEER_CONNECTOR_2 _ACE_PEER_CONNECTOR
#define ACE_PEER_CONNECTOR _ACE_PEER_CONNECTOR
#define ACE_PEER_CONNECTOR_ADDR ACE_TYPENAME _ACE_PEER_CONNECTOR::PEER_ADDR
#if !defined(ACE_HAS_TYPENAME_KEYWORD)
#define ACE_PEER_CONNECTOR_ADDR_ANY ACE_PEER_CONNECTOR_ADDR::sap_any
#else
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
#define ACE_PEER_CONNECTOR_ADDR_ANY ACE_PEER_ADDR_TYPEDEF::sap_any
#endif /* ACE_HAS_TYPENAME_KEYWORD */ 

// Handle ACE_SOCK_*
#define ACE_SOCK_ACCEPTOR ACE_SOCK_Acceptor
#define ACE_SOCK_CONNECTOR ACE_SOCK_Connector
#define ACE_SOCK_STREAM ACE_SOCK_Stream

// Handle ACE_LSOCK_*
#define ACE_LSOCK_ACCEPTOR ACE_LSOCK_Acceptor
#define ACE_LSOCK_CONNECTOR ACE_LSOCK_Connector
#define ACE_LSOCK_STREAM ACE_LSOCK_Stream

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

#else /* TEMPLATES are broken in some form or another (i.e., most C++ compilers) */

// Handle ACE_Message_Queue.
#if defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
#define ACE_SYNCH_1 class _ACE_SYNCH_MUTEX_T, class _ACE_SYNCH_CONDITION_T, class _ACE_SYNCH_SEMAPHORE_T
#define ACE_SYNCH_2 _ACE_SYNCH_MUTEX_T, _ACE_SYNCH_CONDITION_T, _ACE_SYNCH_SEMAPHORE_T
#else
#define ACE_SYNCH_1 class _ACE_SYNCH_MUTEX_T, class _ACE_SYNCH_CONDITION_T
#define ACE_SYNCH_2 _ACE_SYNCH_MUTEX_T, _ACE_SYNCH_CONDITION_T
#endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */
#define ACE_SYNCH_MUTEX_T _ACE_SYNCH_MUTEX_T
#define ACE_SYNCH_CONDITION_T _ACE_SYNCH_CONDITION_T
#define ACE_SYNCH_SEMAPHORE_T _ACE_SYNCH_SEMAPHORE_T

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

// Handle ACE_Connector
#define ACE_PEER_CONNECTOR_1 class _ACE_PEER_CONNECTOR, class _ACE_PEER_ADDR
#define ACE_PEER_CONNECTOR_2 _ACE_PEER_CONNECTOR, _ACE_PEER_ADDR
#define ACE_PEER_CONNECTOR _ACE_PEER_CONNECTOR
#define ACE_PEER_CONNECTOR_ADDR _ACE_PEER_ADDR
#define ACE_PEER_CONNECTOR_ADDR_ANY ACE_PEER_CONNECTOR_ADDR::sap_any

// Handle ACE_SOCK_*
#define ACE_SOCK_ACCEPTOR ACE_SOCK_Acceptor, ACE_INET_Addr
#define ACE_SOCK_CONNECTOR ACE_SOCK_Connector, ACE_INET_Addr
#define ACE_SOCK_STREAM ACE_SOCK_Stream, ACE_INET_Addr

// Handle ACE_LSOCK_*
#define ACE_LSOCK_ACCEPTOR ACE_LSOCK_Acceptor, ACE_UNIX_Addr
#define ACE_LSOCK_CONNECTOR ACE_LSOCK_Connector, ACE_UNIX_Addr
#define ACE_LSOCK_STREAM ACE_LSOCK_Stream, ACE_UNIX_Addr

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

#define S_IRWXU 00700           /* read, write, execute: owner. */
#define S_IRUSR 00400           /* read permission: owner. */
#define S_IWUSR 00200           /* write permission: owner. */
#define S_IXUSR 00100           /* execute permission: owner. */
#define S_IRWXG 00070           /* read, write, execute: group. */
#define S_IRGRP 00040           /* read permission: group. */
#define S_IWGRP 00020           /* write permission: group. */
#define S_IXGRP 00010           /* execute permission: group. */
#define S_IRWXO 00007           /* read, write, execute: other. */
#define S_IROTH 00004           /* read permission: other. */
#define S_IWOTH 00002           /* write permission: other. */
#define S_IXOTH 00001           /* execute permission: other. */

#endif /* ACE_LACKS_MODE_MASKS */

#if defined (ACE_LACKS_SEMBUF_T)
struct sembuf 
{
  u_short sem_num; // semaphore # 
  short sem_op; // semaphore operation 
  short sem_flg; // operation flags 
};
#endif /* ACE_LACKS_SEMBUF_T */

#if defined (ACE_HAS_H_ERRNO)
void herror (const char *str);
#endif /* ACE_HAS_H_ERRNO */

#if defined (ACE_LACKS_UALARM_PROTOTYPE)
extern "C" u_int ualarm (u_int usecs, u_int interval);
#endif /* ACE_LACKS_UALARM_PROTOTYPE */

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
#  if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
#    include /**/ <cwchar>
#  else /* ACE_HAS_STANDARD_CPP_LIBRARY */
#    include /**/ <wchar.h>
#  endif /* ACE_HAS_STANDARD_CPP_LIBRARY */
#endif /* ACE_HAS_UNICODE */

#if defined (ACE_HAS_BROKEN_WRITEV)
typedef struct iovec ACE_WRITEV_TYPE;
#else
typedef const struct iovec ACE_WRITEV_TYPE;
#endif /* ACE_HAS_BROKEN_WRITEV */
 
#if defined (ACE_HAS_BROKEN_READV)
typedef const struct iovec ACE_READV_TYPE;
#else
typedef struct iovec ACE_READV_TYPE;
#endif /* ACE_HAS_BROKEN_WRITEV */

#if defined (ACE_HAS_BROKEN_SETRLIMIT)
typedef struct rlimit ACE_SETRLIMIT_TYPE;
#else
typedef const struct rlimit ACE_SETRLIMIT_TYPE;
#endif /* ACE_HAS_BROKEN_SETRLIMIT */

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
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

#if !defined (ACE_DEFAULT_THREAD_PRIORITY)
#define ACE_DEFAULT_THREAD_PRIORITY (-0x7fffffffL - 1L)
#endif /* ACE_DEFAULT_THREAD_PRIORITY */

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

#  if defined (ACE_HAS_STHREADS)
#    include /**/ <synch.h>
#    include /**/ <thread.h>
#    define ACE_SCOPE_PROCESS P_PID
#    define ACE_SCOPE_LWP P_LWPID
#    define ACE_SCOPE_THREAD (ACE_SCOPE_LWP + 1)
#  else
#    define ACE_SCOPE_PROCESS 0
#    define ACE_SCOPE_LWP 1
#    define ACE_SCOPE_THREAD 2
#  endif /* ACE_HAS_STHREADS */

#  if ! (defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS))
#    define ACE_SCHED_OTHER 0
#    define ACE_SCHED_FIFO 1
#    define ACE_SCHED_RR 2
#  endif /* ! (ACE_HAS_DCETHREADS || ACE_HAS_PTHREADS) */

#  if defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)
#    define ACE_SCHED_OTHER SCHED_OTHER
#    define ACE_SCHED_FIFO SCHED_FIFO
#    define ACE_SCHED_RR SCHED_RR

// Definitions for mapping POSIX pthreads onto Solaris threads.

#    if defined (ACE_HAS_FSU_PTHREADS)
#      define PTHREAD_DETACHED        0x1
#      define PTHREAD_SCOPE_SYSTEM    0x2
#      define PTHREAD_INHERIT_SCHED   0x4
#      define PTHREAD_NOFLOAT         0x8
#      define PTHREAD_CREATE_UNDETACHED       0
#      define PTHREAD_CREATE_DETACHED         PTHREAD_DETACHED
#      define PTHREAD_CREATE_JOINABLE         0
#      define PTHREAD_SCOPE_PROCESS           0
#      define PTHREAD_EXPLICIT_SCHED          0
#      define PTHREAD_MIN_PRIORITY            0
#      define PTHREAD_MAX_PRIORITY            126
#    endif /* ACE_HAS_FSU_PTHREADS */

#    if defined (ACE_HAS_DCETHREADS) && !defined (ACE_HAS_DCE_DRAFT4_THREADS)
#      define PRIORITY_MAX                    PTHREAD_MAX_PRIORITY
#    endif /* ACE_HAS_DCETHREADS */

// Definitions for THREAD- and PROCESS-LEVEL priorities...some
// implementations define these while others don't.  In order to
// further complicate matters, we don't redefine the default (*_DEF)
// values if they've already been defined, which allows individual
// programs to have their own ACE-wide "default".

// PROCESS-level values
#    define ACE_PROC_PRI_FIFO_MIN  (sched_get_priority_min(SCHED_FIFO))
#    define ACE_PROC_PRI_FIFO_MAX  (sched_get_priority_max(SCHED_FIFO))
#    define ACE_PROC_PRI_RR_MIN    (sched_get_priority_min(SCHED_RR))
#    define ACE_PROC_PRI_RR_MAX    (sched_get_priority_max(SCHED_RR))
#    define ACE_PROC_PRI_OTHER_MIN (sched_get_priority_min(SCHED_OTHER))
#    define ACE_PROC_PRI_OTHER_MAX (sched_get_priority_max(SCHED_OTHER))
#    if !defined(ACE_PROC_PRI_FIFO_DEF)
#      define ACE_PROC_PRI_FIFO_DEF (ACE_PROC_PRI_FIFO_MIN + (ACE_PROC_PRI_FIFO_MAX - ACE_PROC_PRI_FIFO_MIN)/2)
#    endif
#    if !defined(ACE_PROC_PRI_RR_DEF)
#      define ACE_PROC_PRI_RR_DEF (ACE_PROC_PRI_RR_MIN + (ACE_PROC_PRI_RR_MAX - ACE_PROC_PRI_RR_MIN)/2)
#    endif
#    if !defined(ACE_PROC_PRI_OTHER_DEF)
#      define ACE_PROC_PRI_OTHER_DEF (ACE_PROC_PRI_OTHER_MIN + (ACE_PROC_PRI_OTHER_MAX - ACE_PROC_PRI_OTHER_MIN)/2)
#    endif

// THREAD-level values
#    if defined(PRI_FIFO_MIN) && defined(PRI_FIFO_MAX) && defined(PRI_RR_MIN) && defined(PRI_RR_MAX) && defined(PRI_OTHER_MIN) && defined(PRI_OTHER_MAX)
#      define ACE_THR_PRI_FIFO_MIN  PRI_FIFO_MIN
#      define ACE_THR_PRI_FIFO_MAX  PRI_FIFO_MAX
#      define ACE_THR_PRI_RR_MIN    PRI_RR_MIN
#      define ACE_THR_PRI_RR_MAX    PRI_RR_MAX
#      define ACE_THR_PRI_OTHER_MIN PRI_OTHER_MIN
#      define ACE_THR_PRI_OTHER_MAX PRI_OTHER_MAX
#    else
#      define ACE_THR_PRI_FIFO_MIN  ACE_PROC_PRI_FIFO_MIN
#      define ACE_THR_PRI_FIFO_MAX  ACE_PROC_PRI_FIFO_MAX
#      define ACE_THR_PRI_RR_MIN    ACE_PROC_PRI_RR_MIN
#      define ACE_THR_PRI_RR_MAX    ACE_PROC_PRI_RR_MAX
#      define ACE_THR_PRI_OTHER_MIN ACE_PROC_PRI_OTHER_MIN
#      define ACE_THR_PRI_OTHER_MAX ACE_PROC_PRI_OTHER_MAX
#    endif
#    if !defined(ACE_THR_PRI_FIFO_DEF)
#      define ACE_THR_PRI_FIFO_DEF  (ACE_THR_PRI_FIFO_MIN + (ACE_THR_PRI_FIFO_MAX - ACE_THR_PRI_FIFO_MIN)/2)
#    endif
#    if !defined(ACE_THR_PRI_RR_DEF)
#      define ACE_THR_PRI_RR_DEF (ACE_THR_PRI_RR_MIN + (ACE_THR_PRI_RR_MAX - ACE_THR_PRI_RR_MIN)/2)
#    endif
#    if !defined(ACE_THR_PRI_OTHER_DEF)
#      define ACE_THR_PRI_OTHER_DEF (ACE_THR_PRI_OTHER_MIN + (ACE_THR_PRI_OTHER_MAX - ACE_THR_PRI_OTHER_MIN)/2)
#    endif

#    if !defined (ACE_HAS_TID_T)
typedef pthread_t tid_t;
#    endif /* ACE_HAS_TID_T */

// Typedefs to help compatibility with Windows NT and Pthreads.
#    if defined (ACE_HAS_PTHREAD_T)
typedef pthread_t ACE_hthread_t;
#    else /* ACE_HAS_PTHREAD_T */
typedef tid_t ACE_hthread_t;
#    endif /* ACE_HAS_PTHREAD_T */

// Make it easier to write portable thread code.
typedef pthread_t ACE_thread_t;
typedef pthread_key_t ACE_thread_key_t;
typedef pthread_mutex_t ACE_mutex_t;
typedef pthread_cond_t ACE_cond_t;
typedef pthread_mutex_t ACE_thread_mutex_t;

#    if !defined (PTHREAD_CANCEL_DISABLE)
#      define PTHREAD_CANCEL_DISABLE      0
#    endif /* PTHREAD_CANCEL_DISABLE */

#    if !defined (PTHREAD_CANCEL_ENABLE)
#      define PTHREAD_CANCEL_ENABLE       0
#    endif /* PTHREAD_CANCEL_ENABLE */

#    if !defined (PTHREAD_CANCEL_DEFERRED)
#      define PTHREAD_CANCEL_DEFERRED     0
#    endif /* PTHREAD_CANCEL_DEFERRED */

#    if !defined (PTHREAD_CANCEL_ASYNCHRONOUS)
#      define PTHREAD_CANCEL_ASYNCHRONOUS 0
#    endif /* PTHREAD_CANCEL_ASYNCHRONOUS */

#    define THR_CANCEL_DISABLE      PTHREAD_CANCEL_DISABLE
#    define THR_CANCEL_ENABLE       PTHREAD_CANCEL_ENABLE
#    define THR_CANCEL_DEFERRED     PTHREAD_CANCEL_DEFERRED
#    define THR_CANCEL_ASYNCHRONOUS PTHREAD_CANCEL_ASYNCHRONOUS

#    if !defined (PTHREAD_CREATE_JOINABLE)
#      if defined (PTHREAD_CREATE_UNDETACHED)
#        define PTHREAD_CREATE_JOINABLE PTHREAD_CREATE_UNDETACHED
#      else
#        define PTHREAD_CREATE_JOINABLE 0
#      endif /* PTHREAD_CREATE_UNDETACHED */
#    endif /* PTHREAD_CREATE_JOINABLE */

#    if !defined (PTHREAD_CREATE_DETACHED)
#      define PTHREAD_CREATE_DETACHED 1
#    endif /* PTHREAD_CREATE_DETACHED */

#    if !defined (PTHREAD_PROCESS_PRIVATE)
#      if defined (PTHREAD_MUTEXTYPE_FAST)
#        define PTHREAD_PROCESS_PRIVATE PTHREAD_MUTEXTYPE_FAST
#      else
#        define PTHREAD_PROCESS_PRIVATE 0
#      endif /* PTHREAD_MUTEXTYPE_FAST */
#    endif /* PTHREAD_PROCESS_PRIVATE */

#    if !defined (PTHREAD_PROCESS_SHARED)
#      if defined (PTHREAD_MUTEXTYPE_FAST)
#        define PTHREAD_PROCESS_SHARED PTHREAD_MUTEXTYPE_FAST
#      else
#        define PTHREAD_PROCESS_SHARED 0
#      endif /* PTHREAD_MUTEXTYPE_FAST */
#    endif /* PTHREAD_PROCESS_SHARED */

#    if defined (ACE_HAS_DCETHREADS)
#      if defined (PTHREAD_PROCESS_PRIVATE)
#        define USYNC_THREAD    PTHREAD_PROCESS_PRIVATE
#      else
#        define USYNC_THREAD    MUTEX_NONRECURSIVE_NP
#      endif /* PTHREAD_PROCESS_PRIVATE */

#      if defined (PTHREAD_PROCESS_SHARED)
#        define USYNC_PROCESS   PTHREAD_PROCESS_SHARED
#      else
#        define USYNC_PROCESS   MUTEX_NONRECURSIVE_NP
#      endif /* PTHREAD_PROCESS_SHARED */
#    elif !defined (ACE_HAS_STHREADS)
#      define USYNC_THREAD PTHREAD_PROCESS_PRIVATE
#      define USYNC_PROCESS PTHREAD_PROCESS_SHARED
#    endif /* ACE_HAS_DCETHREADS */

#    define THR_BOUND               0x00000001
#  if defined (CHORUS)
#    define THR_NEW_LWP             0x00000000
#  else
#    define THR_NEW_LWP             0x00000002
#  endif /* CHORUS */
#    define THR_DETACHED            0x00000040
#    define THR_SUSPENDED           0x00000080
#    define THR_DAEMON              0x00000100
#    define THR_JOINABLE            0x00010000
#    define THR_SCHED_FIFO          0x00020000
#    define THR_SCHED_RR            0x00040000
#    define THR_SCHED_DEFAULT       0x00080000
#    if defined (ACE_HAS_IRIX62_THREADS)
#      define THR_SCOPE_SYSTEM        0x00100000
#    else
#      define THR_SCOPE_SYSTEM        THR_BOUND
#    endif /* ACE_HAS_IRIX62_THREADS */
#    define THR_SCOPE_PROCESS       0x00200000
#    define THR_INHERIT_SCHED       0x00400000
#    define THR_EXPLICIT_SCHED      0x00800000
#    define THR_SCHED_IO	    0x01000000

#    if !defined (ACE_HAS_STHREADS)
#    if !defined (ACE_HAS_POSIX_SEM)
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
#    endif /* !ACE_HAS_POSIX_SEM */

#if defined (ACE_LACKS_PTHREAD_YIELD) && defined (ACE_HAS_THR_YIELD)
#if defined (USYNC_THREAD)
#undef USYNC_THREAD
#endif /* USYNC_THREAD */
#if defined (USYNC_PROCESS)
#undef USYNC_PROCESS
#endif /* USYNC_PROCESS */
#    include /**/ <thread.h>
#endif /* defined (ACE_LACKS_PTHREAD_YIELD) && defined (ACE_HAS_THR_YIELD) */

#  else
// If we are on Solaris we can just reuse the existing implementations
// of these synchronization types.
typedef rwlock_t ACE_rwlock_t;
#    if !defined (ACE_HAS_POSIX_SEM)
typedef sema_t ACE_sema_t;
#    endif /* !ACE_HAS_POSIX_SEM */
#  endif /* !ACE_HAS_STHREADS */
#elif defined (ACE_HAS_STHREADS)
// Solaris threads, without PTHREADS.
// Typedefs to help compatibility with Windows NT and Pthreads.
typedef thread_t ACE_thread_t;
typedef thread_key_t ACE_thread_key_t;
typedef mutex_t ACE_mutex_t;
typedef rwlock_t ACE_rwlock_t;
#  if !defined (ACE_HAS_POSIX_SEM)
typedef sema_t ACE_sema_t;
#  endif /* !ACE_HAS_POSIX_SEM */
typedef cond_t ACE_cond_t;
typedef ACE_thread_t ACE_hthread_t;
typedef ACE_mutex_t ACE_thread_mutex_t;

#  define THR_CANCEL_DISABLE      0
#  define THR_CANCEL_ENABLE       0
#  define THR_CANCEL_DEFERRED     0
#  define THR_CANCEL_ASYNCHRONOUS 0
#  define THR_JOINABLE            0
#  define THR_SCHED_FIFO          0
#  define THR_SCHED_RR            0
#  define THR_SCHED_DEFAULT       0
#elif defined (VXWORKS)
// For mutex implementation using mutual-exclusion semaphores (which
// can be taken recursively).
#  include /**/ <semLib.h>    

#  include /**/ <hostLib.h>
#  include /**/ <ioLib.h>
#  include /**/ <remLib.h>
#  include /**/ <selectLib.h>
#  include /**/ <sigLib.h>
#  include /**/ <sockLib.h>
#  include /**/ <sysLib.h>
#  include /**/ <taskLib.h>
#  include /**/ <taskHookLib.h>

extern "C" 
struct sockaddr_un {
  short sun_family;    // AF_UNIX.
  char  sun_path[108]; // path name.
};

#define MAXPATHLEN  1024
#define MAXNAMLEN   255
#define NSIG (_NSIGS + 1)

// task options:  the other options are either obsolete, internal, or for
// Fortran or Ada support
#  define VX_UNBREAKABLE	0x0002	/* breakpoints ignored */
#  define VX_FP_TASK	   	0x0008	/* floating point coprocessor */
#  define VX_PRIVATE_ENV 	0x0080	/* private environment support */
#  define VX_NO_STACK_FILL	0x0100	/* do not stack fill for checkstack () */

#  define THR_CANCEL_DISABLE      0
#  define THR_CANCEL_ENABLE       0
#  define THR_CANCEL_DEFERRED     0
#  define THR_CANCEL_ASYNCHRONOUS 0
#  define THR_BOUND               0
#  define THR_NEW_LWP             0
#  define THR_DETACHED            0
#  define THR_SUSPENDED           0
#  define THR_DAEMON              0
#  define THR_JOINABLE            0
#  define THR_SCHED_FIFO          0
#  define THR_SCHED_RR            0
#  define THR_SCHED_DEFAULT       0
#  define USYNC_THREAD            0
#  define USYNC_PROCESS           1 /* it's all global on VxWorks (without MMU
                                     option) */

typedef SEM_ID ACE_mutex_t;
// implement ACE_thread_mutex_t with ACE_mutex_t sinces there's just one process . . .
typedef ACE_mutex_t ACE_thread_mutex_t;
#  if !defined (ACE_HAS_POSIX_SEM)
// Use VxWorks semaphores, wrapped . . .
typedef struct
{
  SEM_ID sema_;
  // Semaphore handle.  This is allocated by VxWorks.
  
  char *name_;
  // Name of the semaphore:  always NULL with VxWorks.
} ACE_sema_t;
#  endif /* !ACE_HAS_POSIX_SEM */
typedef char * ACE_thread_t;
typedef int ACE_hthread_t;
// Key type: the ACE TSS emulation requires the key type be unsigned,
// for efficiency.  (Current POSIX and Solaris TSS implementations also
// use unsigned int, so the ACE TSS emulation is compatible with them.)
typedef u_int ACE_thread_key_t;

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

// These need to be different values, neither of which can be 0...
#  define USYNC_THREAD 1
#  define USYNC_PROCESS 2

#  define THR_CANCEL_DISABLE      0
#  define THR_CANCEL_ENABLE       0
#  define THR_CANCEL_DEFERRED     0
#  define THR_CANCEL_ASYNCHRONOUS 0
#  define THR_DETACHED    0       /* ?? ignore in most places */
#  define THR_BOUND       0       /* ?? ignore in most places */
#  define THR_NEW_LWP     0       /* ?? ignore in most places */
#  define THR_JOINABLE    0       /* ?? ignore in most places */
#  define THR_SUSPENDED   CREATE_SUSPENDED
#  define THR_USE_AFX             0x01000000
#  define THR_SCHED_FIFO          0
#  define THR_SCHED_RR            0
#  define THR_SCHED_DEFAULT       0
#endif /* ACE_HAS_DCETHREADS || ACE_HAS_PTHREADS */

#if defined (ACE_LACKS_COND_T)
class ACE_Export ACE_cond_t
  // = TITLE
  //     This structure is used to implement condition variables on
  //     VxWorks and Win32.
  //
  // = DESCRIPTION
  //     At the current time, this stuff only works for threads
  //     within the same process.
{
  friend class ACE_OS;
public:
  long waiters (void) const;
  // Returns the number of waiters.

protected:
  long waiters_;
  // Number of waiting threads.

  ACE_thread_mutex_t waiters_lock_;
  // Serialize access to the waiters count.

  ACE_sema_t sema_;
  // Queue up threads waiting for the condition to become signaled.

#if defined (VXWORKS)
  ACE_sema_t waiters_done_;
  // A semaphore used by the broadcast/signal thread to wait for all
  // the waiting thread(s) to wake up and be released from the
  // semaphore.
#elif defined (ACE_WIN32)
  HANDLE waiters_done_;
  // An auto reset event used by the broadcast/signal thread to wait
  // for the waiting thread(s) to wake up and get a chance at the
  // semaphore.
#else
#error "SOMEONE FIX ME!"
#endif /* VXWORKS */

  size_t was_broadcast_;
  // Keeps track of whether we were broadcasting or just signaling.
};
#endif /* ACE_LACKS_COND_T */

#if defined (ACE_LACKS_RWLOCK_T)
struct ACE_Export ACE_rwlock_t
  // = TITLE
  //     This is used to implement readers/writer locks on NT,
  //     VxWorks, and POSIX pthreads.
  //
  // = DESCRIPTION
  //     At the current time, this stuff only works for threads
  //     within the same process.
{
friend class ACE_OS;
protected:
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
#elif !defined (ACE_HAS_STHREADS)
#include /**/ <synch.h>
typedef rwlock_t ACE_rwlock_t;
#endif /* ACE_LACKS_RWLOCK_T */

#else /* !ACE_HAS_THREADS, i.e., the OS/platform doesn't support threading. */
// Give these things some reasonable value...
#define ACE_SCOPE_PROCESS 0
#define ACE_SCOPE_LWP 1
#define ACE_SCOPE_THREAD 2
#define ACE_SCHED_OTHER 0
#define ACE_SCHED_FIFO 1
#define ACE_SCHED_RR 2
#define THR_CANCEL_DISABLE      0
#define THR_CANCEL_ENABLE       0
#define THR_CANCEL_DEFERRED     0
#define THR_CANCEL_ASYNCHRONOUS 0
#define THR_JOINABLE    0       /* ?? ignore in most places */
#define THR_DETACHED    0       /* ?? ignore in most places */
#define THR_DAEMON      0       /* ?? ignore in most places */
#define THR_BOUND       0       /* ?? ignore in most places */
#define THR_NEW_LWP     0       /* ?? ignore in most places */
#define THR_SUSPENDED   0       /* ?? ignore in most places */
#define THR_SCHED_FIFO          0
#define THR_SCHED_RR            0
#define THR_SCHED_DEFAULT       0
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
typedef u_int ACE_thread_key_t;
#endif /* ACE_HAS_THREADS */

#include /**/ <sys/types.h>
#include /**/ <sys/stat.h>

#include "ace/stdcpp.h"

#if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
class ostream;
#endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

#include /**/ <fcntl.h>

// This must come after signal.h is #included.
#if defined (SCO)
#define SIGIO SIGPOLL
#include /**/ <sys/regset.h>
#endif /* SCO */

#if defined (ACE_HAS_SIG_MACROS)
#undef sigemptyset
#undef sigfillset
#undef sigaddset
#undef sigdelset
#undef sigismember
#endif /* ACE_HAS_SIG_MACROS */

// This must come after signal.h is #included.  It's to counteract
// the sigemptyset and sigfillset #defines, which only happen
// when __OPTIMIZE__ is #defined (really!) on Linux.
#if defined (linux) && defined (__OPTIMIZE__)
#undef sigemptyset
#undef sigfillset
#endif /* linux && __OPTIMIZE__ */

// sigwait is yet another macro on Digital UNIX 4.0,
// just causing trouble when introducing member functions with the same name.
// Thanks to Thilo Kielmann" <kielmann@informatik.uni-siegen.de> for this fix.
#if defined (DIGITAL_UNIX)
#undef sigwait
#endif /* DIGITAL_UNIX */

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
# if defined (__hpux)
// HP-UX's stdlib.h (long *) doesn't match that man page (unsigned int *)
typedef long *ACE_RANDR_TYPE;
# else
typedef unsigned int *ACE_RANDR_TYPE;
# endif /* __hpux */
#endif /* ACE_HAS_BROKEN_RANDR */

#if defined (ACE_HAS_UTIME)
#include /**/ <utime.h>
#endif /* ACE_HAS_UTIME */

#if !defined (ACE_HAS_MSG) && !defined (SCO)
struct msghdr {};
#endif /* ACE_HAS_MSG */

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
#elif defined (ACE_HAS_TANDEM_SIGNALS)     
typedef void (*ACE_SignalHandler)(...);    
typedef void (*ACE_SignalHandlerV)(...);   
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
#if !defined (m88k)     /*  with SVR4_SIGNAL_T */
typedef void (*ACE_SignalHandlerV)(void);
#else
typedef void (*ACE_SignalHandlerV)(int);
#endif  //  m88k        /*  with SVR4_SIGNAL_T */
#elif defined (ACE_WIN32)
typedef void (__cdecl *ACE_SignalHandler)(int); 
typedef void (__cdecl *ACE_SignalHandlerV)(int);
#elif defined (ACE_HAS_UNIXWARE_SVR4_SIGNAL_T)
typedef void (*ACE_SignalHandler)(int);
typedef void (*ACE_SignalHandlerV)(...);
#else /* This is necessary for some older broken version of cfront */
#if defined (SIG_PF)
#define ACE_SignalHandler SIG_PF
#else
typedef void (*ACE_SignalHandler)(int);	
#endif /* SIG_PF */
typedef void (*ACE_SignalHandlerV)(...);
#endif /* ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES */

#if defined (BUFSIZ)
#define ACE_STREAMBUF_SIZE BUFSIZ
#else
#define ACE_STREAMBUF_SIZE 1024
#endif /* BUFSIZ */

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
#else
#include /**/ <sys/utsname.h>
#endif /* ACE_LACKS_UTSNAME_T */

#if defined (ACE_WIN32)
// Turn off warnings for /W4
// To resume any of these warning: #pragma warning(default: 4xxx)
// which should be placed after these defines
#ifndef ALL_WARNINGS
// #pragma warning(disable: 4101)  // unreferenced local variable
#pragma warning(disable: 4127)  /* constant expression for TRACE/ASSERT */
#pragma warning(disable: 4134)  /* message map member fxn casts */
#pragma warning(disable: 4511)  /* private copy constructors are good to have */
#pragma warning(disable: 4512)  /* private operator= are good to have */
#pragma warning(disable: 4514)  /* unreferenced inlines are common */
#pragma warning(disable: 4710)  /* private constructors are disallowed */
#pragma warning(disable: 4705)  /* statement has no effect in optimized code */
// #pragma warning(disable: 4701)  // local variable *may* be used without init
// #pragma warning(disable: 4702)  // unreachable code caused by optimizations
#pragma warning(disable: 4791)  /* loss of debugging info in retail version */
// #pragma warning(disable: 4204)  // non-constant aggregate initializer
#pragma warning(disable: 4275)  /* deriving exported class from non-exported */
#pragma warning(disable: 4251)  /* using non-exported as public in exported */
#pragma warning(disable: 4786)  /* identifier was truncated to '255' characters in the browser information */
#pragma warning(disable: 4097)  /* typedef-name used as synonym for class-name */
#endif /*!ALL_WARNINGS */

// STRICT type checking in WINDOWS.H enhances type safety for Windows
// programs by using distinct types to represent all the different
// HANDLES in Windows. So for example, STRICT prevents you from
// mistakenly passing an HPEN to a routine expecting an HBITMAP.
// Note that we only use this if we 
#if defined (ACE_HAS_STRICT) && (ACE_HAS_STRICT != 0)
#if !defined (STRICT)	/* may already be defined */
#define STRICT
#endif /* !STRICT */
#endif /* ACE_HAS_STRICT */

#include /**/ <sys/timeb.h>

// The following defines are used by the ACE Name Server...
#if !defined (ACE_DEFAULT_NAMESPACE_DIR_W)
#define ACE_DEFAULT_NAMESPACE_DIR_W L"C:\\temp\\"
#endif /* ACE_DEFAULT_NAMESPACE_DIR_W */

#if !defined (ACE_DEFAULT_NAMESPACE_DIR_A)
#define ACE_DEFAULT_NAMESPACE_DIR_A "C:\\temp\\"
#endif /* ACE_DEFAULT_NAMESPACE_DIR_A */

#if !defined (ACE_DEFAULT_LOCALNAME_A)
#define ACE_DEFAULT_LOCALNAME_A "localnames"
#endif /* ACE_DEFAULT_LOCALNAME_A */

#if !defined (ACE_DEFAULT_LOCALNAME_W)
#define ACE_DEFAULT_LOCALNAME_W L"localnames"
#endif /* ACE_DEFAULT_LOCALNAME_W */

#if !defined (ACE_DEFAULT_GLOBALNAME_A)
#define ACE_DEFAULT_GLOBALNAME_A "globalnames"
#endif /* ACE_DEFAULT_GLOBALNAME_A */

#if !defined (ACE_DEFAULT_GLOBALNAME_W)
#define ACE_DEFAULT_GLOBALNAME_W L"globalnames"
#endif /* ACE_DEFAULT_GLOBALNAME_W */

// Need to work around odd glitches with NT.
#if !defined (ACE_MAX_DEFAULT_PORT)
#define ACE_MAX_DEFAULT_PORT 0
#endif /* ACE_MAX_DEFAULT_PORT */

// We're on WinNT or Win95
#define ACE_PLATFORM_A "Win32"
#define ACE_PLATFORM_EXE_SUFFIX_A ".exe"
#define ACE_PLATFORM_W L"Win32"
#define ACE_PLATFORM_EXE_SUFFIX_W L".exe"

// Used for ACE_MMAP_Memory_Pool
#if !defined (ACE_DEFAULT_BACKING_STORE)
#define ACE_DEFAULT_BACKING_STORE __TEXT ("C:\\temp\\ace-malloc-XXXXXX")
#endif /* ACE_DEFAULT_BACKING_STORE */

// Used for logging
#if !defined (ACE_DEFAULT_LOGFILE)
#define ACE_DEFAULT_LOGFILE "C:\\temp\\logfile"
#endif /* ACE_DEFAULT_LOGFILE */

// Used for dynamic linking
#if !defined (ACE_DEFAULT_SVC_CONF)
#define ACE_DEFAULT_SVC_CONF ".\\svc.conf"
#endif /* ACE_DEFAULT_SVC_CONF */

// The following are #defines and #includes that are specific to
// WIN32.
#define ACE_STDIN GetStdHandle (STD_INPUT_HANDLE)
#define ACE_STDOUT GetStdHandle (STD_OUTPUT_HANDLE)
#define ACE_STDERR GetStdHandle (STD_ERROR_HANDLE)

// Default semaphore key and mutex name
#if !defined (ACE_DEFAULT_SEM_KEY)
#define ACE_DEFAULT_SEM_KEY "ACE_SEM_KEY"
#endif /* ACE_DEFAULT_SEM_KEY */

#define ACE_INVALID_SEM_KEY 0

#define ACE_SEH_TRY __try
#define ACE_SEH_EXCEPT(X) __except(X)
#define ACE_SEH_FINALLY __finally

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
#define NFDBITS 32 /* only used in unused functions... */
// These two may be used for internal flags soon:
#define MAP_PRIVATE 1
#define MAP_SHARED  2
#define MAP_FIXED   4

#define RUSAGE_SELF 1

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

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
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
typedef ACE_QWORD ACE_hrtime_t;
inline ACE_QWORD ACE_MAKE_QWORD (DWORD lo, DWORD hi) { return uint64 (lo, hi); }
inline DWORD ACE_LOW_DWORD  (ACE_QWORD q) { return q.LowPart; }
inline DWORD ACE_HIGH_DWORD (ACE_QWORD q) { return q.HighPart; }
#else
typedef unsigned __int64 ACE_QWORD;
typedef signed __int64 ACE_hrtime_t;  /* VC++ won't convert unsigned __int64 to double */
// typedef unsigned __int64 ACE_hrtime_t; /* Why do we need this? */
inline ACE_QWORD ACE_MAKE_QWORD (DWORD lo, DWORD hi) { return ACE_QWORD (lo) | (ACE_QWORD (hi) << 32); }
inline DWORD ACE_LOW_DWORD  (ACE_QWORD q) { return (DWORD) q; }
inline DWORD ACE_HIGH_DWORD (ACE_QWORD q) { return (DWORD) (q >> 32); }
#endif /* !defined (_MSC_VER) */

// Win32 dummies to help compilation.

typedef int mode_t;
typedef int uid_t;
typedef int gid_t;
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

struct iovec
{
  char *iov_base; // data to be read/written
  size_t iov_len; // byte count to read/write
};

typedef DWORD ACE_id_t;
typedef int ACE_pri_t;

// Dynamic loading-related types - used for dlopen and family.
#if !defined(RTLD_LAZY)
# define RTLD_LAZY 1
#endif /* !RTLD_LAZY */
typedef HINSTANCE ACE_SHLIB_HANDLE;
const int ACE_DEFAULT_SHLIB_MODE = 0;

#else /* !defined (ACE_WIN32) */

typedef const char *LPCTSTR;
typedef char *LPTSTR;
typedef char TCHAR;

#if !defined (__TEXT)
#if (defined (ACE_HAS_UNICODE) && (defined (UNICODE)))
#define __TEXT(STRING) L##STRING
#else
#define __TEXT(STRING) STRING 
#endif /* UNICODE && ACE_HAS_UNICODE */
#endif /* !defined __TEXT */

#if defined (m88k)
#define RUSAGE_SELF 1
#endif  /*  m88k */

// Default port is MAX_SHORT.
#define ACE_MAX_DEFAULT_PORT 65535

// Default semaphore key
#if !defined (ACE_DEFAULT_SEM_KEY)
#define ACE_DEFAULT_SEM_KEY 1234
#endif /* ACE_DEFAULT_SEM_KEY */

#define ACE_INVALID_SEM_KEY -1

// Define the pathname separator characters for UNIX.
#define ACE_DIRECTORY_SEPARATOR_STR_A "/"
#define ACE_DIRECTORY_SEPARATOR_CHAR_A '/'

// We're some kind of UNIX...
#define ACE_PLATFORM_A "UNIX"
#define ACE_PLATFORM_EXE_SUFFIX_A ""

#if defined (ACE_HAS_UNICODE)
#define ACE_DIRECTORY_SEPARATOR_STR_W L"/"
#define ACE_DIRECTORY_SEPARATOR_CHAR_W L'/'
#define ACE_PLATFORM_W L"UNIX"
#define ACE_PLATFORM_EXE_SUFFIX_W L""
#else
#define ACE_DIRECTORY_SEPARATOR_STR_W "/"
#define ACE_DIRECTORY_SEPARATOR_CHAR_W '/'
#define ACE_PLATFORM_W "UNIX"
#define ACE_PLATFORM_EXE_SUFFIX_W ""
#endif /* ACE_HAS_UNICODE */

#define ACE_LD_SEARCH_PATH "LD_LIBRARY_PATH"
#define ACE_LD_SEARCH_PATH_SEPARATOR_STR ":"
#define ACE_LOGGER_KEY "/tmp/server_daemon"
#if defined (__hpux)
#define ACE_DLL_SUFFIX ".sl"
#else
#define ACE_DLL_SUFFIX ".so"
#endif /* __hpux */
#define ACE_DLL_PREFIX "lib"

// The following 3 defines are used by the ACE Name Server...
#if !defined (ACE_DEFAULT_NAMESPACE_DIR_A)
#define ACE_DEFAULT_NAMESPACE_DIR_A "/tmp"
#endif /* ACE_DEFAULT_NAMESPACE_DIR_A */

#if !defined (ACE_DEFAULT_LOCALNAME_A)
#define ACE_DEFAULT_LOCALNAME_A "/localnames"
#endif /* ACE_DEFAULT_LOCALNAME_A */

#if !defined (ACE_DEFAULT_GLOBALNAME_A)
#define ACE_DEFAULT_GLOBALNAME_A "/globalnames"
#endif /* ACE_DEFAULT_GLOBALNAME_A */

#if defined (ACE_HAS_UNICODE)
#if !defined (ACE_DEFAULT_NAMESPACE_DIR_W)
#define ACE_DEFAULT_NAMESPACE_DIR_W L"/tmp"
#endif /* ACE_DEFAULT_NAMESPACE_DIR_W */
#if !defined (ACE_DEFAULT_LOCALNAME_W)
#define ACE_DEFAULT_LOCALNAME_W L"/localnames"
#endif /* ACE_DEFAULT_LOCALNAME_W */
#if !defined (ACE_DEFAULT_GLOBALNAME_W)
#define ACE_DEFAULT_GLOBALNAME_W L"/globalnames"
#endif /* ACE_DEFAULT_GLOBALNAME_W */
#else
#if !defined (ACE_DEFAULT_NAMESPACE_DIR_W)
#define ACE_DEFAULT_NAMESPACE_DIR_W "/tmp"
#endif /* ACE_DEFAULT_NAMESPACE_DIR_W */
#if !defined (ACE_DEFAULT_LOCALNAME_W)
#define ACE_DEFAULT_LOCALNAME_W "/localnames"
#endif /* ACE_DEFAULT_LOCALNAME_W */
#if !defined (ACE_DEFAULT_GLOBALNAME_W)
#define ACE_DEFAULT_GLOBALNAME_W "/globalnames"
#endif /* ACE_DEFAULT_GLOBALNAME_W */
#endif /* ACE_HAS_UNICODE */

// Used for ACE_MMAP_Memory_Pool
#if !defined (ACE_DEFAULT_BACKING_STORE)
#define ACE_DEFAULT_BACKING_STORE "/tmp/ace-malloc-XXXXXX" 
#endif /* ACE_DEFAULT_BACKING_STORE */

// Used for logging
#if !defined (ACE_DEFAULT_LOGFILE)
#define ACE_DEFAULT_LOGFILE "/tmp/logfile"
#endif /* ACE_DEFAULT_LOGFILE */

// Used for dynamic linking.
#if !defined (ACE_DEFAULT_SVC_CONF)
#define ACE_DEFAULT_SVC_CONF "./svc.conf"
#endif /* ACE_DEFAULT_SVC_CONF */

// The following are #defines and #includes that are specific to UNIX.

#define ACE_STDIN 0
#define ACE_STDOUT 1
#define ACE_STDERR 2

// Be consistent with Winsock naming
#define ACE_INVALID_HANDLE -1
#define ACE_SYSCALL_FAILED -1

#define ACE_SEH_TRY
#define ACE_SEH_EXCEPT(X)
#define ACE_SEH_FINALLY

// The "null" device on UNIX.
#define ACE_DEV_NULL "/dev/null"

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
#if !defined (LPSECURITY_ATTRIBUTES)
#define LPSECURITY_ATTRIBUTES int
#endif /* !defined LPSECURITY_ATTRIBUTES */
#if !defined (GENERIC_READ)
#define GENERIC_READ 0
#endif /* !defined GENERIC_READ */
#if !defined (FILE_SHARE_READ)
#define FILE_SHARE_READ 0
#endif /* !defined FILE_SHARE_READ */
#if !defined (OPEN_EXISTING)
#define OPEN_EXISTING 0
#endif /* !defined OPEN_EXISTING */
#if !defined (FILE_ATTRIBUTE_NORMAL)
#define FILE_ATTRIBUTE_NORMAL 0
#endif /* !defined FILE_ATTRIBUTE_NORMAL */
#if !defined (MAXIMUM_WAIT_OBJECTS)
#define MAXIMUM_WAIT_OBJECTS 0
#endif /* !defined MAXIMUM_WAIT_OBJECTS */
#if !defined (FILE_FLAG_OVERLAPPED)
#define FILE_FLAG_OVERLAPPED 0
#endif /* !defined FILE_FLAG_OVERLAPPED */

#if defined (ACE_HAS_BROKEN_IF_HEADER)
struct ifafilt;
#endif /* ACE_HAS_BROKEN_IF_HEADER */

#if defined (ACE_HAS_AIX_BROKEN_SOCKET_HEADER)
  #undef __cplusplus
  #include /**/ <sys/socket.h>
  #define __cplusplus
#else
  #include /**/ <sys/socket.h>
#endif /* ACE_HAS_AIX_BROKEN_SOCKET_HEADER */

extern "C" 
{
#if defined (VXWORKS)
  struct  hostent {
    char    *h_name;        /* official name of host */
    int     h_addrtype;     /* host address type */
    int     h_length;       /* address length */
    char    **h_addr_list;  /* (first, only) address from name server */
#define	h_addr h_addr_list[0]	/* the first address */
  };
#else
#include /**/ <netdb.h>
#endif /* VXWORKS */
#include /**/ <net/if.h>
#include /**/ <netinet/in.h>
#include /**/ <arpa/inet.h>
}
#if !defined (ACE_LACKS_TCP_H)
#include /**/ <netinet/tcp.h>
#endif /* ACE_LACKS_TCP_H */

#if defined (CHORUS)
#include /**/ <chorus.h>
#include /**/ <cx/select.h>
#include /**/ <sys/uio.h>
#include /**/ <time.h>
#include /**/ <stdfileio.h>
#include /**/ <am/afexec.h>

// This must come after limits.h is included
#define MAXPATHLEN _POSIX_PATH_MAX

typedef cx_fd_mask fd_mask;
#ifndef howmany
#define howmany(x, y)   (((x)+((y)-1))/(y))
#endif /* howmany */
typedef void (*__sighandler_t)(int); // keep Signal compilation happy
#elif ! defined (VXWORKS)
#include /**/ <sys/uio.h>
#include /**/ <sys/ipc.h>
#include /**/ <sys/shm.h>
#include /**/ <sys/sem.h>
#include /**/ <sys/file.h>
#include /**/ <sys/time.h>
#include /**/ <sys/resource.h>
#include /**/ <sys/wait.h>
#include /**/ <pwd.h>
#endif /* ! VXWORKS */
#include /**/ <sys/ioctl.h>
#include /**/ <dirent.h>
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

/* Set the proper handle type for dynamically-loaded libraries. */
/* Also define a default 'mode' for loading a library - the names and values */
/* differ between OSes, so if you write code that uses the mode, be careful */
/* of the platform differences. */
#if defined (ACE_HAS_SVR4_DYNAMIC_LINKING)
# include /**/ <dlfcn.h>
  typedef void *ACE_SHLIB_HANDLE;
#if !defined (RTLD_LAZY)
#define RTLD_LAZY 1
#endif /* !RTLD_LAZY */
  const int ACE_DEFAULT_SHLIB_MODE = RTLD_LAZY;
#elif defined (__hpux)
# if __cplusplus >= 199707L
#   include /**/ <dl.h>
# else
#   include /**/ <cxxdl.h>
# endif /* HP aC++ vs. HP C++ */
  typedef shl_t ACE_SHLIB_HANDLE;
  const int ACE_DEFAULT_SHLIB_MODE = BIND_DEFERRED;
#else
  #if !defined(RTLD_LAZY)
  # define RTLD_LAZY 1
  #endif /* !RTLD_LAZY */
  typedef void *ACE_SHLIB_HANDLE;
  const int ACE_DEFAULT_SHLIB_MODE = RTLD_LAZY;
#endif /* ACE_HAS_SVR4_DYNAMIC_LINKING */

#if defined (ACE_HAS_SOCKIO_H)
#include /**/ <sys/sockio.h>
#endif /* ACE_HAS_SOCKIO_ */

// There must be a better way to do this...
#if !defined (RLIMIT_NOFILE)
#if defined (linux) || defined (AIX) || defined (SCO)
#if defined (RLIMIT_OFILE)
#define RLIMIT_NOFILE RLIMIT_OFILE
#else
#define RLIMIT_NOFILE 200
#endif /* RLIMIT_OFILE */
#endif /* defined (linux) || defined (AIX) || defined (SCO) */
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
#if !defined (ACE_LACKS_SYSV_MSG_H)
#include /**/ <sys/msg.h>
#endif /* ACE_LACKS_SYSV_MSG_H */
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

#if defined (ACE_HAS_STHREADS)
#include /**/ <sys/priocntl.h>
#if defined (ACE_LACKS_PRI_T)
typedef int pri_t;
#endif /* ACE_LACKS_PRI_T */
typedef id_t ACE_id_t;
typedef pri_t ACE_pri_t;
#else
typedef long ACE_id_t;
typedef short ACE_pri_t;
#endif /* ACE_HAS_STHREADS */

#if defined (ACE_HAS_HI_RES_TIMER)
  /* hrtime_t is defined on systems (Suns) with ACE_HAS_HI_RES_TIMER */
typedef hrtime_t ACE_hrtime_t;
#elif defined (ACE_HAS_LONGLONG_T)
typedef unsigned long long ACE_hrtime_t;
#else
class ACE_Export ACE_U_LongLong
  // = TITLE
  //     Unsigned long long for platforms that don't have one.
  //
  // = DESCRIPTION
  //     Provide our own unsigned long long.  This is intended to be
  //     use with ACE_High_Res_Timer, so the division operator assumes
  //     that the quotient fits into a u_long.
  //     Please note that the constructor takes (optionally) two values.
  //     The high one contributes 0x100000000 times its value.  So,
  //     for example, (0, 2) is _not_ 20000000000, but instead
  //     0x200000000.  To emphasize this, the default values are expressed
  //     in hex, and dump () outputs the value in hex.
{
public:
  // = Initialization and termination methods.
  ACE_U_LongLong (u_long lo = 0x0, u_long hi = 0x0);
  ACE_U_LongLong (const ACE_U_LongLong &);
  ACE_U_LongLong &operator= (const ACE_U_LongLong &);
  ~ACE_U_LongLong (void);

  // = Overloaded relation operators.
  int operator== (const ACE_U_LongLong &) const;
  int operator< (const ACE_U_LongLong &) const;
  int operator<= (const ACE_U_LongLong &) const;
  int operator> (const ACE_U_LongLong &) const;
  int operator>= (const ACE_U_LongLong &) const;

  ACE_U_LongLong operator+ (const ACE_U_LongLong &) const;
  ACE_U_LongLong operator- (const ACE_U_LongLong &) const;
  u_long operator/ (u_long) const;

  ACE_U_LongLong &operator+= (const ACE_U_LongLong &);
  ACE_U_LongLong &operator-= (const ACE_U_LongLong &);

  // = Helper methods.
  void output (FILE * = stdout) const;
  // Outputs the value to the FILE, in hex.

  u_long hi (void) const;
  u_long lo (void) const;

  void hi (u_long hi);
  void lo (u_long lo);

  ACE_ALLOC_HOOK_DECLARE;

private:
  u_long hi_;
  u_long lo_;
};

typedef ACE_U_LongLong ACE_hrtime_t;
#endif /* ACE_HAS_HI_RES_TIMER */

#endif /* ACE_WIN32 */

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

#if defined (ACE_LACKS_SIGSET)
typedef unsigned int sigset_t; 
#endif /* ACE_LACKS_SIGSET */

#if defined (ACE_LACKS_SIGACTION)
struct sigaction 
{
  int sa_flags;
  ACE_SignalHandlerV sa_handler;
  sigset_t sa_mask;
};
#endif /* ACE_LACKS_SIGACTION */

// This one exists only to please Service_Config.h and
// Service_Manager.cpp
#if !defined (SIGHUP)
#define SIGHUP 1
#endif /* SIGHUP */

#if !defined (SIGINT)
#define SIGINT 2
#endif /* SIGINT */

#if !defined (SIGSEGV)
#define SIGSEGV 11
#endif /* SIGSEGV */

#if !defined (SIG_DFL)
#define SIG_DFL ((__sighandler_t)0)
#endif /* SIG_DFL */

#if !defined (SIG_IGN)
#define SIG_IGN ((__sighandler_t)1)     /* ignore signal */
#endif /* SIG_IGN */

#if !defined (SIG_ERR)
#define SIG_ERR ((__sighandler_t)-1)    /* error return from signal */
#endif /* SIG_ERR */

#if !defined (SIGIO)
#define SIGIO 29
#endif /* SIGSEGV */

#if !defined (SIGCHLD)
#define SIGCHLD 18
#endif /* SIGCHLD */

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

#if !defined (IP_DROP_MEMBERSHIP)
#define IP_DROP_MEMBERSHIP 0
#endif /* IP_DROP_MEMBERSHIP */

#if !defined (IP_ADD_MEMBERSHIP)
#define IP_ADD_MEMBERSHIP 0
#endif /* IP_ADD_MEMBERSHIP */

#if !defined (SIOCGIFADDR)
#define SIOCGIFADDR 0
#endif /* SIOCGIFADDR */

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
#define NSIG 0
#endif /* NSIG */

#if !defined (R_OK)
#define R_OK    04      /* Test for Read permission. */
#endif /* R_OK */

#if !defined (W_OK)
#define W_OK    02      /* Test for Write permission. */
#endif /* W_OK */

#if !defined (X_OK)
#define X_OK    01      /* Test for eXecute permission. */
#endif /* X_OK */

#if !defined (F_OK)
#define F_OK    0       /* Test for existence of File. */
#endif /* F_OK */

#if !defined (EIDRM)
#define EIDRM 0
#endif /* !EIDRM */

#if !defined (ENOTSUP)
#define ENOTSUP ENOSYS  /* Operation not supported      . */
#endif /* !ENOTSUP */

#if !defined (WNOHANG)
#define WNOHANG 42
#endif /* !WNOHANG */

#if !defined (EDEADLK)
#define EDEADLK 1000 /* Some large number.... */
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

#if !defined (WNOHANG)
#define WNOHANG 0
#endif /* WNOHANG */

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
typedef int ACE_INT32;
#else
typedef u_long ACE_UINT32;
typedef long ACE_INT32;
#endif /* ACE_HAS_64BIT_LONGS */

#if !defined (ETIMEDOUT) && defined (ETIME)
#define ETIMEDOUT ETIME
#endif /* ETIMEDOUT */

#if !defined (ETIME) && defined (ETIMEDOUT)
#define ETIME ETIMEDOUT
#endif /* ETIMED */

#if !defined (_SC_TIMER_MAX)
#define _SC_TIMER_MAX 44
#endif /* _SC_TIMER_MAX */

// Default number of <ACE_Event_Handler>s supported by
// <ACE_Timer_Heap>.
#if !defined (ACE_DEFAULT_TIMERS)
#define ACE_DEFAULT_TIMERS _SC_TIMER_MAX
#endif /* ACE_DEFAULT_TIMERS */

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

// Max size of an ACE Log Record data buffer.  This can be reset in
// the config.h file if you'd like to increase or decrease the size.
#if !defined (ACE_MAXLOGMSGLEN)
#define ACE_MAXLOGMSGLEN 4 * 1024
#endif /* ACE_MAXLOGMSGLEN */

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

extern "C" {
typedef void (*ACE_THR_C_DEST)(void *);
}
typedef void (*ACE_THR_DEST)(void *);

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
  siginfo_t (ACE_HANDLE *handles);	// JCEJ 12/23/96

  ACE_HANDLE si_handle_;
  // Win32 HANDLE that has become signaled.

  ACE_HANDLE *si_handles_;
  // Array of Win32 HANDLEs all of which have become signaled.
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

class ACE_Export ACE_Thread_ID
  // = TITLE
  //     Defines a platform-independent thread ID.
{
public:
  // = Initialization method.
  ACE_Thread_ID (ACE_thread_t, ACE_hthread_t);

  // = Set/Get the Thread ID.
  ACE_thread_t id (void);
  void id (ACE_thread_t);

  // = Set/Get the Thread handle.
  ACE_hthread_t handle (void);
  void handle (ACE_hthread_t);

  // != Comparison operator.
  int operator == (const ACE_Thread_ID &);
  int operator != (const ACE_Thread_ID &);

private:
  ACE_thread_t thread_id_;
  // Identify the thread.

  ACE_hthread_t thread_handle_;
  // Handle to the thread (typically used to "wait" on Win32).
};

// Type of the extended signal handler.
typedef void (*ACE_Sig_Handler_Ex) (int, siginfo_t *siginfo, ucontext_t *ucontext);

// If the xti.h file redefines the function names, do it now, else
// when the function definitions are encountered, they won't match the
// declaration here.

#if defined (ACE_REDEFINES_XTI_FUNCTIONS)
#include /**/ <xti.h>
#if defined (UNIXWARE)         /* They apparantly forgot one... */
extern "C" int _xti_error(char *);
#endif /* UNIXWARE */
#endif /* ACE_REDEFINES_XTI_FUNCTIONS */

// = The ACE_Sched_Priority type should be used for platform-
//   independent thread and process priorities, by convention.
//   int should be used for OS-specific priorities.
typedef int ACE_Sched_Priority;

// forward declaration
class ACE_Sched_Params;

#if defined (ACE_LACKS_FILELOCKS)
#if ! defined (VXWORKS)
// VxWorks defines struct flock in sys/fcntlcom.h.  But it doesn't
// appear to support flock ().
struct flock 
{
  short	l_type;
  short	l_whence;
  off_t	l_start;
  off_t	l_len;		/* len == 0 means until end of file */
  long	l_sysid;
  pid_t	l_pid;
  long	l_pad[4];		/* reserve area */
};
#endif /* ! VXWORKS */
#endif /* ACE_LACKS_FILELOCKS */

#if !defined (ACE_HAS_IP_MULTICAST)
struct ip_mreq 
{
  struct in_addr imr_multiaddr;  
  // IP multicast address of group
  struct in_addr imr_interface;  
  // local IP address of interface
};
#endif /* ACE_HAS_IP_MULTICAST */

#if !defined (ACE_HAS_STRBUF_T)
struct strbuf
{
  int maxlen; // no. of bytes in buffer.
  int len;    // no. of bytes returned.
  void *buf;  // pointer to data.
};
#endif /* ACE_HAS_STRBUF_T */

struct ACE_Export ACE_Str_Buf : public strbuf 
  // = TITLE
  //     Simple wrapper for STREAM pipes strbuf.
{
  // = Initialization method
  ACE_Str_Buf (void *b = 0, int l = 0, int max = 0);
  // Constructor.

  ACE_Str_Buf (strbuf &);
  // Constructor.
};

#if defined (ACE_HAS_BROKEN_BITSHIFT)
#define ACE_MSB_MASK (~(ACE_UINT32 (1) << ACE_UINT32 (NFDBITS - 1)))
#else
// This needs to go here to avoid overflow problems on some compilers.
#define ACE_MSB_MASK (~(1 << (NFDBITS - 1)))
#endif /* ACE_HAS_BROKEN_BITSHIFT */

// Run the thread entry point for the <ACE_Thread_Adapter>.  This must
// be an extern "C" to make certain compilers happy...
extern "C" void *ace_thread_adapter (void *args);

// Forward decl.
class ACE_Thread_Manager;

class ACE_Thread_Adapter
  // = TITLE
  //     Converts a C++ function into a function <ace_thread_adapter>
  //     function that can be called from a thread creation routine
  //     (e.g., <pthread_create> or <_beginthreadex>) that expects an
  //     extern "C" entry point.  This class also makes it possible to
  //     transparently provide hooks to register a thread with an
  //     <ACE_Thread_Manager>.
  // 
  // = DESCRIPTION
  //     This class is used in <ACE_OS::thr_create>.
{
public:
  ACE_Thread_Adapter (ACE_THR_FUNC user_func,
		      void *arg,
		      ACE_THR_C_FUNC entry_point = (ACE_THR_C_FUNC) ace_thread_adapter,
		      ACE_Thread_Manager *tmgr = 0);
  // Constructor.

  void *invoke (void);
  // Execute the <user_func_> with the <arg>.

  ACE_Thread_Manager *thr_mgr (void);
  // Accessor for the optional <Thread_Manager>. 

  ACE_THR_C_FUNC entry_point (void);
  // Accessor for the C entry point function to the OS thread creation
  // routine.

private:
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

#if !defined (ACE_THREADS_DONT_INHERIT_LOG_MSG)
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
#endif /* ACE_THREADS_DONT_INHERIT_LOG_MSG */
};

class ACE_Export ACE_OS
  // = TITLE
  //     This class defines an operating system independent
  //     programming API that shields developers from non-portable
  //     aspects of writing efficient system programs on Win32, POSIX,
  //     and other versions of UNIX.  If you are porting ACE to a new
  //     platform, this is the place to focus your attention.  Please
  //     see the README file in this directory for complete
  //     information on the meaning of the various macros.
  //
  // = DESCRIPTION
  //     This class encapsulates all the differences between various
  //     versions of UNIX and WIN32!  The other components in
  //     ACE are programmed to use only the methods in this class,
  //     which makes it *much* easier to move ACE to a new platform.
  //     The methods in this class also automatically restart when 
  //     interrupts occur during system calls (assuming that the
  //     <ACE_Log_Msg::restart> flag is enabled).
{
public:
  struct ace_flock_t
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
  static int getopt (int argc,
		     char *const *argv,
		     const char *optstring); 
  static long sysconf (int);

  // = A set of wrappers for condition variables.
  static int cond_broadcast (ACE_cond_t *cv);
  static int cond_destroy (ACE_cond_t *cv);
  static int cond_init (ACE_cond_t *cv,
			int type = USYNC_THREAD,
			LPCTSTR name = 0,
			void *arg = 0);
  static int cond_signal (ACE_cond_t *cv);
  static int cond_timedwait (ACE_cond_t *cv,
			     ACE_mutex_t *m,
			     ACE_Time_Value *);
  static int cond_wait (ACE_cond_t *cv,
			ACE_mutex_t *m);
#if defined (ACE_WIN32) && defined (ACE_HAS_WTHREADS)
  static int cond_timedwait (ACE_cond_t *cv,
			     ACE_thread_mutex_t *m,
			     ACE_Time_Value *);
  static int cond_wait (ACE_cond_t *cv,
			ACE_thread_mutex_t *m);
#endif /* ACE_WIN32 && ACE_HAS_WTHREADS */

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
  static char *dlerror (void);
  static ACE_SHLIB_HANDLE dlopen (ACE_DL_TYPE filename,
				  int mode = ACE_DEFAULT_SHLIB_MODE);
  static void *dlsym (ACE_SHLIB_HANDLE handle, ACE_DL_TYPE symbol);

  // = A set of wrappers for stdio file operations.
  static int last_error (void);
  static void last_error (int);
  static int fclose (FILE *fp);
  static int fcntl (ACE_HANDLE handle,
		    int cmd,
		    int val = 0);
  static int fdetach (const char *file);
  static FILE *fdopen (ACE_HANDLE handle,
		       const char *mode);
  static char *fgets (char *buf,
		      int size,
		      FILE *fp);
  static int fflush (FILE *fp);
  static FILE *fopen (const char *filename,
		      const char *mode);
  static int fprintf (FILE *fp,
		      const char *format,
		      ...);
  static size_t fread (void *ptr,
		       size_t size,
		       size_t nelems,
		       FILE *fp); 
  static int fstat (ACE_HANDLE,
		    struct stat *);
  static int stat (const char *file,
		   struct stat *);
  static int ftruncate (ACE_HANDLE,
			off_t);
  static size_t fwrite (const void *ptr,
			size_t size,
			size_t nitems,
                        FILE *fp); 
  static char *gets (char *str);
  static void perror (const char *s);
  static int printf (const char *format, ...);
  static int puts (const char *s);
  static void rewind (FILE *fp);
  static int sprintf (char *buf,
		      const char *format,
		      ...);
  static int vsprintf (char *buffer,
		       const char *format,
		       va_list argptr); 
  
  // = A set of wrappers for file locks.
  static int flock_init (ACE_OS::ace_flock_t *lock,
			 int flags = 0, 
                         LPCTSTR name = 0,
			 mode_t perms = 0);
  static int flock_destroy (ACE_OS::ace_flock_t *lock);
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
  static pid_t fork (void);
  static pid_t fork (const char *program_name);
  static pid_t fork_exec (char *argv[]);
  // Forks and exec's a process in a manner that works on Solaris and
  // NT.  argv[0] must be the full path name to the executable.

  static gid_t getgid (void);
  static pid_t getpid (void);
  static pid_t getpgid (pid_t pid);
  static uid_t getuid (void);
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
  static ACE_hrtime_t gethrtime (void);
#if defined (ACE_HAS_POWERPC) && defined (ghs)
  static void readPPCTimeBase (u_long &most,
			       u_long &least);
#endif /* ACE_HAS_POWERPC && ghs */
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

#if defined (ACE_HAS_BROKEN_R_ROUTINES)
#undef ctime_r
#undef asctime_r
#undef rand_r   
#undef getpwnam_r
#endif /* ACE_HAS_BROKEN_R_ROUTINES */

#if defined (difftime)
#define ACE_DIFFTIME(t1, t0) difftime(t1,t0)
#undef difftime
#endif /* difftime */

  // = A set of wrappers for operations on time.
  static double difftime (time_t t1,
			  time_t t0);
  static time_t time (time_t *tloc = 0);
  static time_t mktime (struct tm *timeptr);
  static struct tm *localtime (const time_t *clock);
  static struct tm *localtime_r (const time_t *clock,
				 struct tm *res);
  static struct tm *gmtime (const time_t *clock);
  static struct tm *gmtime_r (const time_t *clock,
			      struct tm *res);
  static char *asctime (const struct tm *tm);
  static char *asctime_r (const struct tm *tm,
			  char *buf, int buflen);
  static char *ctime (const time_t *t);
  static char *ctime_r (const time_t *clock,
			char *buf,
			int buflen);
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

  // = A set of wrappers for mutex locks.
  static int mutex_init (ACE_mutex_t *m,
			 int type = USYNC_THREAD,
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
				int type = USYNC_THREAD,
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
  static ssize_t pread (ACE_HANDLE handle,
			void *buf,
			size_t nbyte,
			off_t offset);
  static ssize_t readv (ACE_HANDLE handle,
			struct iovec *iov,
			int iovlen); 
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
  static int writev (ACE_HANDLE handle,
		     const struct iovec *iov,
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

  // = A set of wrappers for directory operations.
  static int chdir (const char *path);
  static int mkdir (const char *path,
		    mode_t mode = ACE_DEFAULT_DIR_PERMS);
  static int mkfifo (const char *file,
		     mode_t mode = ACE_DEFAULT_FILE_PERMS);
  static char *mktemp (char *t);
  static char *getcwd (char *,
		       size_t);
  static mode_t umask (mode_t cmask);
  static int unlink (const char *path);
  static char *tempnam (const char *dir,
			const char *pfx);

  // = A set of wrappers for random number operations.
  static int rand (void);
  static int rand_r (ACE_RANDR_TYPE seed);
  static void srand (u_int seed);

  // = A set of wrappers for readers/writer locks.
  static int rwlock_init (ACE_rwlock_t *rw,
			  int type = USYNC_THREAD,
                          LPCTSTR name = 0,
			  void *arg = 0);
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
			u_int count, int type = USYNC_THREAD,
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
  static int sched_params (const ACE_Sched_Params &);

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
  static struct passwd *getpwnam (const char *user);
  static struct passwd *getpwnam_r (const char *name,
				    struct passwd *pwent,
                                    char *buffer,
				    int buflen);
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

  // = A set of wrappers for regular expressions.
  static char *compile (const char *instring,
			char *expbuf, 
			char *endbuf); 
  static int step (const char *str,
		   char *expbuf);

  // = A set of wrappers for non-UNICODE string operations.
  static int strcasecmp (const char *s,
			 const char *t);
  static char *strcat (char *s,
		       const char *t);
  static char *strchr (const char *s,
		       int c);
  static char *strrchr (const char *s,
			int c);
  static int strcmp (const char *s,
		     const char *t);
  static char *strcpy (char *s,
		       const char *t);
  static size_t strspn(const char *s1,
		       const char *s2);
  static char *strstr (const char *s,
		       const char *t);
  static char *strdup (const char *s);
  static size_t strlen (const char *s);
  static int strncmp (const char *s,
		      const char *t,
		      size_t len);
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
  static char *strsplit_r (char *s,
                           const char *token,
                           char *&next_start);
  static long strtol (const char *s,
		      char **ptr,
		      int base);
  static unsigned long strtoul(const char *s,
                               char **ptr,
                               int base);

#if defined (ACE_HAS_UNICODE)
  // = A set of wrappers for UNICODE string operations.
  static wchar_t *strcat (wchar_t *s,
			  const wchar_t *t);
  static wchar_t *strchr (const wchar_t *s,
			  wint_t c);
  static wchar_t *strrchr (const wchar_t *s,
			   wint_t c);
  static int strcmp (const wchar_t *s,
		     const wchar_t *t);
  static wchar_t *strcpy (wchar_t *s,
			  const wchar_t *t);
  static size_t strlen (const wchar_t *s);
  static int strncmp (const wchar_t *s,
		      const wchar_t *t,
		      size_t len);
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
  //static int isspace (wint_t c);

#if defined (ACE_WIN32)
  static wchar_t *strstr (const wchar_t *s,
			  const wchar_t *t);
  static wchar_t *strdup (const wchar_t *s);
  static int sprintf (wchar_t *buf,
		      const wchar_t *format,
		      ...);
  static int sprintf (wchar_t *buf,
		      const char *format,
		      ...);
  static int vsprintf (wchar_t *buffer,
		       const wchar_t *format,
		       va_list argptr);

  static int access (const wchar_t *path,
		     int amode);
  static FILE *fopen (const wchar_t *filename,
		      const wchar_t *mode);
  static int stat (const wchar_t *file,
		   struct stat *);
  static wchar_t *getenv (const wchar_t *symbol);
  static int system (const wchar_t *s);
  static int hostname (wchar_t *name,
		       size_t maxnamelen);
  static ACE_HANDLE open (const wchar_t *filename,
			  int mode,
			  int perms = 0,
			  LPSECURITY_ATTRIBUTES sa = 0);
  static int unlink (const wchar_t *path);
  static ACE_SHLIB_HANDLE dlopen (ACE_WIDE_DL_TYPE filename,
				  
				  int mode = ACE_DEFAULT_SHLIB_MODE);
  static wchar_t *mktemp (wchar_t *t);
  static int mkdir (const wchar_t *path,
		    mode_t mode = ACE_DEFAULT_DIR_PERMS);
  static int chdir (const wchar_t *path);

#endif /* ACE_WIN32 */
#endif /* ACE_HAS_UNICODE */

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

#if 0
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
#endif /* 0 */

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
  static int thr_getspecific (ACE_thread_key_t key,
			      void **data);
  static int thr_keyfree (ACE_thread_key_t key);
  static int thr_key_detach (void *inst);
#if defined (ACE_HAS_THR_C_DEST)
  static int thr_keycreate (ACE_thread_key_t *key,
			    ACE_THR_C_DEST,
			    void *inst = 0);
#else
  static int thr_keycreate (ACE_thread_key_t *key,
			    ACE_THR_DEST,
			    void *inst = 0);
#endif /* ACE_HAS_THR_C_DEST */
  static int thr_key_used (ACE_thread_key_t key);
  static size_t thr_min_stack (void);
  static int thr_setconcurrency (int hint);
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

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0) && defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS)
  static int netdb_acquire (void);
  static int netdb_release (void);
#endif /* defined (ACE_MT_SAFE) && ACE_LACKS_NETDB_REENTRANT_FUNCTIONS */
};

#if defined (ACE_LACKS_TIMEDWAIT_PROTOTYPES)
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
extern "C" ssize_t readv_timedwait (ACE_HANDLE handle,
				    struct iovec *iov,
				    int iovcnt,
				    struct timespec* timeout);
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
extern "C" ssize_t writev_timedwait (ACE_HANDLE handle,
				     ACE_WRITEV_TYPE *iov,
				     int iovcnt,
				     struct timespec *timeout);
#endif /* ACE_LACKS_TIMEDWAIT_PROTOTYPES */

# if defined (ACE_HAS_TSS_EMULATION)
    // Allow config.h to set the default number of thread keys.
#   if !defined (ACE_DEFAULT_THREAD_KEYS)
#     define ACE_DEFAULT_THREAD_KEYS 64
#   endif /* ! ACE_DEFAULT_THREAD_KEYS */

class ACE_TSS_Emulation
  // = TITLE
  //     Thread-specific storage emulation.
  //
  // = DESCRIPTION
  //     This provides a thread-specific storage implementation.
  //     It is intended for use on platforms that don't have a
  //     native TSS, or have a TSS with limitations such as the
  //     number of keys or lack of support for removing keys.
{
public:
  typedef void (*ACE_TSS_DESTRUCTOR)(void *value) /* throw () */;

  static ACE_thread_key_t total_keys ();
  // Returns the total number of keys allocated so far.

  static ACE_thread_key_t next_key ();
  // Returns the next available key, or (key_t) -1 if none are available.

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

  static void tss_allocate ();
  // Allocates a TSS array for the current thread.

  static void tss_deallocate ();
  // Deallocates the current thread's TSS array.

private:
  enum { ACE_TSS_THREAD_KEYS_MAX = ACE_DEFAULT_THREAD_KEYS };
  // Maximum number of TSS keys allowed over the life of the program.

  // Global TSS structures.
  static ACE_thread_key_t total_keys_;
  // Always contains the value of the next key to be allocated.

  static ACE_TSS_DESTRUCTOR tss_destructor_ [ACE_TSS_THREAD_KEYS_MAX];
  // Array of thread exit hooks (TSS destructors) that are called for each
  // key (that has one) when the thread exits.

  static void **&tss_base ();
  // Location of current thread's TSS array.

#if ! defined (VXWORKS)
  // This implementation only works if ::thr_self () returns a small
  // integer.  It is intended for use in testing on Solaris only.
  enum { ACE_TSS_THREADS_MAX = 1024 };

public:
  static void **tss_collection_ [ACE_TSS_THREADS_MAX];
  // Array, indexed by thread handle, of TSS object arrays.  Those
  // are indexed by key.  This is for testing only.
private:

#if 0 // not implemented yet . . .
  // Rely on native thread specific storage for the implementation,
  // but just use one key.

  static ACE_thread_key_t native_tss_key_;
#endif /* 0 */
#endif /* ! VXWORKS */
};

#else   /* ! ACE_HAS_TSS_EMULATION */
# if defined (TLS_MINIMUM_AVAILABLE)
#   // WIN32 platforms define TLS_MINIMUM_AVAILABLE natively.
#   define ACE_DEFAULT_THREAD_KEYS TLS_MINIMUM_AVAILABLE
# endif /* TSL_MINIMUM_AVAILABLE */
#endif /* ACE_HAS_TSS_EMULATION */

// A useful abstraction for expressions involving operator new since
// we can change memory allocation error handling policies (e.g.,
// depending on whether ANSI/ISO exception handling semantics are
// being used). 

#define ACE_NEW_RETURN(POINTER,CONSTRUCTOR,RET_VAL) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL; } \
   } while (0)
#define ACE_NEW(POINTER,CONSTRUCTOR) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { errno = ENOMEM; return; } \
   } while (0)
#define ACE_ALLOCATOR_RETURN(POINTER,ALLOCATOR,RET_VAL) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL; } \
   } while (0)
#define ACE_ALLOCATOR(POINTER,ALLOCATOR) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; return; } \
   } while (0)

// Some more useful abstration for expressions involving 
// ACE_Allocator.malloc ().  The difference between these macro with
// ACE_ALLOCATOR is that we provide construction also.
#define ACE_NEW_MALLOC_RETURN(POINTER,ALLOCATOR,CONSTRUCTOR,RET_VAL) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL;} \
     else { new (POINTER) CONSTRUCTOR; } \
   } while (0)
#define ACE_NEW_MALLOC(POINTER,ALLOCATOR,CONSTRUCTOR) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; return;} \
     else { new (POINTER) CONSTRUCTOR; } \
   } while (0)
#define ACE_DES_FREE(POINTER,DEALLOCATOR,CLASS) \
   do { POINTER->CLASS::~CLASS (); DEALLOCATOR (POINTER); } while (0)

#if defined (ACE_HAS_BROKEN_EXPLICIT_TEMPLATE_DESTRUCTOR)
#define ACE_DES_FREE_TEMPLATE(POINTER,DEALLOCATOR,T_CLASS,T_PARAMETER) \
   do { POINTER->T_CLASS ## T_PARAMETER::~T_CLASS (); \
        DEALLOCATOR (POINTER); \
      } while (0)
#else
#define ACE_DES_FREE_TEMPLATE(POINTER,DEALLOCATOR,T_CLASS,T_PARAMETER) \
   do { POINTER->T_CLASS ## T_PARAMETER::~T_CLASS ## T_PARAMETER (); \
        DEALLOCATOR (POINTER); \
      } while (0)
#endif /* ACE_HAS_BROKEN_EXPLICIT_TEMPLATE_DESTRUCTOR */

#if defined (ACE_HAS_SIGNAL_SAFE_OS_CALLS)
// The following two macros ensure that system calls are properly
// restarted (if necessary) when interrupts occur.
#define ACE_OSCALL(OP,TYPE,FAILVALUE,RESULT) \
  do \
    RESULT = (TYPE) OP; \
  while (RESULT == FAILVALUE && errno == EINTR && ACE_LOG_MSG->restart ())
#define ACE_OSCALL_RETURN(OP,TYPE,FAILVALUE) \
  do { \
    TYPE ace_result_; \
    do \
      ace_result_ = (TYPE) OP; \
    while (ace_result_ == FAILVALUE && errno == EINTR && ACE_LOG_MSG->restart ()); \
    return ace_result_; \
  } while (0)
#elif defined (ACE_WIN32)
#define ACE_OSCALL_RETURN(X,TYPE,FAILVALUE) \
  do { \
    TYPE ace_result_ = (TYPE) X; \
    if (ace_result_ == FAILVALUE) \
      errno = ::GetLastError (); \
    return ace_result_; \
  } while (0)
#define ACE_OSCALL(X,TYPE,FAILVALUE,RESULT) \
  do { \
    RESULT = (TYPE) X; \
    if (RESULT == FAILVALUE) \
      errno = ::GetLastError (); \
  } while (0)
#else
#define ACE_OSCALL_RETURN(OP,TYPE,FAILVALUE) do { TYPE ace_result_ = FAILVALUE; ace_result_ = ace_result_; return OP; } while (0)
#define ACE_OSCALL(OP,TYPE,FAILVALUE,RESULT) do { RESULT = (TYPE) OP; } while (0)
#endif /* ACE_HAS_SIGNAL_SAFE_OS_CALLS */

#if !defined (ACE_DEFAULT_MUTEX_A)
#define ACE_DEFAULT_MUTEX_A "ACE_MUTEX"
#endif /* ACE_DEFAULT_MUTEX_A */

#if !defined (ACE_MAIN)
#define ACE_MAIN main
#endif /* ! ACE_MAIN */

#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
// Rename "main ()" on platforms that don't allow it to be called "main ()".
// Also, create an ACE_Object_Manager static instance in "main ()".
#include "ace/Object_Manager.h"

#define main \
ace_main_i (int, char *[]);                      /* forward declaration */ \
int \
ACE_MAIN (int argc, char *argv[])   /* user's entry point, e.g., "main" */ \
{ \
  ACE_Object_Manager ace_object_manager;        /* has program lifetime */ \
  return ace_main_i (argc, argv);         /* what the user calls "main" */ \
} \
int \
ace_main_i
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER */

#if defined (UNICODE)

#if !defined (ACE_DEFAULT_NAMESPACE_DIR)
#define ACE_DEFAULT_NAMESPACE_DIR ACE_DEFAULT_NAMESPACE_DIR_W
#endif /* ACE_DEFAULT_NAMESPACE_DIR */
#if !defined (ACE_DEFAULT_LOCALNAME)
#define ACE_DEFAULT_LOCALNAME ACE_DEFAULT_LOCALNAME_W 
#endif /* ACE_DEFAULT_LOCALNAME */
#if !defined (ACE_DEFAULT_GLOBALNAME)
#define ACE_DEFAULT_GLOBALNAME ACE_DEFAULT_GLOBALNAME_W
#endif /* ACE_DEFAULT_GLOBALNAME */
#if !defined (ACE_DIRECTORY_SEPARATOR_STR)
#define ACE_DIRECTORY_SEPARATOR_STR ACE_DIRECTORY_SEPARATOR_STR_W
#endif /* ACE_DIRECTORY_SEPARATOR_STR */
#if !defined (ACE_DIRECTORY_SEPARATOR_CHAR)
#define ACE_DIRECTORY_SEPARATOR_CHAR ACE_DIRECTORY_SEPARATOR_CHAR_W
#endif /* ACE_DIRECTORY_SEPARATOR_CHAR */
#if !defined (ACE_PLATFORM)
#define ACE_PLATFORM ACE_PLATFORM_W
#endif /* ACE_PLATFORM */
#if !defined (ACE_PLATFORM_EXE_SUFFIX)
#define ACE_PLATFORM_EXE_SUFFIX ACE_PLATFORM_EXE_SUFFIX_W
#endif /* ACE_PLATFORM_EXE_SUFFIX */
#if !defined (ACE_DEFAULT_MUTEX_W)
#define ACE_DEFAULT_MUTEX_W L"ACE_MUTEX"
#endif /* ACE_DEFAULT_MUTEX_W */
#if !defined (ACE_DEFAULT_MUTEX)
#define ACE_DEFAULT_MUTEX ACE_DEFAULT_MUTEX_W
#endif /* ACE_DEFAULT_MUTEX */

#else

#if !defined (ACE_DEFAULT_NAMESPACE_DIR)
#define ACE_DEFAULT_NAMESPACE_DIR ACE_DEFAULT_NAMESPACE_DIR_A
#endif /* ACE_DEFAULT_NAMESPACE_DIR */
#if !defined (ACE_DEFAULT_LOCALNAME)
#define ACE_DEFAULT_LOCALNAME ACE_DEFAULT_LOCALNAME_A
#endif /* ACE_DEFAULT_LOCALNAME */
#if !defined (ACE_DEFAULT_GLOBALNAME)
#define ACE_DEFAULT_GLOBALNAME ACE_DEFAULT_GLOBALNAME_A
#endif /* ACE_DEFAULT_GLOBALNAME */
#if !defined (ACE_DIRECTORY_SEPARATOR_STR)
#define ACE_DIRECTORY_SEPARATOR_STR ACE_DIRECTORY_SEPARATOR_STR_A
#endif /* ACE_DIRECTORY_SEPARATOR_STR */
#if !defined (ACE_DIRECTORY_SEPARATOR_CHAR)
#define ACE_DIRECTORY_SEPARATOR_CHAR ACE_DIRECTORY_SEPARATOR_CHAR_A
#endif /* ACE_DIRECTORY_SEPARATOR_CHAR */
#if !defined (ACE_PLATFORM)
#define ACE_PLATFORM ACE_PLATFORM_A
#endif /* ACE_PLATFORM */
#if !defined (ACE_PLATFORM_EXE_SUFFIX)
#define ACE_PLATFORM_EXE_SUFFIX ACE_PLATFORM_EXE_SUFFIX_A
#endif /* ACE_PLATFORM_EXE_SUFFIX */
#if !defined (ACE_DEFAULT_MUTEX_W)
#define ACE_DEFAULT_MUTEX_W "ACE_MUTEX"
#endif /* ACE_DEFAULT_MUTEX_W */
#if !defined (ACE_DEFAULT_MUTEX)
#define ACE_DEFAULT_MUTEX ACE_DEFAULT_MUTEX_A
#endif /* ACE_DEFAULT_MUTEX */
#endif /* UNICODE */

#if defined (ACE_HAS_INLINED_OSCALLS)
#if defined (ACE_INLINE)
#undef ACE_INLINE
#endif /* ACE_INLINE */
#define ACE_INLINE inline
#include "ace/OS.i"
#endif /* ACE_HAS_INLINED_OSCALLS */

#include "ace/Trace.h"

// These need to come here to avoid problems with circular dependencies.
#include "ace/Log_Msg.h"

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

#if defined (UNICODE)
#include "ace/Auto_Ptr.h"
#include "ace/SString.h"
#define ACE_WIDE_STRING(ASCII_STRING) \
ACE_WString (ASCII_STRING).fast_rep ()
#define ACE_MULTIBYTE_STRING(WIDE_STRING) \
ACE_Auto_Basic_Array_Ptr<char> (ACE_WString (WIDE_STRING).char_rep ()).get ()
#define ACE_TEXT_STRING ACE_WString
#else
#define ACE_WIDE_STRING(ASCII_STRING) ASCII_STRING
#define ACE_MULTIBYTE_STRING(WIDE_STRING) WIDE_STRING
#define ACE_TEXT_STRING ACE_CString
#endif /* UNICODE */

#endif  /* ACE_OS_H */
