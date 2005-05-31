// -*- C++ -*-

// ===================================================================
/**
 *  @file   PolicyFactory_Registry.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
// ===================================================================

#ifndef TAO_PI_CLIENT_H
#define TAO_PI_CLIENT_H

#include /**/ "ace/pre.h"

#include "pi_client_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

#include "ace/Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/CORBA_macros.h"
#include "tao/PolicyFactory_Registry_Adapter.h"

//-- Forward Declarations--
namespace CORBA
{
  typedef ULong PolicyType;

  class Policy;
  typedef Policy *Policy_ptr;

  class Any;

  class Environment;
}

namespace PortableInterceptor
{
  class PolicyFactory;
  typedef PolicyFactory *PolicyFactory_ptr;
}

/**
 * @class TAO_PI_Client_Init
 *
 */
class TAO_PI_CLIENT_Export TAO_PI_Client_Init
{
public:

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_PI_Client_Initializer =
  &TAO_PI_Client_Init::Initializer;

#else

static int
TAO_Requires_PI_Client_Initializer =
  TAO_PI_Client_Init::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */


#include /**/ "ace/post.h"

#endif  /* TAO_PI_CLIENT_H */
