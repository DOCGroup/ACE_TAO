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

#include "ace/pre.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "portableserver_export.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

/// Policy factory for all PortableServer related policies.
class TAO_PortableServer_Export TAO_PortableServer_PolicyFactory
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

#include "ace/post.h"

#endif /* TAO_PORTABLESERVER_POLICY_FACTORY_H */
