/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SunOS
// 5.[56] platforms using the SunC++ 4.x compiler. This works with the
// Orbix 2.x CORBA IDL compiler.

#ifndef ACE_CONFIG_ORBIX_H
#define ACE_CONFIG_ORBIX_H

#include "ace/config-sunos5.5-sunc++-4.x.h"

// Platform contains the Orbix CORBA implementation.
#define ACE_HAS_ORBIX 1

// Platform contains the multi-threaded Orbix CORBA implementation.
#define ACE_HAS_MT_ORBIX 1

#endif /* ACE_CONFIG_ORBIX_H */
