// -*- C++ -*-

//=============================================================================
/**
 *  @file LifespanPolicyFactory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_LOADABLE_LIFESPAN_POLICY_H
#define TAO_LOADABLE_LIFESPAN_POLICY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "PolicyFactory.h"
#include "PortableServerC.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CORBA
{
  class PolicyError;
}

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export LifespanPolicyFactory :
       public virtual PolicyFactory
    {
    public:
      virtual ~LifespanPolicyFactory (void);

      /// Return the cached policy type for this policy.
      virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

      /// Returns the scope at which this policy can be applied. See orbconf.h.
      virtual TAO_Policy_Scope _tao_scope (void) const;

      /// Create a new lifespan policy
      /**
       * @note If all the compilers supported covariant return types we could
       * change this to a CORBA::Policy_ptr create() call, which is defined
       * as pure virtual in the base. This is something for the future.
       */
      ::PortableServer::LifespanPolicy_ptr create (
        ::PortableServer::LifespanPolicyValue value);

      /// Create a new lifespan policy
      ::PortableServer::LifespanPolicy_ptr create (
        const CORBA::Any &value ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::PolicyError));
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, LifespanPolicyFactory)
    ACE_FACTORY_DECLARE (TAO_PortableServer, LifespanPolicyFactory)
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_LOADABLE_LIFESPAN_POLICY_H */
