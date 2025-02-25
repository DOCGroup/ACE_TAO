// -*- C++ -*-

#ifndef ACE_CONFIG_INTEGRITY178_H
#define ACE_CONFIG_INTEGRITY178_H

/*
 * This config.h file is for version 5.0.0 of the
 * INTEGRITY-178B RTOS from Green Hills Software
 * http://www.ghs.com/products/rtos/integrity.html
 */

#include "config-integrity-common.h"

// The macro INTEGRITY178B is used in places where the existing INTEGRITY or ghs macros
// are not sufficient due to differences between this version of INTEGRITY-178 and the
// INTEGRITY version that was used by this version of ACE. For example, INTEGRITY-178
// does not have AllocateNullConsoleDescriptor function (see OS_NS_fcntl.cpp).
// NOTE: if INTEGRITY178B is defined, then INTEGRITY is also defined.
#define INTEGRITY178B

// Exclude unavailable functions in libraries such as libposix_178b_be.a or file systems libraries.
// Newly introduced macros w.r.t. this ACE version are annotated in the comments.
#if defined ACE_LACKS_POSIX
# define ACE_LACKS_FSTAT // new
# define ACE_LACKS_LSTAT
# define ACE_LACKS_STAT // new
# define ACE_LACKS_UMASK
# define ACE_LACKS_GETTIMEOFDAY // new
# define ACE_LACKS_ALARM // new
# define ACE_LACKS_DUP // new
# define ACE_LACKS_DUP2
# define ACE_LACKS_FTRUNCATE // new
# define ACE_LACKS_GETHOSTNAME // new
# define ACE_LACKS_SLEEP // new
# define ACE_LACKS_SELECT // new
# define ACE_LACKS_READV
# define ACE_LACKS_WRITEV
# define ACE_LACKS_KILL // new
# define ACE_LACKS_SIGEMPTYSET // new
# define ACE_LACKS_SIGFILLSET // new
# define ACE_LACKS_SIGISMEMBER // new
# define ACE_LACKS_SIGADDSET // new
# define ACE_LACKS_SIGDELSET // new
# define ACE_LACKS_SIGPROCMASK // new
# define ACE_LACKS_TIME // new
# define ACE_LACKS_MMAP
# define ACE_LACKS_SETEGID
# define ACE_LACKS_SETUID
# define ACE_LACKS_SETEUID
# define ACE_LACKS_SETGID
# define ACE_LACKS_GETEUID
# define ACE_LACKS_GETUID
# define ACE_LACKS_GETEGID
# define ACE_LACKS_GETGID
# define ACE_LACKS_TEMPNAM /* believe it or not, this is ANSI C */
# define ACE_LACKS_STRPTIME
# define ACE_LACKS_SYSTEM
# define ACE_LACKS_WAIT
# define ACE_LACKS_WAITPID
# define ACE_LACKS_EXEC
# define ACE_LACKS_FORK
# define ACE_LACKS_MKFIFO
# define ACE_LACKS_MKTEMP
# define ACE_LACKS_MKSTEMP
# define ACE_LACKS_MPROTECT
# define ACE_LACKS_SEEKDIR
# define ACE_LACKS_MSYNC
# define ACE_LACKS_UNAME
# define ACE_LACKS_ISATTY
# define ACE_LACKS_STRCASECMP
# define ACE_LACKS_TRUNCATE
#endif

// Support turning off network utility functions from GHNet-178's libnet.a.
// However, socket functions from GHNet-178's libsocket.a must be available.
#if defined ACE_NO_GHNET178_LIBNET
/***** Network utility functions *****/
# define ACE_LACKS_GETADDRINFO
# define ACE_LACKS_GAI_STRERROR
# define ACE_LACKS_GETNAMEINFO
# define ACE_LACKS_GETSERVBYNAME
# define ACE_LACKS_IF_NAMEINDEX
# define ACE_LACKS_IF_NAMETOINDEX
# define ACE_LACKS_GETHOSTBYADDR // new
# define ACE_LACKS_GETHOSTBYNAME // new
# define ACE_LACKS_GETPROTOBYNAME // new
# define ACE_LACKS_GETPROTOBYNUMBER // new
# define ACE_LACKS_GETHOSTBYADDR_R // new
# define ACE_LACKS_GETHOSTBYNAME_R // new
# define ACE_LACKS_GETPROTOBYNAME_R // new
# define ACE_LACKS_GETPROTOBYNUMBER_R // new
# define ACE_LACKS_INET_ADDR // new
# define ACE_LACKS_INET_NTOA // new
# define ACE_LACKS_INET_ATON
# define ACE_LACKS_INET_NTOP
# define ACE_LACKS_INET_PTON

/***** Socket functions *****/
# define ACE_LACKS_FCNTL
# define ACE_LACKS_IOCTL // new
# define ACE_LACKS_RECVMSG
# define ACE_LACKS_SENDMSG
# define ACE_LACKS_SHUTDOWN
# define ACE_LACKS_SOCKETPAIR
#endif

/****** Posix Defines *****/
#define ACE_LACKS_DLFCN_H
#define ACE_LACKS_REGEX_H

/****** C++17 Defines *****/
#define ACE_LACKS_STD_WSTRING
#define ACE_LACKS_STD_STRING_VIEW
#define ACE_FALLTHROUGH

// INTEGRITY has this concept of thread-specific storage in its native API.
// It's mentioned as Task-specific data in its document.
// INTEGRITY-178B does not seem to have thread-specific storage. TSS is just
// mentioned in some places in the manuals but there are no APIs.
//#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_HAS_TSS_EMULATION

#endif /* ACE_CONFIG_INTEGRITY178_H */
