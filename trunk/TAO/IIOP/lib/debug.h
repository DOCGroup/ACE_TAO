// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    debug.h
//
// = DESCRIPTION
//     debug/trace support.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
// 
// ============================================================================

#if !defined (TAO_DEBUG_H)
#define	TAO_DEBUG_H

#ifndef	DECLARED_STRERROR
extern "C" char *strerror (int);
#endif /* DECLARED_STRERROR */

// These are global to simplify is use by other code, very much in
// particular by getopt and related argument-parsing code
//
// THREADING NOTE: don't set them except in an unthreaded environment
// such as process initialization.  They're treated as immutable.

// @@ Please replace these globally -- they should be called
// "tao_debug_level", etc. to protect the namespace.

// 0 to ??; higher == more
extern u_int TAO_debug_level;	

// set by getopt
extern char *TAO_debug_filter;

// These are just simple 0, 1, and 2 argument messages that will
// appear when debugging's enabled, regardless of category.  They also
// just compile out painlessly.

#if defined (DEBUG)
#include <stdio.h>
#include <errno.h>
#include <string.h>

// 1, 2, 3 argument messages -- generic
#define dmsg(s)		{ if (TAO_debug_level) dmsg_v (s); }
#define dmsg1(s,a1)	{ if (TAO_debug_level) dmsg_v (s, a1); }
#define dmsg2(s,a1,a2)	{ if (TAO_debug_level) dmsg_v (s, a1, a2); }

// dump CORBA_Exception, if any, with id tag
#define dexc(env,s)	{ if (TAO_debug_level && env.exception ()) \
				_dmsg_x (env, s); }

// dump POSIX error indication, if any, with ID tag
#define dperror(str)	{ if (TAO_debug_level) dmsg_v ("%s: %s", \
				str, strerror (errno)); }

// dump socket error indication, if any, with ID tag
#if defined (_WINSOCKAPI_)
#define	dsockerr(s)	{ if (TAO_debug_level) dmsg_v ("%s:  winsock error %d", \
				s, WSAGetLastError()); }
#else
#define	dsockerr(s)	dperror(s)
#endif /* _WINSOCKAPI_ */

#else	/* !DEBUG */
#define dmsg(s)		{ }
#define dmsg1(s,a1)	{ }
#define dmsg2(s,a1,a2)	{ }

#define dexc(env, s)	{ }
#define	dperror(s)	{ }
#define	dsockerr(s)	{ }
#endif	/* DEBUG */

// These don't compile out; you must #ifdef them.  This is done
// intentionally since CPP macros have severe limits, and varargs _(or
// lack thereof) is one of them.

#if defined (DEBUG)

// This is like an fprintf statement except the filter is a set of
// characters (string).  If TAO_debug_level is nonzero and any characters
// in that string are in the "TAO_debug_filter" string, the message is
// then printed.  Assign thosee characters as needed.

extern void _EXPFUNC dmsg_filter (const char *_FAR filter,
				  const char *_FAR fmt, ...);

// Filter according to TAO_debug_level instead of category.  (For speed,
// test against TAO_debug_level directly.)

extern void _EXPFUNC dmsg_filter (u_int level, 
				  const char *_FAR fmt, ...);

// General varargs debug message printer, no filtering

extern void _EXPFUNC dmsg_v (const char *_FAR fmt, ...);
extern void _EXPFUNC _dmsg_x (CORBA_Environment _FAR &env,
				const char *_FAR info);
extern void _EXPFUNC dmsg_opaque (char *_FAR label,
				  u_char *_FAR buffer,
				  u_long len);
extern void _EXPFUNC dmsg_opaque_full (char *_FAR label,
				       const u_char *_FAR buffer,
				       u_long len);
#endif /* DEBUG */

#endif /* TAO_DEBUG_H */
