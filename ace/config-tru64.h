/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for the
// Digital UNIX V4.0a and later platforms.  It relies on
// config-osf1-4.0.h, and adds deltas for newer platforms.

#ifndef ACE_CONFIG_TRU64_H
#define ACE_CONFIG_TRU64_H
#include "ace/pre.h"

#if defined (DIGITAL_UNIX)
# include "ace/config-osf1-4.0.h"
# if DIGITAL_UNIX >= 0x40E
#   define ACE_LACKS_SYSTIME_H
# endif /* DIGITAL_UNIX >= 0x40E */
# if DIGITAL_UNIX >= 0x500
#   define _LIBC_POLLUTION_H_
# endif /* DIGITAL_UNIX >= 0x500 */
# if DIGITAL_UNIX >= 0x510
#   define ACE_LACKS_CUSERID
# endif /* DIGITAL_UNIX >= 0x510 */
#else  /* ! DIGITAL_UNIX */
# include "ace/config-osf1-3.2.h"
#endif /* ! DIGITAL_UNIX */

#include "ace/post.h"
#endif /* ACE_CONFIG_TRU64_H */
