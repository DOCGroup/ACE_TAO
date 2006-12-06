// -*- C++ -*-

#include "tao/ORB_Constants.h"
#include "tao/TSS_Resources.h"
#include "tao/PortableServer/RequestProcessingStrategyDefaultServant.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/ServantManagerC.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/PortableServer/POA_Current_Impl.h"

ACE_RCSID (PortableServer,
           Request_Processing,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategyDefaultServant::RequestProcessingStrategyDefaultServant (void)
      : default_servant_ (0)
    {
    }

    void
    RequestProcessingStrategyDefaultServant::strategy_cleanup(
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
      this->default_servant_ = 0;
    }

    PortableServer::ServantManager_ptr
    RequestProcessingStrategyDefaultServant::get_servant_manager (
      ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        PortableServer::ServantManager::_nil ());
    }

    void
    RequestProcessingStrategyDefaultServant::set_servant_manager (
      PortableServer::ServantManager_ptr /*imgr*/
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    PortableServer::Servant
    RequestProcessingStrategyDefaultServant::get_servant (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      // This operation returns the default servant associated with the
      // POA.
      return this->default_servant_.in ();
    }

    void
    RequestProcessingStrategyDefaultServant::set_servant (
      PortableServer::Servant servant
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      // This operation registers the specified servant with the POA as
      // the default servant. This servant will be used for all requests
      // for which no servant is found in the Active Object Map.
      this->default_servant_ = servant;

      // The implementation of set_servant will invoke _add_ref at least
      // once on the Servant argument before returning. When the POA no
      // longer needs the Servant, it will invoke _remove_ref on it the
      // same number of times.
      if (servant != 0)
        {
          // A recursive thread lock without using a recursive thread
          // lock.  Non_Servant_Upcall has a magic constructor and
          // destructor.  We unlock the Object_Adapter lock for the
          // duration of the servant activator upcalls; reacquiring once
          // the upcalls complete.  Even though we are releasing the lock,
          // other threads will not be able to make progress since
          // <Object_Adapter::non_servant_upcall_in_progress_> has been
          // set.
          Non_Servant_Upcall non_servant_upcall (*this->poa_);
          ACE_UNUSED_ARG (non_servant_upcall);

          servant->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

    TAO_SERVANT_LOCATION
    RequestProcessingStrategyDefaultServant::locate_servant (
      const PortableServer::ObjectId & system_id,
      PortableServer::Servant & servant
      ACE_ENV_ARG_DECL)
    {
      TAO_SERVANT_LOCATION location = TAO_SERVANT_NOT_FOUND;

      location = this->poa_->servant_present (system_id,
                                              servant
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (TAO_SERVANT_NOT_FOUND);

      if (location == TAO_SERVANT_NOT_FOUND)
        {
          if (this->default_servant_.in () != 0)
            {
              location = TAO_DEFAULT_SERVANT;
            }
        }

      return location;
    }

    PortableServer::Servant
    RequestProcessingStrategyDefaultServant::locate_servant (
      const char * /*operation*/,
      const PortableServer::ObjectId & system_id,
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
          // If the POA has the USE_DEFAULT_SERVANT policy, a default servant
          // has been associated with the POA so the POA will invoke the
          // appropriate method on that servant. If no servant has been
          // associated with the POA, the POA raises the OBJ_ADAPTER system
          // exception.
          PortableServer::Servant default_servant = this->default_servant_.in ();
          if (default_servant == 0)
            {
              ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (CORBA::OMGVMCID | 3,
                                                    CORBA::COMPLETED_NO),
                                                    0);
            }
          else
            {
              // Success
              servant = default_servant;
            }
        }

        return servant;
    }

    PortableServer::Servant
    RequestProcessingStrategyDefaultServant::system_id_to_servant (
      const PortableServer::ObjectId &system_id
      ACE_ENV_ARG_DECL)
    {
      PortableServer::Servant servant = this->default_servant_.in ();

      if (servant == 0)
        {
          servant = this->poa_->find_servant (system_id
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }

      return servant;
    }

    PortableServer::Servant
    RequestProcessingStrategyDefaultServant::id_to_servant (
      const PortableServer::ObjectId & /*id*/
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      PortableServer::Servant servant = this->default_servant_.in ();

      if (servant == 0)
        {
          /*
           * If using default servant request processing strategy but
           * no default servant is available, we will raise the
           * ObjectNotActive system exception.
           */
          ACE_THROW_RETURN (PortableServer::POA::ObjectNotActive (),
                            0);
        }

      return servant;
    }

    void
    RequestProcessingStrategyDefaultServant::cleanup_servant (
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
    RequestProcessingStrategyDefaultServant::etherealize_objects (
      CORBA::Boolean /*etherealize_objects*/)
    {
    }

    PortableServer::ObjectId *
    RequestProcessingStrategyDefaultServant::servant_to_id (
      PortableServer::Servant servant
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ServantNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      PortableServer::Servant default_servant = this->default_servant_.in ();

      if (default_servant != 0 &&
          default_servant == servant)
        {
          // If they are the same servant, then check if we are in an
          // upcall.
          TAO::Portable_Server::POA_Current_Impl *poa_current_impl =
            static_cast <TAO::Portable_Server::POA_Current_Impl *>
                        (TAO_TSS_Resources::instance ()->poa_current_impl_);
          // If we are in an upcall on the default servant, return the
          // ObjectId associated with the current invocation.
          if (poa_current_impl != 0 &&
              servant == poa_current_impl->servant ())
            {
              return poa_current_impl->get_object_id ();
            }
        }

      return this->poa_->servant_to_user_id (servant ACE_ENV_ARG_PARAMETER);
    }

    void
    RequestProcessingStrategyDefaultServant::post_invoke_servant_cleanup(
      const PortableServer::ObjectId &/*system_id*/,
      const TAO::Portable_Server::Servant_Upcall &/*servant_upcall*/)
    {
    }

    ::PortableServer::RequestProcessingPolicyValue
    RequestProcessingStrategyDefaultServant::type() const
    {
      return ::PortableServer::USE_DEFAULT_SERVANT;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */

