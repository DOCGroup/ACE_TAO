// -*- C++ -*-

//=============================================================================
/**
 *  @file Active_Policy_Strategies.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_ACTIVE_POLICY_STRATEGIES_H
#define TAO_PORTABLESERVER_ACTIVE_POLICY_STRATEGIES_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"
#include <memory>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ThreadPolicyC.h"
#include "tao/PortableServer/IdAssignmentPolicyC.h"
#include "tao/PortableServer/IdUniquenessPolicyC.h"
#include "tao/PortableServer/ServantRetentionPolicyC.h"
#include "tao/PortableServer/LifespanPolicyC.h"
#include "tao/PortableServer/ImplicitActivationPolicyC.h"
#include "tao/PortableServer/RequestProcessingPolicyC.h"

#include "tao/PortableServer/ThreadStrategy.h"
#include "tao/PortableServer/IdAssignmentStrategy.h"
#include "tao/PortableServer/IdUniquenessStrategy.h"
#include "tao/PortableServer/ImplicitActivationStrategy.h"
#include "tao/PortableServer/LifespanStrategy.h"
#include "tao/PortableServer/RequestProcessingStrategy.h"
#include "tao/PortableServer/ServantRetentionStrategy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Root_POA;

namespace TAO
{
  namespace Portable_Server
  {
    class Cached_Policies;

    /**
     * This class stores the active policy strategies used for a certain POA.
     */
    class Active_Policy_Strategies
    {
    public:
      Active_Policy_Strategies () = default;

      void update (Cached_Policies &policies, TAO_Root_POA* poa);

      void cleanup ();

      ThreadStrategy *thread_strategy () const;

      RequestProcessingStrategy *request_processing_strategy () const;

      IdAssignmentStrategy *id_assignment_strategy () const;

      IdUniquenessStrategy *id_uniqueness_strategy () const;

      LifespanStrategy *lifespan_strategy () const;

      ImplicitActivationStrategy *implicit_activation_strategy () const;

      ServantRetentionStrategy *servant_retention_strategy () const;

    private:
      void create (::PortableServer::ThreadPolicyValue value);
      void create (::PortableServer::IdAssignmentPolicyValue value);
      void create (::PortableServer::IdUniquenessPolicyValue value);
      void create (::PortableServer::ServantRetentionPolicyValue value);
      void create (::PortableServer::LifespanPolicyValue value);
      void create (::PortableServer::ImplicitActivationPolicyValue value);
      void create (::PortableServer::RequestProcessingPolicyValue value, ::PortableServer::ServantRetentionPolicyValue srvalue);

    private:
      std::unique_ptr<ThreadStrategy> thread_strategy_ {};
      std::unique_ptr<IdAssignmentStrategy> id_assignment_strategy_ {};
      std::unique_ptr<IdUniquenessStrategy> id_uniqueness_strategy_ {};
      std::unique_ptr<ServantRetentionStrategy> servant_retention_strategy_ {};
      std::unique_ptr<LifespanStrategy> lifespan_strategy_ {};
      std::unique_ptr<ImplicitActivationStrategy> implicit_activation_strategy_ {};
      std::unique_ptr<RequestProcessingStrategy> request_processing_strategy_ {};
    };

    /**
     * This class quards the cleanup of strategies if something went wrong
     * in the code that called Active_Policy_Strategies::update().
     */
    class Active_Policy_Strategies_Cleanup_Guard
    {
    public:
      Active_Policy_Strategies_Cleanup_Guard () = delete;
      Active_Policy_Strategies_Cleanup_Guard (Active_Policy_Strategies *p);
      ~Active_Policy_Strategies_Cleanup_Guard ();
      Active_Policy_Strategies *_retn ();

    private:
      Active_Policy_Strategies *ptr_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/Active_Policy_Strategies.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_ACTIVE_POLICY_STRATEGIES_H */
