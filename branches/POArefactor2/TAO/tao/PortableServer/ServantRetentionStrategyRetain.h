// -*- C++ -*-

//=============================================================================
/**
 *  @file ServantRetentionStrategyRetain.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_SERVANT_RETENTION_STRATEGY_RETAIN_H
#define TAO_SERVANT_RETENTION_STRATEGY_RETAIN_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "Active_Object_Map.h"
#include "ace/Service_Config.h"
#include "Servant_Location.h"
#include "ServantRetentionStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace CORBA
{
  class PolicyError;
  class PolicyList;
}

class TAO_POA;

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Retain_Servant_Retention_Strategy :
       public virtual ServantRetentionStrategy
    {
    public:
      Retain_Servant_Retention_Strategy (void);

      virtual ~Retain_Servant_Retention_Strategy (void);

      CORBA::ULong waiting_servant_deactivation (void) const;

      virtual void strategy_init (TAO_POA *poa);

      virtual int is_servant_in_map (PortableServer::Servant servant,
                                      int &wait_occurred_restart_call);

      virtual TAO_Active_Object_Map* get_aom() const;

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

      void deactivate_object (const PortableServer::ObjectId &id
                              ACE_ENV_ARG_DECL);

      virtual
      PortableServer::Servant
      reference_to_servant (CORBA::Object_ptr reference
                            ACE_ENV_ARG_DECL);

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
        TAO_Active_Object_Map::Map_Entry *active_object_map_entry
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

    protected:
      int
      is_user_id_in_map (const PortableServer::ObjectId &id,
                         CORBA::Short priority,
                         int &priorities_match,
                         int &wait_occurred_restart_call);

      void
      deactivate_map_entry (TAO_Active_Object_Map::Map_Entry *active_object_map_entry
                            ACE_ENV_ARG_DECL);

      PortableServer::ObjectId *servant_to_system_id_i (
          PortableServer::Servant p_servant,
          CORBA::Short &priority
          ACE_ENV_ARG_DECL
        )
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ServantNotActive,
                         PortableServer::POA::WrongPolicy));

    private:
      TAO_Active_Object_Map *active_object_map_;
      CORBA::ULong waiting_servant_deactivation_;
      CORBA::Boolean etherealize_objects_;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_SERVANT_RETENTION_STRATEGY_RETAIN_H */
