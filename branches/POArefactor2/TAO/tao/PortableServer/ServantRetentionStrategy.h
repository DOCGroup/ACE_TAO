// -*- C++ -*-

//=============================================================================
/**
 *  @file ServantRetentionStrategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_SERVANTRETENTIONSTRATEGY_H
#define TAO_SERVANTRETENTIONSTRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "Policy_Strategy.h"
#include "Active_Object_Map.h"
#include "ace/Service_Config.h"
#include "Servant_Location.h"
#include "Servant_Upcall.h"

#define TAO_PORTABLESERVER_SAFE_INCLUDE
#include "PortableServerC.h"
#undef TAO_PORTABLESERVER_SAFE_INCLUDE

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
class TAO_Active_Object_Map;

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export ServantRetentionStrategy :
       public virtual Policy_Strategy
    {
    public:
      ServantRetentionStrategy (void);

      virtual ~ServantRetentionStrategy (void);

      virtual CORBA::ULong waiting_servant_deactivation (void) const = 0;

      virtual void strategy_init (TAO_POA *poa);

      virtual TAO_Active_Object_Map* get_aom() const = 0;

      virtual int is_servant_in_map (PortableServer::Servant servant,
                                      int &wait_occurred_restart_call) = 0;

      virtual
      TAO_SERVANT_LOCATION
      locate_servant (const PortableServer::ObjectId &system_id,
                      PortableServer::Servant &servant
                      ACE_ENV_ARG_DECL) = 0;

      virtual
      PortableServer::Servant
      locate_servant (const char *operation,
                      const PortableServer::ObjectId &system_id,
                      TAO::Portable_Server::Servant_Upcall &servant_upcall,
                      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
                      int &wait_occurred_restart_call
                      ACE_ENV_ARG_DECL) = 0;

      virtual
      PortableServer::ObjectId *
      activate_object (PortableServer::Servant servant,
                       CORBA::Short priority,
                       int &wait_occurred_restart_call
                            ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ServantAlreadyActive,
                         PortableServer::POA::WrongPolicy)) = 0;

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
                   PortableServer::POA::WrongPolicy)) = 0;

      virtual void deactivate_object (const PortableServer::ObjectId &id
                                      ACE_ENV_ARG_DECL) = 0;

      virtual
      PortableServer::Servant
      reference_to_servant (CORBA::Object_ptr reference
                            ACE_ENV_ARG_DECL) = 0;

      virtual
      PortableServer::ObjectId *
      reference_to_id (CORBA::Object_ptr reference,
                       PortableServer::ObjectId system_id
                       ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongAdapter,
                         PortableServer::POA::WrongPolicy)) = 0;

      virtual
      PortableServer::Servant
      id_to_servant (const PortableServer::ObjectId &id
                     ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy)) = 0;

      virtual
      CORBA::Object_ptr
      id_to_reference (const PortableServer::ObjectId &id
                       ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy)) = 0;

      virtual
      void
      deactivate_all_objects (CORBA::Boolean etherealize_objects
                              ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy)) = 0;

      virtual
      void
      cleanup_servant (
        TAO_Active_Object_Map::Map_Entry *active_object_map_entry
        ACE_ENV_ARG_DECL) = 0;

      virtual
      PortableServer::ObjectId *
      servant_to_id (PortableServer::Servant servant
                          ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantNotActive,
                   PortableServer::POA::WrongPolicy)) = 0;

      virtual
      CORBA::Object_ptr
      servant_to_reference (PortableServer::Servant servant
                            ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableServer::POA::ServantNotActive,
                      PortableServer::POA::WrongPolicy)) = 0;

      virtual
      CORBA::Object_ptr create_reference (
        const char *intf,
        CORBA::Short priority
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual
      CORBA::Object_ptr create_reference_with_id (
        const PortableServer::ObjectId &oid,
        const char *intf,
        CORBA::Short priority
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy)) = 0;

    protected:
      TAO_POA* poa_;
    };
  }
}
#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_SERVANTRETENTIONSTRATEGY_H */
