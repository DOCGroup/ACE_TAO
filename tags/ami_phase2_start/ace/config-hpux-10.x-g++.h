/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for HP
// platforms running HP/UX 10.x using G++.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#include "ace/config-g++-common.h"

// These are apparantly some things which are special to g++ on HP?  They are
// compiler-related settings, but not in config-g++-common.h

#define ACE_HAS_BROKEN_CONVERSIONS
#define ACE_HAS_STRING_CLASS
// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T
#define _CLOCKID_T

#include "ace/config-hpux-10.x.h"

#endif /* ACE_CONFIG_H */
