/**
 *  @file ServantRetentionStrategy.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
#ifndef TAO_SERVANTRETENTIONSTRATEGY_H
#define TAO_SERVANTRETENTIONSTRATEGY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/Servant_Location.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
    {
    public:
      ServantRetentionStrategy ();
      virtual ~ServantRetentionStrategy () = default;

      virtual void strategy_init (TAO_Root_POA *poa) = 0;

      virtual void strategy_cleanup() = 0;

      virtual CORBA::ULong waiting_servant_deactivation () const = 0;

      virtual int is_servant_in_map (PortableServer::Servant servant,
                                     bool &wait_occurred_restart_call) = 0;

      virtual TAO_Servant_Location servant_present (
        PortableServer::ObjectId const &system_id,
        PortableServer::Servant &servant) = 0;

      virtual PortableServer::Servant find_servant (
        PortableServer::ObjectId const &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall,
        TAO::Portable_Server::POA_Current_Impl &poa_current_impl) = 0;

      virtual int find_servant_priority (
        PortableServer::ObjectId const &system_id,
        CORBA::Short &priority) = 0;

      virtual PortableServer::ObjectId *activate_object (
        PortableServer::Servant servant,
        CORBA::Short priority,
        bool &wait_occurred_restart_call) = 0;

      virtual void activate_object_with_id (
        PortableServer::ObjectId const &id,
        PortableServer::Servant servant,
        CORBA::Short priority,
        bool &wait_occurred_restart_call) = 0;

      virtual void deactivate_object (PortableServer::ObjectId const &id) = 0;

      virtual PortableServer::Servant find_servant (
        PortableServer::ObjectId const &system_id) = 0;

      virtual PortableServer::ObjectId *system_id_to_object_id (
        PortableServer::ObjectId const &system_id) = 0;

      virtual PortableServer::Servant user_id_to_servant (
        PortableServer::ObjectId const &id) = 0;

      virtual CORBA::Object_ptr id_to_reference (
        PortableServer::ObjectId const &id,
        bool indirect) = 0;

      virtual void deactivate_all_objects () = 0;

      virtual PortableServer::ObjectId *servant_to_user_id (
        PortableServer::Servant servant) = 0;

      virtual CORBA::Object_ptr servant_to_reference (
        PortableServer::Servant servant) = 0;

      virtual CORBA::Object_ptr create_reference (
        char const *intf,
        CORBA::Short priority) = 0;

#if !defined (CORBA_E_MICRO)
      virtual CORBA::Object_ptr create_reference_with_id (
        PortableServer::ObjectId const &oid,
        char const *intf,
        CORBA::Short priority) = 0;
#endif

      virtual int rebind_using_user_id_and_system_id (
        PortableServer::Servant servant,
        PortableServer::ObjectId const &user_id,
        PortableServer::ObjectId const &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall) = 0;

      virtual int unbind_using_user_id (
        PortableServer::ObjectId const &user_id) = 0;

      virtual CORBA::Boolean servant_has_remaining_activations (
        PortableServer::Servant servant) = 0;

      virtual ::PortableServer::ServantRetentionPolicyValue type() const = 0;

      virtual TAO_Active_Object_Map * get_active_object_map() const = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_SERVANTRETENTIONSTRATEGY_H */
