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
    public virtual ::CORBA::LocalObject
{
public:
  CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                   const CORBA::Any & value);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_POLICY_FACTORY_H */
