// -*- C++ -*-

//=============================================================================
/**
 *  @file Active_Policy_Strategies.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_ACTIVE_POLICY_STRATEGIES_H
#define TAO_PORTABLESERVER_ACTIVE_POLICY_STRATEGIES_H
#include /**/ "ace/pre.h"

#include "Thread_Strategy.h"
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_POA;

namespace TAO
{
  namespace Portable_Server
  {
    class Cached_Policies;
    class Thread_Strategy;
    class Request_Processing_Strategy;
    class Id_Assignment_Strategy;
    class Lifespan_Strategy;
    class Id_Uniqueness_Strategy;
    class Activation_Strategy;
    class ServantRetentionStrategy;

    /**
     * This class stores the active policy strategies used for a certain POA.
     */
    class TAO_PortableServer_Export Active_Policy_Strategies
    {
    public:
      Active_Policy_Strategies();

      void update (Cached_Policies &policies,
                   TAO_POA* poa
                   ACE_ENV_ARG_DECL);

      Thread_Strategy *thread_strategy (void) const;

      Request_Processing_Strategy *request_processing_strategy (void) const;

      Id_Assignment_Strategy *id_assignment_strategy (void) const;

      Id_Uniqueness_Strategy *id_uniqueness_strategy (void) const;

      Lifespan_Strategy *lifespan_strategy (void) const;

      Activation_Strategy *activation_strategy (void) const;

      ServantRetentionStrategy *servant_retention_strategy (void) const;

    private:
      Thread_Strategy *thread_strategy_;
      Request_Processing_Strategy *request_processing_strategy_;
      Id_Assignment_Strategy *id_assignment_strategy_;
      Lifespan_Strategy *lifespan_strategy_;
      Id_Uniqueness_Strategy *id_uniqueness_strategy_;
      Activation_Strategy *activation_strategy_;
      ServantRetentionStrategy *servant_retention_strategy_;
    };
  }
}

#if defined (__ACE_INLINE__)
# include "Active_Policy_Strategies.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_ACTIVE_POLICY_STRATEGIES_H */
