// -*- C++ -*-

//=============================================================================
/**
 *  @file Loadable_Id_Uniqueness_Policy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_LOADABLE_ID_UNIQUENESS_POLICY_H
#define TAO_LOADABLE_ID_UNIQUENESS_POLICY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "POA_Policy.h"
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
  class TAO_PortableServer_Export Loadable_Id_Uniqueness_Policy :
     public virtual POA_Policy
  {
  public:
    virtual ~Loadable_Id_Uniqueness_Policy (void);

    /// Return the cached policy type for this policy.
    virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

    /// Returns the scope at which this policy can be applied. See orbconf.h.
    virtual TAO_Policy_Scope _tao_scope (void) const;

    /// Create a new id uniqueness policy
    /**
     * @note If all the compilers supported covariant return types we could
     * change this to a CORBA::Policy_ptr create() call, which is defined
     * as pure virtual in the base. This is something for the future.
     */
    PortableServer::IdUniquenessPolicy_ptr create (
      PortableServer::IdUniquenessPolicyValue value);

    /// Create a new id uniqueness policy
    PortableServer::IdUniquenessPolicy_ptr create (
      const CORBA::Any &value ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError));
  };

  ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, Loadable_Id_Uniqueness_Policy)
  ACE_FACTORY_DECLARE (TAO_PortableServer, Loadable_Id_Uniqueness_Policy)
}

#include /**/ "ace/post.h"
#endif /* TAO_LOADABLE_ID_UNIQUENESS_POLICY_H */
