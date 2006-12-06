// -*- C++ -*-

//=============================================================================
/**
 *  @file ServantRetentionStrategyNonRetain.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_SERVANT_RETENTION_STRATEGY_NON_RETAIN_H
#define TAO_SERVANT_RETENTION_STRATEGY_NON_RETAIN_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/ServantRetentionStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Location.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Root_POA;

namespace TAO
{
  namespace Portable_Server
  {
    class ServantRetentionStrategyNonRetain
       : public ServantRetentionStrategy
    {
    public:
      ServantRetentionStrategyNonRetain (void);

      virtual void strategy_init (TAO_Root_POA *poa ACE_ENV_ARG_DECL);

      virtual void strategy_cleanup(ACE_ENV_SINGLE_ARG_DECL);

      CORBA::ULong waiting_servant_deactivation (void) const;

      virtual
      PortableServer::ObjectId *
      activate_object (PortableServer::Servant servant,
                       CORBA::Short priority,
                       int &wait_occurred_restart_call
                            ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ServantAlreadyActive,
                         PortableServer::POA::WrongPolicy));

#if !defined (CORBA_E_MICRO)
      virtual
      void
      activate_object_with_id (const PortableServer::ObjectId &id,
                               PortableServer::Servant servant,
                               CORBA::Short priority,
                               int &wait_occurred_restart_call
                               ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::ObjectAlreadyActive,
                   PortableServer::POA::WrongPolicy));
#endif

      virtual void deactivate_object (const PortableServer::ObjectId &id
                                      ACE_ENV_ARG_DECL);

      virtual PortableServer::Servant find_servant (
        const PortableServer::ObjectId &system_id
        ACE_ENV_ARG_DECL);

      virtual int is_servant_in_map (PortableServer::Servant servant,
                                      int &wait_occurred_restart_call);

      virtual PortableServer::ObjectId *system_id_to_object_id (
        const PortableServer::ObjectId &system_id
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongAdapter,
                           PortableServer::POA::WrongPolicy));

      virtual
      PortableServer::Servant
      user_id_to_servant (const PortableServer::ObjectId &id
                          ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy));

      CORBA::Object_ptr
      id_to_reference (const PortableServer::ObjectId &id,
                       bool indirect
                       ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy));

      virtual TAO_SERVANT_LOCATION servant_present (
        const PortableServer::ObjectId &system_id,
        PortableServer::Servant &servant
        ACE_ENV_ARG_DECL);

      virtual PortableServer::Servant find_servant (
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall,
        TAO::Portable_Server::POA_Current_Impl &poa_current_impl
        ACE_ENV_ARG_DECL);

      virtual int find_servant_priority (
        const PortableServer::ObjectId &system_id,
        CORBA::Short &priority
        ACE_ENV_ARG_DECL);

      virtual void deactivate_all_objects (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      virtual PortableServer::ObjectId *servant_to_user_id (
        PortableServer::Servant servant
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ServantNotActive,
                           PortableServer::POA::WrongPolicy));

      virtual CORBA::Object_ptr servant_to_reference (
        PortableServer::Servant servant
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ServantNotActive,
                           PortableServer::POA::WrongPolicy));

      virtual CORBA::Object_ptr create_reference (
        const char *intf,
        CORBA::Short priority
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy));

      virtual CORBA::Object_ptr create_reference_with_id (
        const PortableServer::ObjectId &oid,
        const char *intf,
        CORBA::Short priority
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException));

      virtual int rebind_using_user_id_and_system_id (
        PortableServer::Servant servant,
        const PortableServer::ObjectId &user_id,
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall);

      virtual CORBA::Boolean servant_has_remaining_activations (
        PortableServer::Servant servant);

      virtual int unbind_using_user_id (
        const PortableServer::ObjectId &user_id);

        virtual ::PortableServer::ServantRetentionPolicyValue type() const;

    protected:
      TAO_Root_POA *poa_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 && !CORBA_E_COMPACT && !CORBA_E_MICRO */

#include /**/ "ace/post.h"
#endif /* TAO_SERVANT_RETENTION_STRATEGY_NON_RETAIN_H */
