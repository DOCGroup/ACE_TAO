// -*- C++ -*-

//=============================================================================
/**
 *  @file Servant_Retention_Strategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_SERVANT_RETENTION_STRATEGY_H
#define TAO_SERVANT_RETENTION_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "PortableServerC.h"
#include "Policy_Strategy.h"
#include "Active_Object_Map.h"
#include "ace/Service_Config.h"
#include "Object_Adapter.h" // for TAO_SERVANT_LOCATION, move this enum to another file


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
    class TAO_PortableServer_Export Servant_Retention_Strategy :
       public virtual Policy_Strategy
    {
    public:
      Servant_Retention_Strategy (void);

      virtual ~Servant_Retention_Strategy (void);

      virtual CORBA::ULong waiting_servant_deactivation (void) const = 0;

      virtual void strategy_init (TAO_POA *poa);

      //virtual TAO_Active_Object_Map* get_aom() const = 0;

      virtual int is_servant_in_map (PortableServer::Servant servant,
                                      int &wait_occurred_restart_call) = 0;

      virtual
      int
      is_user_id_in_map (const PortableServer::ObjectId &id,
                            CORBA::Short priority,
                            int &priorities_match,
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
                          ACE_ENV_ARG_DECL) = 0;

    protected:
      TAO_POA* poa_;
    };

    class TAO_PortableServer_Export Retain_Servant_Retention_Strategy :
       public virtual Servant_Retention_Strategy
    {
    public:
      Retain_Servant_Retention_Strategy (void);

      virtual ~Retain_Servant_Retention_Strategy (void);

      CORBA::ULong waiting_servant_deactivation (void) const;

      virtual void strategy_init (TAO_POA *poa);

      virtual int is_servant_in_map (PortableServer::Servant servant,
                                      int &wait_occurred_restart_call);

      virtual
      int
      is_user_id_in_map (const PortableServer::ObjectId &id,
                            CORBA::Short priority,
                            int &priorities_match,
                            int &wait_occurred_restart_call);

      virtual TAO_Active_Object_Map* get_aom() const;

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
                          ACE_ENV_ARG_DECL);

    protected:
      void
      deactivate_map_entry (TAO_Active_Object_Map::Map_Entry *active_object_map_entry
                            ACE_ENV_ARG_DECL);

    private:
      TAO_Active_Object_Map *active_object_map_;
      CORBA::ULong waiting_servant_deactivation_;
      CORBA::Boolean etherealize_objects_;
    };

    class TAO_PortableServer_Export Non_Retain_Servant_Retention_Strategy :
       public virtual Servant_Retention_Strategy
    {
    public:
      virtual ~Non_Retain_Servant_Retention_Strategy (void);

      virtual TAO_Active_Object_Map* get_aom() const;

      CORBA::ULong waiting_servant_deactivation (void) const;

      virtual void deactivate_object (const PortableServer::ObjectId &id
                                      ACE_ENV_ARG_DECL);

      virtual
      PortableServer::Servant
      reference_to_servant (CORBA::Object_ptr reference
                            ACE_ENV_ARG_DECL);

      virtual int is_servant_in_map (PortableServer::Servant servant,
                                      int &wait_occurred_restart_call);

      virtual
      int
      is_user_id_in_map (const PortableServer::ObjectId &id,
                            CORBA::Short priority,
                            int &priorities_match,
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
        TAO_Active_Object_Map::Map_Entry *active_object_map_entry
        ACE_ENV_ARG_DECL);

      virtual
      PortableServer::ObjectId *
      servant_to_id (PortableServer::Servant servant
                          ACE_ENV_ARG_DECL);
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_SERVANT_RETENTION_STRATEGY_H */
