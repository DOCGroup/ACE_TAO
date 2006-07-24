// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingStrategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_REQUEST_PROCESSING_STRATEGY_H
#define TAO_REQUEST_PROCESSING_STRATEGY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/Policy_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Location.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace PortableServer
{
  class ServantManager;
  typedef ServantManager *ServantManager_ptr;
}

namespace TAO
{
  namespace Portable_Server
  {
    class Servant_Upcall;
    class POA_Current_Impl;
  }
}

class TAO_Root_POA;

namespace TAO
{
  namespace Portable_Server
  {
    class RequestProcessingStrategy
      : public Policy_Strategy
    {
    public:
      RequestProcessingStrategy (void);

      virtual void strategy_init(
        TAO_Root_POA *poa
        ACE_ENV_ARG_DECL);

      virtual void strategy_init(
        TAO_Root_POA *poa,
        ::PortableServer::ServantRetentionPolicyValue
        ACE_ENV_ARG_DECL);

      virtual void strategy_cleanup(ACE_ENV_SINGLE_ARG_DECL);

#if (TAO_HAS_MINIMUM_POA == 0)

      virtual PortableServer::ServantManager_ptr get_servant_manager (
        ACE_ENV_SINGLE_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual void set_servant_manager (
        PortableServer::ServantManager_ptr imgr
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual void set_servant (PortableServer::Servant servant ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy)) = 0;

#endif /* TAO_HAS_MINIMUM_POA == 0 */

      virtual PortableServer::Servant get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy)) = 0;

      virtual TAO_SERVANT_LOCATION locate_servant (
        const PortableServer::ObjectId &system_id,
        PortableServer::Servant &servant
        ACE_ENV_ARG_DECL) = 0;

      virtual PortableServer::Servant locate_servant (
        const char *operation,
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall,
        TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
        int &wait_occurred_restart_call
        ACE_ENV_ARG_DECL) = 0;

      virtual void cleanup_servant (
        PortableServer::Servant servant,
        const PortableServer::ObjectId &user_id
        ACE_ENV_ARG_DECL) = 0;

      virtual PortableServer::Servant system_id_to_servant (
        const PortableServer::ObjectId &system_id
        ACE_ENV_ARG_DECL) = 0;

      virtual PortableServer::Servant id_to_servant (
        const PortableServer::ObjectId &id
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ObjectNotActive,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual void etherealize_objects (CORBA::Boolean etherealize_objects) = 0;

      virtual PortableServer::ObjectId *servant_to_id (
        PortableServer::Servant servant
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ServantNotActive,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual void post_invoke_servant_cleanup(
        const PortableServer::ObjectId &system_id,
        const TAO::Portable_Server::Servant_Upcall &servant_upcall) = 0;

      virtual ::PortableServer::RequestProcessingPolicyValue type() const = 0;

      virtual ::PortableServer::ServantRetentionPolicyValue sr_type() const;

    protected:
      TAO_Root_POA* poa_;
      ::PortableServer::ServantRetentionPolicyValue sr_value_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_REQUEST_PROCESSING_STRATEGY_H */
