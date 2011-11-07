// -*- C++ -*-
// $Id$

#include "tao/ORB_Constants.h"
#include "tao/PortableServer/RequestProcessingStrategyAOMOnly.h"
#include "tao/PortableServer/ServantManagerC.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/PortableServer/Root_POA.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategyAOMOnly::RequestProcessingStrategyAOMOnly()
    {
    }

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

    PortableServer::ServantManager_ptr
    RequestProcessingStrategyAOMOnly::get_servant_manager (void)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

    void
    RequestProcessingStrategyAOMOnly::set_servant_manager (
      PortableServer::ServantManager_ptr /*imgr*/)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

    void
    RequestProcessingStrategyAOMOnly::set_servant (
      PortableServer::Servant /*servant*/)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

    PortableServer::Servant
    RequestProcessingStrategyAOMOnly::get_servant (void)
    {
      throw PortableServer::POA::WrongPolicy ();
    }

#endif /* TAO_HAS_MINIMUM_POA == 0 !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */

    TAO_SERVANT_LOCATION
    RequestProcessingStrategyAOMOnly::locate_servant (
      const PortableServer::ObjectId & system_id,
      PortableServer::Servant & servant)
    {
      return this->poa_->servant_present (system_id, servant);
    }

    PortableServer::Servant
    RequestProcessingStrategyAOMOnly::locate_servant (
      const char * /*operation*/,
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
      bool & /*wait_occurred_restart_call*/)
    {
      PortableServer::Servant servant = 0;

      servant = this->poa_->find_servant (system_id,
                                          servant_upcall,
                                          poa_current_impl);

      if (servant == 0)
        {
          throw ::CORBA::OBJECT_NOT_EXIST (
            CORBA::OMGVMCID | 2,
            CORBA::COMPLETED_NO);
        }

      return servant;
    }

    PortableServer::Servant
    RequestProcessingStrategyAOMOnly::system_id_to_servant (
      const PortableServer::ObjectId &system_id)
    {
      return this->poa_->find_servant (system_id);
    }

    PortableServer::Servant
    RequestProcessingStrategyAOMOnly::id_to_servant (
      const PortableServer::ObjectId &id)
    {
      return this->poa_->user_id_to_servant_i (id);
    }

    void
    RequestProcessingStrategyAOMOnly::cleanup_servant (
      PortableServer::Servant servant,
      const PortableServer::ObjectId &user_id)
    {
      if (servant)
        {
          // ATTENTION: Trick locking here, see class header for details
          Non_Servant_Upcall non_servant_upcall (*this->poa_);
          ACE_UNUSED_ARG (non_servant_upcall);

          try
            {
              servant->_remove_ref ();
            }
          catch (...)
            {
              // Ignore exceptions from servant cleanup.
            }
        }

      // This operation causes the association of the Object Id specified
      // by the oid parameter and its servant to be removed from the
      // Active Object Map.
      if (this->poa_->unbind_using_user_id (user_id) != 0)
        {
          throw ::CORBA::OBJ_ADAPTER ();
        }
    }

    void
    RequestProcessingStrategyAOMOnly::etherealize_objects (
      CORBA::Boolean /*etherealize_objects*/)
    {
    }

    PortableServer::ObjectId *
    RequestProcessingStrategyAOMOnly::servant_to_id (
      PortableServer::Servant servant)
    {
      return this->poa_->servant_to_user_id (servant);
    }

    void
    RequestProcessingStrategyAOMOnly::post_invoke_servant_cleanup(
      const PortableServer::ObjectId &/*system_id*/,
      const TAO::Portable_Server::Servant_Upcall &/*servant_upcall*/)
    {
    }

    ::PortableServer::RequestProcessingPolicyValue
    RequestProcessingStrategyAOMOnly::type() const
    {
      return ::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY;
    }

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
