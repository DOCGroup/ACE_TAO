// -*- C++ -*-

//=============================================================================
/**
 *  @file     miopconf.h
 *
 *  $Id$
 *
 *  Build configuration file.
 *
 *  @author  Copyright 1995 by Sun Microsystems, Inc.
 *  @author  Chris Cleeland
 *  @author  Douglas C. Schmidt
 */
//=============================================================================

#ifndef TAO_MIOPCONF_H
#define TAO_MIOPCONF_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// This is the version of the MIOP spec that TAO supports. The
// exact usage of the version has not been emphasized. But TAO should
// get a TaggedComponents for a group with this version number. So, for
// the present, we do a sanity check for our version and raise an error on
// a mismatch.

#if !defined (TAO_DEF_MIOP_MAJOR)
#define TAO_DEF_MIOP_MAJOR 1
#endif /* TAO_DEF_MIOP_MAJOR */
#if !defined (TAO_DEF_MIOP_MINOR)
#define TAO_DEF_MIOP_MINOR 0
#endif /* TAO_DEF_MIOP_MINOR */

#include "ace/post.h"
#endif /*TAO_MIOPCONF_H*/
