// -*- C++ -*-

#include "tao/ORB_Constants.h"
#include "tao/PortableServer/RequestProcessingStrategyAOMOnly.h"
#include "tao/PortableServer/ServantManagerC.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/PortableServer/Root_POA.h"

ACE_RCSID (PortableServer,
           Request_Processing,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategyAOMOnly::RequestProcessingStrategyAOMOnly()
    {
    }

#if (TAO_HAS_MINIMUM_POA == 0)

    PortableServer::ServantManager_ptr
    RequestProcessingStrategyAOMOnly::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        PortableServer::ServantManager::_nil ());
    }

    void
    RequestProcessingStrategyAOMOnly::set_servant_manager (
      PortableServer::ServantManager_ptr /*imgr*/
                         ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    void
    RequestProcessingStrategyAOMOnly::set_servant (
      PortableServer::Servant /*servant*/
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

#endif /* TAO_HAS_MINIMUM_POA == 0 */

    PortableServer::Servant
    RequestProcessingStrategyAOMOnly::get_servant (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

    TAO_SERVANT_LOCATION
    RequestProcessingStrategyAOMOnly::locate_servant (
      const PortableServer::ObjectId & system_id,
      PortableServer::Servant & servant
      ACE_ENV_ARG_DECL)
    {
      return this->poa_->servant_present (system_id,
                                          servant
                                          ACE_ENV_ARG_PARAMETER);
    }

    PortableServer::Servant
    RequestProcessingStrategyAOMOnly::locate_servant (
      const char * /*operation*/,
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
      bool & /*wait_occurred_restart_call*/
      ACE_ENV_ARG_DECL)
    {
      PortableServer::Servant servant = 0;

      servant = this->poa_->find_servant (system_id,
                                          servant_upcall,
                                          poa_current_impl
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (servant == 0)
        {
          ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (CORBA::OMGVMCID | 2,
                                                     CORBA::COMPLETED_NO),
                            0);
        }

      return servant;
    }

    PortableServer::Servant
    RequestProcessingStrategyAOMOnly::system_id_to_servant (
      const PortableServer::ObjectId &system_id
      ACE_ENV_ARG_DECL)
    {
      return this->poa_->find_servant (system_id
                                       ACE_ENV_ARG_PARAMETER);
    }

    PortableServer::Servant
    RequestProcessingStrategyAOMOnly::id_to_servant (
      const PortableServer::ObjectId &id
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      return this->poa_->user_id_to_servant_i (id
                                               ACE_ENV_ARG_PARAMETER);
    }

    void
    RequestProcessingStrategyAOMOnly::cleanup_servant (
      PortableServer::Servant servant,
      const PortableServer::ObjectId &user_id
      ACE_ENV_ARG_DECL)
    {
      if (servant)
        {
          // ATTENTION: Trick locking here, see class header for details
          Non_Servant_Upcall non_servant_upcall (*this->poa_);
          ACE_UNUSED_ARG (non_servant_upcall);

          servant->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }

      // This operation causes the association of the Object Id specified
      // by the oid parameter and its servant to be removed from the
      // Active Object Map.
      int result = this->poa_->unbind_using_user_id (user_id);

      if (result != 0)
        {
          ACE_THROW (CORBA::OBJ_ADAPTER ());
        }
    }

    void
    RequestProcessingStrategyAOMOnly::etherealize_objects (
      CORBA::Boolean /*etherealize_objects*/)
    {
    }

    PortableServer::ObjectId *
    RequestProcessingStrategyAOMOnly::servant_to_id (
      PortableServer::Servant servant
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ServantNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      return this->poa_->servant_to_user_id (servant ACE_ENV_ARG_PARAMETER);
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
