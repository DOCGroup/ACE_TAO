/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for the
// Digital UNIX V4.0a and later platforms.  It relies on
// config-osf1-4.0.h, and adds deltas for newer platforms.

#ifndef ACE_CONFIG_OSF1_H
#define ACE_CONFIG_OSF1_H

#if defined (DIGITAL_UNIX)
#  include "ace/config-osf1-4.0.h"
#  if DIGITAL_UNIX >= 5
#    define _LIBC_POLLUTION_H_
#    define ACE_LACKS_SYSTIME_H
#  endif /* DIGITAL_UNIX >= 5 */
#else  /* ! DIGITAL_UNIX */
#  include "ace/config-osf1-3.2.h"
#endif /* ! DIGITAL_UNIX */

#endif /* ACE_CONFIG_OSF1_H */
