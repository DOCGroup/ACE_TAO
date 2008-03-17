//* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for VxWorks
// Based on ACE_VXWORKS it will select the correct config file

#ifndef ACE_CONFIG_VXWORKS_H
#define ACE_CONFIG_VXWORKS_H
#include /**/ "ace/pre.h"

#if !defined (ACE_VXWORKS)
# error You must define ACE_VXWORKS
#endif /* ! ACE_VXWORKS */

#if (ACE_VXWORKS == 0x551)
# include "ace/config-vxworks5.x.h"
#elif (ACE_VXWORKS == 0x620)
# include "ace/config-vxworks6.2.h"
#elif (ACE_VXWORKS == 0x630)
# include "ace/config-vxworks6.3.h"
#elif (ACE_VXWORKS == 0x640)
# include "ace/config-vxworks6.4.h"
#elif (ACE_VXWORKS == 0x650)
# include "ace/config-vxworks6.5.h"
#elif (ACE_VXWORKS == 0x660)
# include "ace/config-vxworks6.6.h"
#else
#error Unknown VxWorks version
#endif

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_VXWORKS_H */

