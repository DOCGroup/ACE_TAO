// -*- C++ -*-

//=============================================================================
/**
 *  @file     ftconf.h
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

#ifndef TAO_FTCONF_H
#define TAO_FTCONF_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// This is the version of the FT_CORBA spec that TAO supports. The
// exact use of this version has not been emphasised. But TAO would
// get TaggedComponents for a group with version number. So, for the
// present we will have this here and do a sanity check for our
// supported version and the one we receive -- raise an error if
// necessary.

#if !defined (TAO_DEF_FT_CORBA_MAJOR)
#define TAO_DEF_FT_CORBA_MAJOR 1
#endif /* TAO_DEF_FT_CORBA_MAJOR */
#if !defined (TAO_DEF_FT_CORBA_MINOR)
#define TAO_DEF_FT_CORBA_MINOR 0
#endif /* TAO_DEF_FT_CORBA_MINOR */

#include /**/ "ace/post.h"
#endif /*TAO_FTCONF_H*/
