// -*- C++ -*-
// $Id$

#ifndef TAO_CLIENT_ORB_INITIALIZER_H
#define TAO_CLIENT_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"

#include "lwft_client_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class ForwardingAgent_i;

/// Client side ORB initializer.
class LWFT_Client_Export Client_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual TAO_Local_RefCounted_Object
{
public:

  Client_ORBInitializer (ForwardingAgent_i *agent);

  ~Client_ORBInitializer (void);

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

private:
  ForwardingAgent_i *agent_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_CLIENT_ORB_INITIALIZER_H */
