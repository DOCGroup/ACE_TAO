/* -*- C++ -*- */
// $Id$

// This configuration file automatically includes the proper
// configurations for IBM's VisualAge C++ compiler on Win32 and AIX.

#ifdef __TOS_WIN__
   #include "ace/config-win32.h"
#elif __TOS_AIX__
   #include "ace/config-aix-4.x.h"
#else
   #include "PLATFORM NOT SPECIFIED"
#endif /* __TOS_WIN__ */
