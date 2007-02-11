// -*- C++ -*-

//=============================================================================
/**
 * @file EndpointPolicy_Factory.h
 *
 * $Id$
 *
 * @author  Yan Dai <dai_y@ociweb.com>
 */
//=============================================================================


#ifndef TAO_ENDPOINTPOLICY_FACTORY_H
#define TAO_ENDPOINTPOLICY_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/EndpointPolicy/EndpointPolicy_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
class TAO_ORB_Core;

/// Policy factory for the endpoint policies.
class TAO_EndpointPolicy_Export TAO_EndpointPolicy_Factory
  : public virtual PortableInterceptor::PolicyFactory,
    public virtual TAO_Local_RefCounted_Object
{
public:

  TAO_EndpointPolicy_Factory (TAO_ORB_Core * orb_core);

  virtual CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                           const CORBA::Any & value
                                           );
private:

  TAO_ORB_Core * orb_core_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_ENDPOINTPOLICY_FACTORY_H */
