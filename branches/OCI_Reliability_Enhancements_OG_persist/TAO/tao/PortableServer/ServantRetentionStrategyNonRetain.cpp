// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServantRetentionStrategyNonRetain.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "tao/PortableServer/ServantRetentionStrategyNonRetain.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/Servant_Upcall.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/debug.h"

#include "ace/OS_NS_sys_time.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionStrategyNonRetain::ServantRetentionStrategyNonRetain (void) :
      poa_ (0),
      sys_id_count_ (0)
    {
    }

    void
    ServantRetentionStrategyNonRetain::strategy_init (TAO_Root_POA *poa)
    {
      poa_ = poa;
    }

    void
    ServantRetentionStrategyNonRetain::strategy_cleanup (void)
    {
      poa_ = 0;
    }

    void
    ServantRetentionStrategyNonRetain::deactivate_object (
      const PortableServer::ObjectId &/*id*/)
    {
      // When using Non_Retain we don't have an active object map and we just
      // can't deactivate any object
      throw PortableServer::POA::WrongPolicy ();
    }

    PortableServer::Servant
    ServantRetentionStrategyNonRetain::find_servant (
      const PortableServer::ObjectId &/*system_id*/)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

    PortableServer::ObjectId *
    ServantRetentionStrategyNonRetain::system_id_to_object_id (
      const PortableServer::ObjectId &system_id)
    {
      // The system id is the id (and no conversion/transformation is
      // needed).
      PortableServer::ObjectId *id = 0;
      ACE_NEW_THROW_EX (id,
                        PortableServer::ObjectId (system_id),
                        CORBA::NO_MEMORY ());

      return id;
    }

    PortableServer::Servant
    ServantRetentionStrategyNonRetain::user_id_to_servant (
      const PortableServer::ObjectId &/*id*/)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

    CORBA::Object_ptr
    ServantRetentionStrategyNonRetain::id_to_reference (
      const PortableServer::ObjectId &/*id*/,
      bool /*indirect*/)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

    TAO_SERVANT_LOCATION
    ServantRetentionStrategyNonRetain::servant_present (
      const PortableServer::ObjectId &/*system_id*/,
      PortableServer::Servant &/*servant*/)
    {
      return TAO_SERVANT_NOT_FOUND;
    }

    PortableServer::Servant
    ServantRetentionStrategyNonRetain::find_servant (
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl)
    {
      // We have the NON_RETAIN policy, user id is the system id.

      // Smartly copy all the data; <poa_current_impl.object_id_> does
      // not own the data.
      poa_current_impl.replace_object_id (system_id);

      servant_upcall.user_id (&system_id);

      return 0;
    }

    int
    ServantRetentionStrategyNonRetain::find_servant_priority (
        const PortableServer::ObjectId &/*system_id*/,
        CORBA::Short &/*priority*/)
    {
      return -1;
    }

    int
    ServantRetentionStrategyNonRetain::is_servant_in_map (
      PortableServer::Servant /*servant*/,
      bool &/*wait_occurred_restart_call*/)
    {
      return 0;
    }

    CORBA::ULong
    ServantRetentionStrategyNonRetain::waiting_servant_deactivation (void) const
    {
      return 0;
    }

    void
    ServantRetentionStrategyNonRetain::deactivate_all_objects (void)
    {
    }

    PortableServer::ObjectId *
    ServantRetentionStrategyNonRetain::servant_to_user_id (
      PortableServer::Servant /*servant*/)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

    CORBA::Object_ptr
    ServantRetentionStrategyNonRetain::servant_to_reference (
      PortableServer::Servant /*servant*/)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

    PortableServer::ObjectId *
    ServantRetentionStrategyNonRetain::activate_object (
      PortableServer::Servant /*servant*/,
      CORBA::Short /*priority*/,
      bool &/*wait_occurred_restart_call*/)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

    void
    ServantRetentionStrategyNonRetain::activate_object_with_id (
      const PortableServer::ObjectId &/*id*/,
      PortableServer::Servant /*servant*/,
      CORBA::Short /*priority*/,
      bool &/*wait_occurred_restart_call*/)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

    CORBA::Object_ptr
    ServantRetentionStrategyNonRetain::create_reference (
      const char *intf,
      CORBA::Short priority)
    {
      // This operation creates an object reference that encapsulates a
      // POA-generated Object Id value and the specified interface
      // repository id. This operation does not cause an activation to
      // take place. The resulting reference may be passed to clients, so
      // that subsequent requests on those references will cause the
      // appropriate servant manager to be invoked, if one is
      // available. The generated Object Id value may be obtained by
      // invoking POA::reference_to_id with the created reference.

      PortableServer::ObjectId_var system_id;
      PortableServer::ObjectId user_id;

      // Otherwise, it is the NON_RETAIN policy.  Therefore, any ol'
      // object id will do (even an empty one). However, to make an
      // object id useful for discriminating objects in applications
      // use a simple id of a counter and a time stamp. The use of a
      // counter by itself is not sufficient for uniqueness over time
      // and a timestamp isn't sufficient since multiple IDs may be
      // requested within the same time unit.

      PortableServer::ObjectId *sys_id = 0;
      ACE_NEW_THROW_EX (sys_id,
                        PortableServer::ObjectId (8),
                        CORBA::NO_MEMORY ());

      sys_id->length(8);

      long count = this->sys_id_count_++;
      ACE_Time_Value now = ACE_OS::gettimeofday();

      *reinterpret_cast<ACE_UINT32 *>(sys_id->get_buffer()) =
        count;

      *reinterpret_cast<ACE_UINT32 *>(sys_id->get_buffer() + 4) =
        static_cast<ACE_UINT32>(now.sec());

      system_id = sys_id;

      // User id is the same as system id.
      user_id = system_id.in ();

      // Remember params for potentially invoking <key_to_object> later.
      this->poa_->key_to_object_params_.set (system_id,
                                             intf,
                                             0,
                                             1,
                                             priority,
                                             true);

      return this->poa_->invoke_key_to_object_helper_i (intf, user_id);
    }

#if !defined (CORBA_E_MICRO)
    CORBA::Object_ptr
    ServantRetentionStrategyNonRetain::create_reference_with_id (
      const PortableServer::ObjectId &oid,
      const char *intf,
      CORBA::Short priority)
    {
      // This operation creates an object reference that encapsulates the
      // specified Object Id and interface repository Id values. This
      // operation does not cause an activation to take place.  The
      // resulting reference may be passed to clients, so that subsequent
      // requests on those references will cause the object to be
      // activated if necessary, or the default servant used, depending on
      // the applicable policies.

      PortableServer::Servant servant = 0;
      PortableServer::ObjectId_var system_id;

      // Otherwise, it is the NON_RETAIN policy.  Therefore, user id
      // is the same as system id.
      PortableServer::ObjectId *sys_id = 0;
      ACE_NEW_THROW_EX (sys_id,
                        PortableServer::ObjectId (oid),
                        CORBA::NO_MEMORY ());

      system_id = sys_id;

      // Remember params for potentially invoking <key_to_object> later.
      this->poa_->key_to_object_params_.set (system_id,
                                             intf,
                                             servant,
                                             1,
                                             priority,
                                             true);

      return this->poa_->invoke_key_to_object_helper_i (intf, oid);
    }
#endif

    int
    ServantRetentionStrategyNonRetain::rebind_using_user_id_and_system_id (
      PortableServer::Servant /*servant*/,
      const PortableServer::ObjectId &/*user_id*/,
      const PortableServer::ObjectId &/*system_id*/,
      TAO::Portable_Server::Servant_Upcall &/*servant_upcall*/)
    {
      return -1;
    }

    CORBA::Boolean
    ServantRetentionStrategyNonRetain::servant_has_remaining_activations (
      PortableServer::Servant /*servant*/)
    {
      return false;
    }

    int
    ServantRetentionStrategyNonRetain::unbind_using_user_id (
      const PortableServer::ObjectId &/*user_id*/)
    {
      return 0;
    }

    ::PortableServer::ServantRetentionPolicyValue
    ServantRetentionStrategyNonRetain::type() const
    {
      return ::PortableServer::NON_RETAIN;
    }

    TAO_Active_Object_Map *
    ServantRetentionStrategyNonRetain::get_active_object_map() const
    {
      return 0;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

