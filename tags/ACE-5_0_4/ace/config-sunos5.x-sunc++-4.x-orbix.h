/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SunOS
// 5.[56] platforms using the SunC++ 4.x compiler. This works with the
// Orbix 2.x CORBA IDL compiler.

#ifndef ACE_CONFIG_ORBIX_H
#define ACE_CONFIG_ORBIX_H

// Pick up correct platform definition file based on compiler's predefined
// macros.
#if defined (__SunOS_5_5) || defined (__SunOS_5_5_1)
#  include "ace/config-sunos5.5.h"
#elif defined (__SunOS_5_6)
#  include "ace/config-sunos5.6.h"
#else
#  error "Need a new platform extension in config-sunos5.x-sunc++-4.x-orbix.h"
#endif /* __SunOS_* */

// Platform contains the Orbix CORBA implementation.
#define ACE_HAS_ORBIX 1

// Platform contains the multi-threaded Orbix CORBA implementation, unless
// overridden by site config.
#if !defined (ACE_HAS_MT_ORBIX)
#  define ACE_HAS_MT_ORBIX 1
#endif /* ACE_HAS_MT_ORBIX */

#endif /* ACE_CONFIG_ORBIX_H */
