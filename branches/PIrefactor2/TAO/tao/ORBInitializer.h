// -*- C++ -*-

// ===================================================================
/**
 *  @file   ORBInitializer_Registry.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_ORB_INITIALIZER_H
#define TAO_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"
#include "Environment.h"
#include "default_environment.h"

namespace PortableInterceptor
{
  class ORBInitializer;
  typedef ORBInitializer *ORBInitializer_ptr;

  /// Register an ORBInitializer with the global ORBInitializer
  /// table.
  TAO_Export void register_orb_initializer (
      ORBInitializer_ptr init
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    );
}

#include /**/ "ace/post.h"

#endif /* TAO_ORB_INITIALIZER_H */
