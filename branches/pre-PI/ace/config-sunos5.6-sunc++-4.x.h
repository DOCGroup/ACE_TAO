/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SunOS 5.6
// platforms using the SunC++ 4.x compiler.

#if !defined (ACE_CONFIG_H)

// ACE_CONFIG_H is defined by the following #included header.

// At this point, there's no difference between our SunOS 5.5 and 5.6
// configurations.
#if defined (__SUNPRO_CC)
# include "ace/config-sunos5.5-sunc++-4.x.h"
#elif defined (__GNUG__)
# include "ace/config-sunos5.5-g++.h"
#else  /* ! __SUNPRO_CC__ && ! __GNUG__ */
# error unsupported compiler in ace/config-sunos5.6.h
#endif /* ! __SUNPRO_CC__ && ! __GNUG__ */

// Additional SunOS 5.6 capabilities . . .
#define ACE_HAS_AIO_CALLS
#endif /* ACE_CONFIG_H */
