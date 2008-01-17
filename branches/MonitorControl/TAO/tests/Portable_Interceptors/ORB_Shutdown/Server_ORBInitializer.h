// -*- C++ -*-

//=============================================================================
/**
 * @file Server_ORBInitializer.h
 *
 * $Id$
 *
 * Implementation header for the PortableInterceptor ORB::shutdown()
 * test server side ORB initializer.
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_SERVER_ORB_INITIALIZER_H
#define TAO_SERVER_ORB_INITIALIZER_H

#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/PI.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

// Forward declaration.
class Server_Request_Interceptor;

/// Server side ORB initializer.
class Server_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  Server_ORBInitializer (bool & destroy_called);

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

private:

  /// Flag that states whether this interceptor's @c destroy() method
  /// was called.
  bool & destroy_called_;

};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#endif /* TAO_SERVER_ORB_INITIALIZER_H */
