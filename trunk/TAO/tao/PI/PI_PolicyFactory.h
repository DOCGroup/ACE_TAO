// -*- C++ -*-

//=============================================================================
/**
 * @file PI_PolicyFactory.h
 *
 * $Id$
 *
 * @author Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_PI_POLICY_FACTORY_H
#define TAO_PI_POLICY_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "PolicyFactoryC.h"

/// Policy factory for all PortableInterceptor related policies.
class TAO_Export TAO_PI_PolicyFactory
  : public virtual PortableInterceptor::PolicyFactory,
    public virtual TAO_Local_RefCounted_Object
{
public:
  CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                   const CORBA::Any & value
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CORBA::PolicyError));
};

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_PI_POLICY_FACTORY_H */
