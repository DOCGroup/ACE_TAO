// -*- C++ -*-

//=============================================================================
/**
 *  @file   DIOP.h
 *
 *  $Id$
 *
 *  This file is used to force DIOP to be linked in and initialized in
 *  static builds.  The application has to include this file in at least
 *  one source file so that a static class instance can be initialized
 *  and force the rest of DIOP into the executable.
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_DIOP_H
#define TAO_DIOP_H
#include "ace/pre.h"
#include "diop_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_DIOP_Export TAO_DIOP_Protocol_Factory_Adder
{
public:
  TAO_DIOP_Protocol_Factory_Adder (void);
};

// Static instances that will force everything in DIOP to be pulled in.
static TAO_DIOP_Protocol_Factory_Adder TAO_DIOP_adder;

#include "ace/post.h"
#endif /* TAO_DIOP_FACTORY_H */

