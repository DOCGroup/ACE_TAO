// -*- C++ -*-

//=============================================================================
/**
 * @file PortableServer_PolicyFactory.h
 *
 * $Id$
 *
 * @author Irfan Pyarali <irfan@oomworks.com>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_POLICY_FACTORY_H
#define TAO_PORTABLESERVER_POLICY_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Policy factory for all PortableServer related policies.
class TAO_PortableServer_PolicyFactory
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

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_POLICY_FACTORY_H */
