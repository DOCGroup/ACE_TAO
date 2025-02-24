// -*- C++ -*-

#ifndef ACE_CONFIG_INTEGRITY_H
#define ACE_CONFIG_INTEGRITY_H

/*
 * This config.h file is for version 11.4.x of the
 * INTEGRITY RTOS from Green Hills Software
 * http://www.ghs.com/products/rtos/integrity.html
 */

// This is crafted to remove dependency on the POSIX calls provided
// by libraries available in INTEGRITY but not in INTEGRITY-178.
// Some network utility and socket functions are also turned off
// (see the sections near the end of the file).

/***** TAO STUFF ****/
// This is not ORBExpress, but define the macro so that Integrity's headers
// don't clobber UserException (see INTEGRITY-include/INTEGRITY_error.h).
#define ORBEXPRESS

#include "config-integrity-common.h"

#define ACE_HAS_TSS_EMULATION
// Even though INTEGRITY has support for thread-specific storage, we are
// not using it since we are imitating the behavior of INTEGRITY-178 which
// does not have thread-specific storage.
//#define ACE_HAS_THREAD_SPECIFIC_STORAGE

/***** Operating System Defines *****/
# define ACE_LACKS_SIGEMPTYSET // new
# define ACE_LACKS_SIGFILLSET // new
# define ACE_LACKS_SIGISMEMBER // new
# define ACE_LACKS_SIGADDSET // new
# define ACE_LACKS_SIGDELSET // new
# define ACE_LACKS_SIGPROCMASK

/***** ANSI defines *****/
#define ACE_LACKS_TEMPNAM /* believe it or not, this is ANSI C */
#define ACE_LACKS_SENDMSG

#define ACE_LACKS_STRPTIME
#define ACE_LACKS_SYSTEM

/****** Posix Defines *****/
#define ACE_LACKS_KILL

#define ACE_LACKS_WAIT
#define ACE_LACKS_WAITPID
#define ACE_LACKS_EXEC
#define ACE_LACKS_FORK
#define ACE_LACKS_MKFIFO
#define ACE_LACKS_MKTEMP
#define ACE_LACKS_MKSTEMP
#define ACE_LACKS_MMAP
#define ACE_LACKS_MPROTECT
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SEMBUF_T
#define ACE_LACKS_SEEKDIR
#define ACE_LACKS_MSYNC
#define ACE_LACKS_SETGID
#define ACE_LACKS_UNAME
#define ACE_LACKS_ISATTY
#define ACE_LACKS_STRCASECMP
#define ACE_LACKS_TRUNCATE

#define ACE_HAS_WCHAR

#define NSIG (SIGRTMAX+1)
#define ACE_NEEDS_HUGE_THREAD_STACKSIZE 0x5000
#define fileno(file) ((file)->io_channel) //Hack to get Svc_Conf_l.cpp compiled

#define ACE_HAS_VOIDPTR_GETTIMEOFDAY

#define ACE_IOCTL_TYPE_ARG2 unsigned long

// When config.h defines ACE_NO_GHS_LIBIO, the functions provied by the
// INTEGRITY-178 Low-Level I/O Library (libio_178b_be.a) are not called by ACE.
#ifdef ACE_NO_GHS_LIBIO
# define ACE_LACKS_FCNTL
#endif

#endif /* ACE_CONFIG_INTEGRITY_H */
