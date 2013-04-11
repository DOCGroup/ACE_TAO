// -*- C++ -*-


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

#include "tao/IORTable/iortable_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IORTable_Export TAO_IORTable_Initializer
{
public:
  /// Used to force the initialization of the ORB code.
  static int init (void);
};

static int
TAO_Requires_IORTable_Initializer = TAO_IORTable_Initializer::init ();

TAO_END_VERSIONED_NAMESPACE_DECL

#define TAO_IORTABLE_SAFE_INCLUDE
#include "tao/IORTable/Locate_ResponseHandler.h"
#include "tao/IORTable/IORTableC.h"
#undef TAO_IORTABLE_SAFE_INCLUDE

#include /**/ "ace/post.h"

#endif /* TAO_IORTABLE_H */
