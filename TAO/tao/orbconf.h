// This may look like C, but it's really -*- C++ -*-
//
// @ (#) $Id$
// Copyright 1995 by Sun Microsystems, Inc
//
// Build configuration file for the Inter-ORB Engine, and application
// level code using it via <corba.hh>
//

#if !defined (TAO_ORB_CONFIG_H)
#define	TAO_ORB_CONFIG_H

#include "ace/OS.h"

// BC++ seems to have a different convention for detecting Win32 than
// VC++.

#if defined (__WIN32__)
#	define _WIN32
#endif	/* BC++ convention */

/* Define if you have the <widec.h> header file.  */
#if !defined (linux) && !defined (_WIN32) && !defined (VXWORKS)
#  define HAVE_WIDEC_H 1
#endif /* !defined (linux) && !defined (_WIN32) && !defined (VXWORKS) */

// Define to `int' if <sys/types.h> doesn't define.
/* #undef pid_t */

// Define if your processor stores words with the most significant
// byte first (like Motorola and SPARC, unlike Intel and VAX).
#if defined (i386) || defined (_M_IX86) || defined (vax)
#else
#  define TAO_WORDS_BIGENDIAN 1
#endif

// The number of bytes in an int. 
#	if defined (_WIN32) || defined (linux) || defined (VXWORKS)
#		define	SIZEOF_INT	4
#	else	// Win16
#		define	SIZEOF_INT	2
#	endif	// Win32/Win16

// The number of bytes in a long.  
#define SIZEOF_LONG 4

// The number of bytes in a void *.  
#define SIZEOF_VOID_P 4

// The number of bytes in a long long.  
#if !defined (_WIN32) && !defined (VXWORKS)
#define SIZEOF_LONG_LONG 8
#endif /* !defined (_WIN32) && !defined (VXWORKS) */

// The number of bytes in a long double.  
#if defined (linux)
#  define SIZEOF_LONG_DOUBLE 12
#elif defined (_WIN32) || defined (VXWORKS) || defined (M_UNIX)
#  define SIZEOF_LONG_DOUBLE 8
#else
#  define SIZEOF_LONG_DOUBLE 16
#endif /* linux */

// The number of bytes in a bool.  
#define SIZEOF_BOOL 0

// The number of bytes in a wchar_t.
#if defined (VXWORKS)
#define SIZEOF_WCHAR_T 1
#else
#define SIZEOF_WCHAR_T 2
#endif /* VXWORKS */

// Define as the return type of signal handlers (int or void).
#define RETSIGTYPE void

// Define if you don't have vprintf but do have _doprnt.  
/* #undef HAVE_DOPRNT */

// Define if you have the vprintf function.  
#define HAVE_VPRINTF 1

// Define if you have strerror declared.  
#define DECLARED_STRERROR 1

// With ACE, we always have this :-)
#define HAVE_GETTIMEOFDAY

#if defined (minor)
	// namespace pollution that's common on older UNIXes,
    // XXX this can't go here, "autoconf" wants to own #undef
/* #	undef	minor */
#endif	/* minor */

// For Win16, near/far pointers reflect same/other segment addressing.

#if defined (unix) || defined (_WIN32) || defined (VXWORKS)
#	define	_FAR
#endif /* defined (unix) || defined (_WIN32) || defined (VXWORKS) */

// Assume DOS/Windows if "configure" didn't get run.

#if !defined (SIZEOF_LONG) || defined (_WIN32)
//#	if defined (_WIN32) || defined (linux)
//#		define	SIZEOF_INT	4
//#	else	// Win16
//#		define	SIZEOF_INT	2
//#	endif	// Win32/Win16

#	define DECLARED_ACCEPT
#	define DECLARED_BIND
#	define DECLARED_CONNECT
#	define DECLARED_GETHOSTNAME
#	define DECLARED_H_ERRNO
#	define DECLARED_LISTEN
#	define DECLARED_LISTEN
#	define DECLARED_SELECT
#	define DECLARED_SETSOCKOPT
#	define DECLARED_SHUTDOWN
#	define DECLARED_SOCKET

#	define HAVE_STRDUP

#  if !defined (linux) && !defined (_WIN32)
typedef	unsigned long pid_t;
#  endif /* !defined (linux) && !defined (_WIN32) */

// "C4355: 'this' : used in base member initializer list"
#  pragma warning (disable:4355) /* disable C4355 warning */

#endif /* !defined (SIZEOF_LONG) || defined (_WIN32) */

#endif	/* TAO_ORB_CONFIG_H */
