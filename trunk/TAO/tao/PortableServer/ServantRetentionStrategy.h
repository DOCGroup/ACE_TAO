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

#include "tao/PortableServer/Policy_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Location.h"
#include "tao/PortableServer/Servant_Upcall.h"
#include "tao/PortableServer/ServantRetentionPolicyC.h"
#include "tao/PortableServer/PortableServer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Root_POA;
class TAO_Active_Object_Map;
struct TAO_Active_Object_Map_Entry;

namespace TAO
{
  namespace Portable_Server
  {
    class ServantRetentionStrategy
      : public Policy_Strategy
    {
    public:
      virtual CORBA::ULong waiting_servant_deactivation (void) const = 0;

      virtual int is_servant_in_map (PortableServer::Servant servant,
                                     int &wait_occurred_restart_call) = 0;

      virtual TAO_SERVANT_LOCATION servant_present (
        const PortableServer::ObjectId &system_id,
        PortableServer::Servant &servant
        ACE_ENV_ARG_DECL) = 0;

      virtual PortableServer::Servant find_servant (
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall,
        TAO::Portable_Server::POA_Current_Impl &poa_current_impl
        ACE_ENV_ARG_DECL) = 0;

      virtual int find_servant_priority (
        const PortableServer::ObjectId &system_id,
        CORBA::Short &priority
        ACE_ENV_ARG_DECL) = 0;

      virtual PortableServer::ObjectId *activate_object (
        PortableServer::Servant servant,
        CORBA::Short priority,
        int &wait_occurred_restart_call
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ServantAlreadyActive,
                           PortableServer::POA::WrongPolicy)) = 0;

#if !defined (CORBA_E_MICRO)
      virtual void activate_object_with_id (
        const PortableServer::ObjectId &id,
        PortableServer::Servant servant,
        CORBA::Short priority,
        int &wait_occurred_restart_call
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ServantAlreadyActive,
                           PortableServer::POA::ObjectAlreadyActive,
                           PortableServer::POA::WrongPolicy)) = 0;
#endif

      virtual void deactivate_object (const PortableServer::ObjectId &id
                                      ACE_ENV_ARG_DECL) = 0;

      virtual PortableServer::Servant find_servant (
        const PortableServer::ObjectId &system_id
        ACE_ENV_ARG_DECL) = 0;

      virtual PortableServer::ObjectId *system_id_to_object_id (
        const PortableServer::ObjectId &system_id
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongAdapter,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual PortableServer::Servant user_id_to_servant (
        const PortableServer::ObjectId &id
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ObjectNotActive,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual CORBA::Object_ptr id_to_reference (
        const PortableServer::ObjectId &id,
        bool indirect
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ObjectNotActive,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual void deactivate_all_objects (ACE_ENV_SINGLE_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual PortableServer::ObjectId *servant_to_user_id (
        PortableServer::Servant servant
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ServantNotActive,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual CORBA::Object_ptr servant_to_reference (
        PortableServer::Servant servant
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ServantNotActive,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual CORBA::Object_ptr create_reference (
        const char *intf,
        CORBA::Short priority
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual CORBA::Object_ptr create_reference_with_id (
        const PortableServer::ObjectId &oid,
        const char *intf,
        CORBA::Short priority
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

      virtual int rebind_using_user_id_and_system_id (
        PortableServer::Servant servant,
        const PortableServer::ObjectId &user_id,
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall) = 0;

      virtual int unbind_using_user_id (
        const PortableServer::ObjectId &user_id) = 0;

      virtual CORBA::Boolean servant_has_remaining_activations (
        PortableServer::Servant servant) = 0;

      virtual ::PortableServer::ServantRetentionPolicyValue type() const = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_SERVANTRETENTIONSTRATEGY_H */
