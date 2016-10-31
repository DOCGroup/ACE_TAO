/* -*- C++ -*- */
// The following configuration file is designed to work for SunOS 5.11
// (Solaris 11) platforms using the SunC++ 5.x (Sun Studio 10-12), or g++
// compilers.

#ifndef ACE_CONFIG_H

// ACE_CONFIG_H is defined by one of the following #included headers.

// #include the SunOS 5.10 config, then add any SunOS 5.11 updates below.
#include "ace/config-sunos5.10.h"

// *TODO*: find out how far this also hold for previous solaris releases
#define ACE_HAS_SOLARIS11_GETPWNAM_R

#endif /* ACE_CONFIG_H */
