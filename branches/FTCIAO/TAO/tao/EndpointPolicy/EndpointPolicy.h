// -*- C++ -*-

//=============================================================================
/**
 *  @file   EndpointPolicy.h
 *
 *  $Id$
 *
 *  @author  Yan Dai <dai_y@ociweb.com>
 */
//=============================================================================


#ifndef TAO_ENDPOINTPOLICY_H
#define TAO_ENDPOINTPOLICY_H

#include /**/ "ace/pre.h"

#include "tao/EndpointPolicy/EndpointPolicy_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_EndpointPolicy_Export TAO_EndpointPolicy_Initializer
  : public ACE_Service_Object
{
public:
  /// Used to force the initialization of the ORB code.
  static int static_init (void);

  /// Initialize the Endpoint Policy loader hooks.
  virtual int init (int argc, ACE_TCHAR* []);
};

static int
TAO_Requires_EndpointPolicy_Initializer = TAO_EndpointPolicy_Initializer::static_init ();



ACE_STATIC_SVC_DECLARE_EXPORT (TAO_EndpointPolicy, TAO_EndpointPolicy_Initializer)
ACE_FACTORY_DECLARE (TAO_EndpointPolicy, TAO_EndpointPolicy_Initializer)

TAO_END_VERSIONED_NAMESPACE_DECL
#include /**/ "ace/post.h"

#define TAO_ENDPOINTPOLICY_SAFE_INCLUDE
#include "tao/EndpointPolicy/EndpointPolicyC.h"
#include "tao/EndpointPolicy/EndpointPolicyA.h"
#include "tao/IOPC.h"
#include "tao/AnyTypeCode/Any.h"
#undef TAO_ENDPOINTPOLICY_SAFE_INCLUDE

#endif /* TAO_ENDPOINTPOLICY_H */
