// $Id$

#ifndef ACE_CONFIG_MIT_PTHREAD_H
#define ACE_CONFIG_MIT_PTHREAD_H
#include "ace/pre.h"

// Platform uses int for select() rather than fd_set.
#if !defined(ACE_HAS_SELECT_H)
#define ACE_HAS_SELECT_H
#endif

// Threads
#define ACE_HAS_THREADS
#if !defined (ACE_MT_SAFE)
        #define ACE_MT_SAFE 1
#endif
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_STD
#define ACE_LACKS_PTHREAD_CANCEL
#define ACE_HAS_PTHREAD_SIGMASK
#define ACE_HAS_SIGWAIT
//#define       ACE_HAS_PTHREAD_YIELD_VOID_PTR
//#define       ACE_HAS_PTHREAD_ATTR_INIT
//#define       ACE_HAS_PTHREAD_ATTR_DESTROY
#define ACE_LACKS_THREAD_PROCESS_SCOPING
//#define ACE_LACKS_THREAD_STACK_ADDR
// If ACE doesn't compile due to the lack of these methods, please
// send email to schmidt@cs.wustl.edu reporting this.
// #define ACE_LACKS_CONDATTR_PSHARED
// #define ACE_LACKS_MUTEXATTR_PSHARED
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SETSCHED

#include <pthread.h>
#if defined(_M_UNIX)
#include <sys/regset.h>
#endif

#define ACE_LACKS_TIMEDWAIT_PROTOTYPES
#define ACE_HAS_RECV_TIMEDWAIT
#define ACE_HAS_RECVFROM_TIMEDWAIT
#define ACE_HAS_RECVMSG_TIMEDWAIT
#define ACE_HAS_SEND_TIMEDWAIT
#define ACE_HAS_SENDTO_TIMEDWAIT
#define ACE_HAS_SENDMSG_TIMEDWAIT
#define ACE_HAS_READ_TIMEDWAIT
#define ACE_HAS_READV_TIMEDWAIT
#define ACE_HAS_WRITE_TIMEDWAIT
#define ACE_HAS_WRITEV_TIMEDWAIT

#include "ace/post.h"
#endif /* ACE_CONFIG_MIT_PTHREAD_H */
