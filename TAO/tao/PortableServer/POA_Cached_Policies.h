// -*- C++ -*-

//=============================================================================
/**
 *  @file POA_Cached_Policies.h
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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

      enum NetworkPriorityModel
      {
        CLIENT_PROPAGATED_NETWORK_PRIORITY,
        SERVER_DECLARED_NETWORK_PRIORITY,
        NO_NETWORK_PRIORITY
      };

      Cached_Policies ();

      ~Cached_Policies ();

      /// Update the cached policy values.
      void update (TAO_POA_Policy_Set &policy_set);

      /**
       * @name Accessor methods to cached values.
       */
      //@{
      ::PortableServer::ThreadPolicyValue thread () const;
      ::PortableServer::LifespanPolicyValue lifespan () const;
      ::PortableServer::IdUniquenessPolicyValue id_uniqueness () const;
      ::PortableServer::IdAssignmentPolicyValue id_assignment () const;
      ::PortableServer::ImplicitActivationPolicyValue implicit_activation () const;
      ::PortableServer::ServantRetentionPolicyValue servant_retention () const;
      ::PortableServer::RequestProcessingPolicyValue request_processing () const;
      PriorityModel priority_model () const;
      CORBA::Short server_priority () const;

      /// Returns the network priority model that was set using a policy
      /// by the server application.
      ///
      NetworkPriorityModel network_priority_model () const;

      /// Returns the request DiffServ codepoint that was set using a policy
      /// by the server application.
      ///
      CORBA::Long request_diffserv_codepoint () const;

      /// Returns the reply DiffServ codepoint that was set using a policy
      /// by the server application.
      ///
      CORBA::Long reply_diffserv_codepoint () const;

      /// Caches the network priority model that was set using a policy
      /// by the server application.
      ///
      void network_priority_model (
        NetworkPriorityModel network_priority_model);

      /// Caches the request DiffServ codepoint that was set using a policy
      /// by the server application.
      ///
      void request_diffserv_codepoint (CORBA::Long diffserv_codepoint);

      /// Caches the reply DiffServ codepoint that was set using a policy
      /// by the server application.
      ///
      void reply_diffserv_codepoint (CORBA::Long diffserv_codepoint);

      void priority_model (PriorityModel priority_model);
      void server_priority (CORBA::Short priority);
      void implicit_activation (::PortableServer::ImplicitActivationPolicyValue value);
      //@}

    protected:

      /// Helper method to update a particular policy.
      void update_policy (const CORBA::Policy_ptr policy);

      ::PortableServer::ThreadPolicyValue thread_;

      ::PortableServer::LifespanPolicyValue lifespan_;

      ::PortableServer::IdUniquenessPolicyValue id_uniqueness_;

      ::PortableServer::IdAssignmentPolicyValue id_assignment_;

      ::PortableServer::ImplicitActivationPolicyValue implicit_activation_;

      ::PortableServer::ServantRetentionPolicyValue servant_retention_;

      ::PortableServer::RequestProcessingPolicyValue request_processing_;

      PriorityModel priority_model_;

      CORBA::Short server_priority_;

      NetworkPriorityModel network_priority_model_;

      CORBA::Long request_diffserv_codepoint_;

      CORBA::Long reply_diffserv_codepoint_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "POA_Cached_Policies.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_POA_CACHED_POLICIES_H */
