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

#include "portableserver_export.h"
#include "Servant_Location.h"
#include "ServantRetentionStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

class TAO_POA;

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Non_Retain_Servant_Retention_Strategy
       : public virtual ServantRetentionStrategy
    {
    public:
      Non_Retain_Servant_Retention_Strategy (void);

      virtual ~Non_Retain_Servant_Retention_Strategy (void);

      virtual
      void strategy_init (
        TAO_POA *poa,
        IdAssignmentStrategy* id_assignment_strategy,
        RequestProcessingStrategy* request_processing_strategy
        ACE_ENV_ARG_DECL);

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

      virtual void deactivate_object (const PortableServer::ObjectId &id
                                      ACE_ENV_ARG_DECL);

      virtual
      PortableServer::Servant
      reference_to_servant (CORBA::Object_ptr reference,
                            PortableServer::ObjectId system_id
                            ACE_ENV_ARG_DECL);

      virtual int is_servant_in_map (PortableServer::Servant servant,
                                      int &wait_occurred_restart_call);

      virtual
      PortableServer::ObjectId *
      reference_to_id (CORBA::Object_ptr reference,
                       PortableServer::ObjectId system_id
                       ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongAdapter,
                         PortableServer::POA::WrongPolicy));

      virtual
      PortableServer::Servant
      id_to_servant (const PortableServer::ObjectId &id
                     ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy));

      CORBA::Object_ptr
      id_to_reference (const PortableServer::ObjectId &id
                       ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy));

      virtual
      TAO_SERVANT_LOCATION
      locate_servant (const PortableServer::ObjectId &system_id,
                      PortableServer::Servant &servant
                      ACE_ENV_ARG_DECL);

      virtual
      PortableServer::Servant
      locate_servant (const char *operation,
                      const PortableServer::ObjectId &system_id,
                      TAO::Portable_Server::Servant_Upcall &servant_upcall,
                      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
                      int &wait_occurred_restart_call
                      ACE_ENV_ARG_DECL);

      virtual
      void
      deactivate_all_objects (CORBA::Boolean etherealize_objects
                              ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));
      virtual
      void
      cleanup_servant (
        TAO_Active_Object_Map_Entry *active_object_map_entry
        ACE_ENV_ARG_DECL);

      virtual
      PortableServer::ObjectId *
      servant_to_id (PortableServer::Servant servant
                     ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantNotActive,
                   PortableServer::POA::WrongPolicy));

      virtual
      CORBA::Object_ptr
      servant_to_reference (PortableServer::Servant servant
                            ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableServer::POA::ServantNotActive,
                      PortableServer::POA::WrongPolicy));

      virtual
      CORBA::Object_ptr create_reference (
        const char *intf,
        CORBA::Short priority
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy));

      virtual
      CORBA::Object_ptr create_reference_with_id (
        const PortableServer::ObjectId &oid,
        const char *intf,
        CORBA::Short priority
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy));

      virtual
      int rebind_using_user_id_and_system_id (
        PortableServer::Servant servant,
        const PortableServer::ObjectId &user_id,
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall);

      virtual
      CORBA::Boolean servant_has_remaining_activations (
        PortableServer::Servant servant);

    protected:
      TAO_POA *poa_;
      RequestProcessingStrategy *request_processing_strategy_;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_SERVANT_RETENTION_STRATEGY_NON_RETAIN_H */
