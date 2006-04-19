/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for the
// Digital UNIX V4.0a and later platforms.  It relies on
// config-osf1-4.0.h, and adds deltas for newer platforms.

#ifndef ACE_CONFIG_TRU64_H
#define ACE_CONFIG_TRU64_H
#include /**/ "ace/pre.h"

#if defined (DIGITAL_UNIX)
# include "ace/config-osf1-4.0.h"
# define ACE_HAS_NONSTATIC_OBJECT_MANAGER
# if DIGITAL_UNIX >= 0x500
    // It's possible that snprintf() is available on earlier Tru64 versions,
    // but this is the only way it's been tested.  Steve Huston, 22-Jan-2003
    // It is not on 4.0F.  Rich Seibel, 6 Feb 2003
#   ifndef ACE_HAS_SNPRINTF
#     define ACE_HAS_SNPRINTF
#   endif /* ACE_HAS_SNPRINTF */
#   define ACE_HAS_XPG4_MULTIBYTE_CHAR 1
    /*Platform has support for
    multi-byte character support
    compliant with the XPG4
    Worldwide Portability
    Interface wide-character
    classification.*/
# endif /* DIGITAL_UNIX >= 0x500 */
# if DIGITAL_UNIX >= 0x40E
#   define ACE_LACKS_STDINT_H
# endif /* DIGITAL_UNIX >= 0x40E */
#endif /* ! DIGITAL_UNIX */

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_TRU64_H */
