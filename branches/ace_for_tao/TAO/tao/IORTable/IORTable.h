/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    IORTable.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#ifndef TAO_IORTABLE_H
#define TAO_IORTABLE_H

#include /**/ "ace/pre.h"

#include "iortable_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_IORTable_Export TAO_IORTable_Initializer
{
public:
  /// Used to force the initialization of the ORB code.
  static int init (void);
};

static int
TAO_Requires_IORTable_Initializer = TAO_IORTable_Initializer::init ();

#define TAO_IORTABLE_SAFE_INCLUDE
#include "IORTableC.h"
#undef TAO_IORTABLE_SAFE_INCLUDE

#include /**/ "ace/post.h"

#endif /* TAO_IORTABLE_H */
