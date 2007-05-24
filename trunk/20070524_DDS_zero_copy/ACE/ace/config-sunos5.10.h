/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SunOS 5.10
// (Solaris 10) platforms using the SunC++ 5.x (Sun Studio 8-10), or g++
// compilers.

#ifndef ACE_CONFIG_H

// ACE_CONFIG_H is defined by one of the following #included headers.

// #include the SunOS 5.9 config, then add any SunOS 5.10 updates below.
#include "ace/config-sunos5.9.h"

#define ACE_HAS_SCANDIR

// Solaris 10 offers a useable alphasort() unlike previous Solaris versions.
#if defined (ACE_LACKS_ALPHASORT)
#  undef ACE_LACKS_ALPHASORT
#endif

// Solaris 10 offers a useable log2() unlike previous Solaris versions.
#if defined (ACE_LACKS_LOG2)
#  undef ACE_LACKS_LOG2
#endif

#endif /* ACE_CONFIG_H */
