/* -*- C++ -*- */
// $Id$

// This configuration file is for use with the PharLap Realtime ETS Kernel.
// It has been tested with PharLap TNT Embedded ToolSuite version 9.1.

#define ACE_HAS_PHARLAP
// Some features are only available with the Realtime edition of ETS.
// Assume that if using ACE, the realtime version is also being used, but
// allow it to be turned off as well.
#ifndef ACE_HAS_PHARLAP_RT
#  define ACE_HAS_PHARLAP_RT
#else
#  if (ACE_HAS_PHARLAP_RT == 0)
#    undef ACE_HAS_PHARLAP_RT
#  endif
#endif

#define ACE_HAS_WINNT4 0
#define ACE_HAS_WINSOCK2 0
#define ACE_LACKS_MMAP
#define ACE_LACKS_MPROTECT
#define ACE_LACKS_MSYNC

// There's no host table, by default. So using "localhost" won't work.
// If your system does have the ability to use "localhost" and you want to,
// define it before including this file.
#if !defined (ACE_LOCALHOST)
# define ACE_LOCALHOST "127.0.0.1"
#endif /* ACE_LOCALHOST */

// Don't know how to get the page size at execution time. This is most likely
// the correct value.
#define ACE_PAGE_SIZE 4096

#include /**/ "ace/config-win32.h"
#include <embkern.h>
