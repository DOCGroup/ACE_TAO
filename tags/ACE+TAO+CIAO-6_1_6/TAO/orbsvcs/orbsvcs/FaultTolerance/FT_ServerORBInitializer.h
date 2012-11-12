// -*- C++ -*-

//=============================================================================
/**
 *  @file     FT_ServerORBInitializer.h
 *
 *  $Id$
 *
 *  @author  Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================
#ifndef TAO_FT_SERVERORB_INITIALIZER_H
#define TAO_FT_SERVERORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "orbsvcs/FaultTolerance/FT_ServerORB_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// RTCORBA ORB initializer.
class TAO_FT_ServerORBInitializer
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual ::CORBA::LocalObject
{
public:

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

private:

  /// Register FTCORBA policy factories.
  void register_policy_factories (PortableInterceptor::ORBInitInfo_ptr info);

  /// Register the necessary interceptors.
  void register_server_request_interceptors (PortableInterceptor::ORBInitInfo_ptr info);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_FT_SERVERORB_INITIALIZER_H */
