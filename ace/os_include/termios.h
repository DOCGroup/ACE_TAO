/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    termios.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_TERMIOS_H
# define ACE_OS_INCLUDE_TERMIOS_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_TERMIOS_H)
#   include /**/ <termios.h>
# endif /* !ACE_LACKS_TERMIOS_H */

// This is sorta interesting.  but termios.h not sys/termios.h should be correct.
# if defined (ACE_NEEDS_TERMIOS_H) // __QNX__
#   include /**/ <termios.h>
# endif /* ACE_NEEDS_TERMIOS_H) */

# if defined (ACE_NEEDS_SYS_MODEM_H) // HPUX
#   include /**/ <sys/modem.h>
# endif /* ACE_NEEDS_SYS_MODEM_H */

#   if !defined (VMIN)
#     define ACE_VMIN 4
#   else
#     define ACE_VMIN VMIN
#   endif /* VMIN */

#   if !defined (VTIME)
#     define ACE_VTIME 5
#   else
#     define ACE_VTIME VTIME
#   endif /* VTIME */

#endif /* ACE_OS_INCLUDE_TERMIOS_H */
