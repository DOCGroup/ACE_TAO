/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SunOS 5.5
// platforms using the SunC++ 4.x compiler. This works with the
// Orbix 2.0 CORBA IDL compiler.

#include "ace/config-sunos5.5-sunc++-4.x.h"

#if !defined (ACE_CONFIG_ORBIX_H)
#define ACE_CONFIG_ORBIX_H

// Platform contains the Orbix CORBA implementation.
#define ACE_HAS_ORBIX

// Platform contains the multi-threaded Orbix CORBA implementation.
//#define ACE_HAS_MT_ORBIX

#endif /* ACE_CONFIG_ORBIX_H */
