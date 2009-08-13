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

#include "tao/PortableServer/ServantRetentionStrategyNonRetain.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Root_POA;

namespace TAO
{
  namespace Portable_Server
  {
    class ServantRetentionStrategyRetain
       : public ServantRetentionStrategyNonRetain
    {
    public:
      ServantRetentionStrategyRetain (void);

      CORBA::ULong waiting_servant_deactivation (void) const;

      virtual void strategy_init (TAO_Root_POA *poa);

      virtual void strategy_cleanup(void);

      virtual int is_servant_in_map (PortableServer::Servant servant,
                                     bool &wait_occurred_restart_call);

      virtual
      PortableServer::ObjectId *
      activate_object (PortableServer::Servant servant,
                       CORBA::Short priority,
                       bool &wait_occurred_restart_call);

#if !defined (CORBA_E_MICRO)
      virtual
      void
      activate_object_with_id (const PortableServer::ObjectId &id,
                               PortableServer::Servant servant,
                               CORBA::Short priority,
                               bool &wait_occurred_restart_call);
#endif

      void deactivate_object (const PortableServer::ObjectId &id);

      virtual PortableServer::Servant find_servant (
        const PortableServer::ObjectId &system_id);

      virtual PortableServer::ObjectId * system_id_to_object_id (
        const PortableServer::ObjectId &system_id);

      virtual
      PortableServer::Servant
      user_id_to_servant (const PortableServer::ObjectId &id);

      CORBA::Object_ptr id_to_reference (const PortableServer::ObjectId &id,
                                         bool indirect);

      virtual
      TAO_SERVANT_LOCATION
      servant_present (const PortableServer::ObjectId &system_id,
                       PortableServer::Servant &servant);

      virtual PortableServer::Servant find_servant (
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall,
        TAO::Portable_Server::POA_Current_Impl &poa_current_impl);

      virtual int find_servant_priority (
        const PortableServer::ObjectId &system_id,
        CORBA::Short &priority);

      virtual void deactivate_all_objects (void);

      virtual PortableServer::ObjectId *servant_to_user_id (
        PortableServer::Servant servant);

      virtual
      CORBA::Object_ptr servant_to_reference (PortableServer::Servant servant);

      virtual
      CORBA::Object_ptr create_reference (
        const char *intf,
        CORBA::Short priority);

#if !defined (CORBA_E_MICRO)
      virtual
      CORBA::Object_ptr create_reference_with_id (
        const PortableServer::ObjectId &oid,
        const char *intf,
        CORBA::Short priority);
#endif

      virtual ::PortableServer::ServantRetentionPolicyValue type() const;

    protected:
      int
      is_user_id_in_map (const PortableServer::ObjectId &id,
                         CORBA::Short priority,
                         bool &priorities_match,
                         bool &wait_occurred_restart_call);

      void
      deactivate_map_entry (TAO_Active_Object_Map_Entry *active_object_map_entry);

      PortableServer::ObjectId *servant_to_system_id_i (
          PortableServer::Servant p_servant,
          CORBA::Short &priority);

      virtual
      int rebind_using_user_id_and_system_id (
        PortableServer::Servant servant,
        const PortableServer::ObjectId &user_id,
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall);

      virtual
      CORBA::Boolean servant_has_remaining_activations (
        PortableServer::Servant servant);

      virtual int unbind_using_user_id (
        const PortableServer::ObjectId &user_id);

    private:
      auto_ptr<TAO_Active_Object_Map> active_object_map_;
      CORBA::ULong waiting_servant_deactivation_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_SERVANT_RETENTION_STRATEGY_RETAIN_H */
