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
#include "ace/config-win32.h"
