/* -*- C++ -*- */
// $Id$

/* The following configuration file is designed to work for Linux
   platforms using GNU C++ and Xavier Leroy's pthreads package.  For
   more information you should check out his Web site:

   http://pauillac.inria.fr/~xleroy/linuxthreads/

   The version I have installed and working is an RPM*
   based on Xavier's 0.5 release. I don't know where
   the tarball of 0.5 can be found, but I suspect that
   Xavier's site has it...

	* RPM == Redhat Package Management

	My system is a Caldera-based distribution with many upgraded
	packages.  If you don't use RPM, there is a program (rpm2cpio)
	which will extract the files for "normal consumption".

	You may also want to check out the "ACE On Linux" pages at:

		http://users.deltanet.com/users/slg/ACE/

	(They were a little out of date when I last was there
	however.) */

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#include "ace/config-linux-common.h"

#define ACE_HAS_SVR4_DYNAMIC_LINKING
#define ACE_HAS_AUTOMATIC_INIT_FINI

// Yes, we do have threads.
#define ACE_HAS_THREADS
// And they're even POSIX pthreads (MIT implementation)
#define ACE_HAS_PTHREADS
// ... and the final standard even!
#define	ACE_HAS_PTHREADS_STD

#define ACE_HAS_PTHREAD_SIGMASK			// JCEJ 12/19/96

#if !defined (ACE_MT_SAFE)
	#define ACE_MT_SAFE 1				// JCEJ 12/22/96	#1
#endif
#define ACE_HAS_THREAD_SPECIFIC_STORAGE		// jcej 12/22/96	#2
#define PTHREAD_MIN_PRIORITY		0	// JCEJ 12/22/96	#3
#if defined(ACE_HAS_PTHREAD_SIGMASK)
#  define PTHREAD_MAX_PRIORITY		99	// CJC  02/11/97
#else
#  define PTHREAD_MAX_PRIORITY          32      // JCEJ 12/22/96	#3
#endif

#define ACE_LACKS_THREAD_STACK_ADDR		// JCEJ 12/17/96
#define ACE_LACKS_THREAD_STACK_SIZE		// JCEJ 12/17/96

#define	ACE_LACKS_RWLOCK_T			// JCEJ 12/23/96	#1
#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS	// JCEJ 1/7-8/96

#define ACE_HAS_SIGWAIT

#if defined(__GLIBC__)
// Platform supports reentrant functions (i.e., all the POSIX *_r
// functions).
#define ACE_HAS_REENTRANT_FUNCTIONS
// getprotobyname_r have a different signature!
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS
// uses ctime_r & asctime_r with only two parameters vs. three
#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
#endif

#include /**/ <pthread.h>

#endif /* ACE_CONFIG_H */
