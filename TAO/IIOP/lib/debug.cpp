// @ (#)debug.cpp	1.3 95/10/02
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:		Simple debug/trace support
//
// THREADING NOTE: the global values here  (debug_{level,filter,stream)
// are assumed to be modified "safely", e.g. in the main thread as
// part of process initialization.  They are treated as immutable
// values through all of this debuging package.
//
// XXX on Windows, make it always use OutputDebugString () instead of stdio

#include	<assert.h>
#include	<limits.h>
#include	<string.h>
#include	<orb.h>

#include "debug.h"

#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

#if 0
#if	unix
#include <sys/types.h>

#elif	defined (_WIN32)
#	include	<windows.h>

#endif
#endif

#ifndef	_POSIX_THREADS		// _POSIX_THREAD_SAFE_FUNCTIONS implied
#define	flockfile (f)
#define funlockfile (f)
#endif	// _POSIX_THREADS

u_int TAO_debug_level	= 0;
char *TAO_debug_filter	= "l";
static FILE *debug_stream = stderr;

// The rest of this file is not needed without -DDEBUG, and unless the
// vfprintf () call is available it can't work.
//
// NOTE:  some older platforms have "_doprnt" that provides much the
// same functionality ... this could be modified to use that routine
// where it's available.
//
#if	defined (DEBUG) && defined (HAVE_VPRINTF)

//
// Support for prefixing debug messages with process ID and, if threaded,
// the thread ID.  This lets messages from different sources be safely
// disentangled, even though they're interspersed in the output stream.
//
static pid_t my_pid;

#if defined (unix) || defined (VXWORKS)

#ifdef	_POSIX_THREADS
//
// Use POSIX initialization support to initialize just once.
//
static pthread_once_t debug_init = PTHREAD_ONCE_INIT;

#define setup () pthread_once (&debug_init, setup_once)

static void
setup_once ()
{
  my_pid = ACE_OS::getpid ();
}

static void
emit_prefix (FILE *stream)
{
  pthread_t self = pthread_self ();

  ACE_OS::fprintf (stream, "p%ld t%ld:  ", (long) my_pid, (long) self);
}

#else	// !defined (_POSIX_THREADS)

// Without threads, guard initialization so it can be repeated,
// and don't emit the thread ID in the messages.

static void
setup (void)
{
  if (my_pid == 0) 
    {
      my_pid = ACE_OS::getpid ();
      // other setup goes here
    }
}

#define	emit_prefix (stream) fprintf (stream, "p%ld:  ", (long) my_pid)
#endif	// !_POSIX_THREADS

#elif	defined (_WIN32)

//
// Not all implementations of Win32 have threads, but in any case
// this code doesn't yet support Win32 threads.
//
static void
setup (void)
{
  if (my_pid == 0) 
    my_pid = GetCurrentProcessId ();
  // other setup goes here
}

#define	emit_prefix (stream) fprintf (stream, "p%ld:  ", my_pid)

#else

#	error "unknown OS platform"
#endif	// OS-specific initialization


void _EXPFUNC
dmsg_filter (const char *_FAR categories,
	     const char *_FAR fmt,
	     ...)
{
  const char *cp;

  if (!categories || !debug_filter)
    return;

  if (*debug_filter != '*') 
    {		// filter with "*" --> all pass
      for (cp = categories; *cp; cp++)
	if (strchr (debug_filter, *cp) != 0)
	  break;

      if (!*cp)
	return;
    }

  va_list ap;

  setup ();
  flockfile (debug_stream);
  emit_prefix (debug_stream);

  switch (*cp) 
    {			// standard categories
    case 'l':		ACE_OS::fprintf (debug_stream, " (LEAK) "); break;
    }

  va_start (ap, fmt);
  vfprintf (debug_stream, fmt, ap);
  va_end (ap);
  if (strchr (fmt, '\n') == 0)
    ACE_OS::fprintf (debug_stream, "\n");
  funlockfile (debug_stream);

#if	defined (_WIN32)
  OutputDebugString ("called dmsg_filter\n");	// experimental
#endif
}

void _EXPFUNC
dmsg_filter (u_int level, 
	     const char *_FAR fmt, 
	     ...)
{
  if (level > debug_level)
    return;

  va_list ap;

  setup ();
  flockfile (debug_stream);
  emit_prefix (debug_stream);
  va_start (ap, fmt);
  vfprintf (debug_stream, fmt, ap);
  va_end (ap);
  if (strchr (fmt, '\n') == 0)
    ACE_OS::fprintf (debug_stream, "\n");
  funlockfile (debug_stream);

#if	defined (_WIN32)
  OutputDebugString ("called dmsg_filter\n");	// experimental
#endif
}

void _EXPFUNC
dmsg_v (const char *_FAR fmt, 
	...)
{
  va_list ap;

  setup ();
  flockfile (debug_stream);
  emit_prefix (debug_stream);
  va_start (ap, fmt);
  vfprintf (debug_stream, fmt, ap);
  va_end (ap);
  if (strchr (fmt, '\n') == 0)
    ACE_OS::fprintf (debug_stream, "\n");
  funlockfile (debug_stream);

#if	defined (_WIN32)
  OutputDebugString ("called dmsg_v\n");	// experimental
#endif
}

void _EXPFUNC
_dmsg_x (CORBA_Environment _FAR	&env,
	 const char *_FAR info)
{
  const CORBA_Exception	*ex = env.exception ();

  setup ();
  flockfile (debug_stream);
  emit_prefix (debug_stream);
  ACE_OS::fprintf (debug_stream, "exception '%s' at '%s'\n", ex->id (), info);
  if (env.exception_type () == SYSTEM_EXCEPTION) 
    {
      CORBA_SystemException *sysex = (CORBA_SystemException *) ex;

      emit_prefix (debug_stream);
      ACE_OS::fprintf (debug_stream, "minor %#lx, completion %#lx\n",
		       sysex->minor (), (long) sysex->completion ());
    }
  funlockfile (debug_stream);

#if	defined (_WIN32)
  OutputDebugString ("called _dmsg_x\n");	// experimental
#endif
}

void _EXPFUNC
dmsg_opaque (char *_FAR label,
	     u_char *_FAR buffer,
	     u_long len)
{
  setup ();
  flockfile (debug_stream);
  emit_prefix (debug_stream);
  ACE_OS::fprintf (debug_stream, "%s ", label);

  if (len == 0 || !buffer)
    ACE_OS::fprintf (debug_stream, " (empty)");
  else if (len > UINT_MAX)
    ACE_OS::fprintf (debug_stream, "Oversized opaque data:  %ld bytes", len);
  else 
    {
      u_int i;

      for (i = 0; i < len; i++)
	if (!isprint (buffer [i]))
	  break;
	
      if (i < len) 
	{
	  if (len >= 20)
	    ACE_OS::fprintf (debug_stream, "%ld bytes binary data", len);
	  else 
	    {
	      ACE_OS::fprintf (debug_stream, "binary data {%2X", buffer [0]);
	      for (i = 1; i < len; i++)
		ACE_OS::fprintf (debug_stream, ", %2x", buffer [i]);
	      ACE_OS::fprintf (debug_stream, "}");
	    }
	} 
      else 
	{
	  if (len >= 50)
	    ACE_OS::fprintf (debug_stream, "%ld bytes string data", len);
	  else
	    ACE_OS::fprintf (debug_stream, "string data { \"%.*s\" }",
 (int) len, buffer);
	}
    }
  ACE_OS::fprintf (debug_stream, "\n");
  funlockfile (debug_stream);

#if	defined (_WIN32)
  OutputDebugString ("called dmsg_opaque\n");	// experimental
#endif
}

void _EXPFUNC
dmsg_opaque_full (char *_FAR label,
		  const u_char *_FAR buffer,
		  u_long len)
{
  setup ();
  flockfile (debug_stream);

  emit_prefix (debug_stream);
  ACE_OS::fprintf (debug_stream, "%s ", label);

  if (len == 0 || !buffer)
    ACE_OS::fprintf (debug_stream, " (empty)");
  else 
    {
      u_int i;

      for (i = 0; i < len; i++)
	{
	  if (i == 0)
	    ACE_OS::fprintf (debug_stream, "\nhex:   ");
	  else if ((i % 32) == 0)
	    ACE_OS::fprintf (debug_stream, "\n       ");
	  else if ((i % 4) == 0)
	    ACE_OS::fprintf (debug_stream, " ");
	  ACE_OS::fprintf (debug_stream, "%02x", buffer[i]);
	}

      for (i = 0; i < len; i++)
	{
	  if (i == 0)
	    ACE_OS::fprintf (debug_stream, "\nchars: ");
	  else if ((i % 32) == 0)
	    ACE_OS::fprintf (debug_stream, "\n       ");
	  else if ((i % 4) == 0)
	    ACE_OS::fprintf (debug_stream, " ");
	  ACE_OS::fprintf (debug_stream, "%c ",
			   (isprint (buffer[i]) ? buffer[i] : '?'));

	}

      ACE_OS::fprintf (debug_stream, "\n");
    }
  ACE_OS::fprintf (debug_stream, "\n");
  funlockfile (debug_stream);

#if	defined (_WIN32)
  OutputDebugString ("called dmsg_opaque_full\n");	// experimental
#endif
}

#endif	// DEBUG && HAVE_VPRINTF
