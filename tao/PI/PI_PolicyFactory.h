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

#include "tao/PI/pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "tao/PI/PI_includeC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Policy factory for all PortableInterceptor related policies.
class TAO_PI_Export TAO_PI_PolicyFactory
  : public virtual PortableInterceptor::PolicyFactory,
    public virtual ::CORBA::LocalObject
{
public:
  CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                   const CORBA::Any & value);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_PI_POLICY_FACTORY_H */
