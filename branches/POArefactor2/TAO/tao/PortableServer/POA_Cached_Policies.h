// -*- C++ -*-

//=============================================================================
/**
 *  @file POA_Cached_Policies.h
 *
 *  $Id$
 *
 *   POA
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================

#ifndef TAO_POA_CACHED_POLICIES_H
#define TAO_POA_CACHED_POLICIES_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ThreadPolicyC.h"
#include "LifespanPolicyC.h"
#include "IdUniquenessPolicyC.h"
#include "IdAssignmentPolicyC.h"
#include "ImplicitActivationPolicyC.h"
#include "ServantRetentionPolicyC.h"
#include "RequestProcessingPolicyC.h"

// Forward references.
class TAO_POA_Policy_Set;

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Cached_Policies
    {
    public:

      enum PriorityModel
      {
        CLIENT_PROPAGATED,
        SERVER_DECLARED,
        NOT_SPECIFIED
      };

      Cached_Policies ();

      ~Cached_Policies (void);

      /// Update the cached policy values.
      void update (TAO_POA_Policy_Set &policy_set
                   ACE_ENV_ARG_DECL);

      /**
       * @name Accessor methods to cached values.
       */
      //@{
      PortableServer::ThreadPolicyValue thread (void) const;
      PortableServer::LifespanPolicyValue lifespan (void) const;
      PortableServer::IdUniquenessPolicyValue id_uniqueness (void) const;
      PortableServer::IdAssignmentPolicyValue id_assignment (void) const;
      PortableServer::ImplicitActivationPolicyValue implicit_activation (void) const;
      PortableServer::ServantRetentionPolicyValue servant_retention (void) const;
      PortableServer::RequestProcessingPolicyValue request_processing (void) const;
      PriorityModel priority_model (void) const;
      CORBA::Short server_priority (void) const;

      void priority_model (PriorityModel priority_model);
      void server_priority (CORBA::Short priority);
      void implicit_activation (PortableServer::ImplicitActivationPolicyValue value);
      //@}

    protected:

      /// Helper method to update a particular policy.
      void update_policy (const CORBA::Policy_ptr policy
                          ACE_ENV_ARG_DECL);

      PortableServer::ThreadPolicyValue thread_;

      PortableServer::LifespanPolicyValue lifespan_;

      PortableServer::IdUniquenessPolicyValue id_uniqueness_;

      PortableServer::IdAssignmentPolicyValue id_assignment_;

      PortableServer::ImplicitActivationPolicyValue implicit_activation_;

      PortableServer::ServantRetentionPolicyValue servant_retention_;

      PortableServer::RequestProcessingPolicyValue request_processing_;

      PriorityModel priority_model_;

      CORBA::Short server_priority_;
    };
  }
}

#if defined (__ACE_INLINE__)
# include "POA_Cached_Policies.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_POA_CACHED_POLICIES_H */
