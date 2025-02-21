// -*- C++ -*-

//=============================================================================
/**
 *  @file ServantRetentionStrategyNonRetain.h
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
#include <atomic>

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
      ServantRetentionStrategyNonRetain () = default;

      void strategy_init (TAO_Root_POA *poa) override;

      void strategy_cleanup() override;

      CORBA::ULong waiting_servant_deactivation () const override;

      PortableServer::ObjectId *
      activate_object (PortableServer::Servant servant,
                       CORBA::Short priority,
                       bool &wait_occurred_restart_call) override;

      void
      activate_object_with_id (const PortableServer::ObjectId &id,
                               PortableServer::Servant servant,
                               CORBA::Short priority,
                               bool &wait_occurred_restart_call) override;

      void deactivate_object (const PortableServer::ObjectId &id) override;

      PortableServer::Servant find_servant (
        const PortableServer::ObjectId &system_id) override;

      int is_servant_in_map (PortableServer::Servant servant,
                             bool &wait_occurred_restart_call) override;

      PortableServer::ObjectId *system_id_to_object_id (
        const PortableServer::ObjectId &system_id) override;

      PortableServer::Servant
      user_id_to_servant (const PortableServer::ObjectId &id) override;

      CORBA::Object_ptr
      id_to_reference (const PortableServer::ObjectId &id,
                       bool indirect) override;

      TAO_Servant_Location servant_present (
        const PortableServer::ObjectId &system_id,
        PortableServer::Servant &servant) override;

      PortableServer::Servant find_servant (
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall,
        TAO::Portable_Server::POA_Current_Impl &poa_current_impl) override;

      int find_servant_priority (
        const PortableServer::ObjectId &system_id,
        CORBA::Short &priority) override;

      void deactivate_all_objects () override;

      PortableServer::ObjectId *servant_to_user_id (PortableServer::Servant servant) override;

      CORBA::Object_ptr servant_to_reference (PortableServer::Servant servant) override;

      CORBA::Object_ptr create_reference (const char *intf, CORBA::Short priority) override;

#if !defined (CORBA_E_MICRO)
      CORBA::Object_ptr create_reference_with_id (
        const PortableServer::ObjectId &oid,
        const char *intf,
        CORBA::Short priority) override;
#endif

      int rebind_using_user_id_and_system_id (
        PortableServer::Servant servant,
        const PortableServer::ObjectId &user_id,
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall) override;

      CORBA::Boolean servant_has_remaining_activations (PortableServer::Servant servant) override;

      int unbind_using_user_id (const PortableServer::ObjectId &user_id) override;

      ::PortableServer::ServantRetentionPolicyValue type() const override;

      TAO_Active_Object_Map * get_active_object_map() const override;

    protected:
      TAO_Root_POA *poa_ {};
      std::atomic<uint32_t> sys_id_count_ { 0 };
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_SERVANT_RETENTION_STRATEGY_NON_RETAIN_H */
