// -*- C++ -*-

//=============================================================================
/**
 *  @file    Request_Processing_Strategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "tao/ORB_Constants.h"
#include "tao/PortableServer/ServantActivatorC.h"
#include "tao/PortableServer/RequestProcessingStrategyServantActivator.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/POA.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/Servant_Upcall.h"

ACE_RCSID (PortableServer,
           Request_Processing,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Servant_Activator_Request_Processing_Strategy::Servant_Activator_Request_Processing_Strategy (void)
    {
    }

    Servant_Activator_Request_Processing_Strategy::~Servant_Activator_Request_Processing_Strategy (void)
    {
    }

    PortableServer::ServantManager_ptr
    Servant_Activator_Request_Processing_Strategy::get_servant_manager (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      return PortableServer::ServantManager::_duplicate (this->servant_activator_.in ());
    }

    void
    Servant_Activator_Request_Processing_Strategy::set_servant_manager (
      PortableServer::ServantManager_ptr imgr
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      // This operation sets the default servant manager associated with the
      // POA. This operation may only be invoked once after a POA has been
      // created. Attempting to set the servant manager after one has already
      // been set will result in the BAD_INV_ORDER system exception with
      // standard minor code 6 being raised (see 11.3.9.12 of the corba spec)
      if (!CORBA::is_nil (this->servant_activator_.in ()))
        {
          ACE_THROW (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 6,
                                           CORBA::COMPLETED_NO));
        }

      this->servant_activator_ = PortableServer::ServantActivator::_narrow (imgr
                                                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->validate_servant_manager (this->servant_activator_.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

    TAO_SERVANT_LOCATION
    Servant_Activator_Request_Processing_Strategy::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant
      ACE_ENV_ARG_DECL)
    {
      TAO_SERVANT_LOCATION location = TAO_SERVANT_NOT_FOUND;

      location = this->poa_->servant_present (system_id,
                                              servant
                                              ACE_ENV_ARG_PARAMETER);

      if (location == TAO_SERVANT_NOT_FOUND)
        {
          if (!CORBA::is_nil (this->servant_activator_.in ()))
            {
              location = TAO_SERVANT_MANAGER;
            }
        }

      return location;
    }

    PortableServer::Servant
    Servant_Activator_Request_Processing_Strategy::locate_servant (
      const char */*operation*/,
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
      int &wait_occurred_restart_call
      ACE_ENV_ARG_DECL)
    {
      PortableServer::Servant servant = 0;

      servant = this->poa_->find_servant (system_id,
                                          servant_upcall,
                                          poa_current_impl
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (servant != 0)
        {
          return servant;
        }

      // If the POA has the USE_SERVANT_MANAGER policy, a servant manager
      // has been associated with the POA so the POA will invoke incarnate
      // or preinvoke on it to find a servant that may handle the
      // request. (The choice of method depends on the NON_RETAIN or
      // RETAIN policy of the POA.) If no servant manager has been
      // associated with the POA, the POA raises the OBJ_ADAPTER system
      // exception.
      //
      // If a servant manager is located and invoked, but the servant
      // manager is not directly capable of incarnating the object, it
      // (the servant manager) may deal with the circumstance in a variety
      // of ways, all of which are the application's responsibility.  Any
      // system exception raised by the servant manager will be returned
      // to the client in the reply. In addition to standard CORBA
      // exceptions, a servant manager is capable of raising a
      // ForwardRequest exception. This exception includes an object
      // reference.
      //

      this->validate_servant_manager (this->servant_activator_.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      servant =
        this->incarnate_servant (poa_current_impl.object_id () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      int error = 0;

      // If the incarnate operation returns a servant that is
      // already active for a different Object Id and if the POA
      // also has the UNIQUE_ID policy, the incarnate has violated
      // the POA policy and is considered to be in error. The POA
      // will raise an OBJ_ADAPTER system exception for the
      // request.
      bool may_activate =
        this->poa_->is_servant_activation_allowed (servant, wait_occurred_restart_call);

      if (!may_activate)
        {
          error = 1;
        }

      // The POA enters the returned Servant value into the Active
      // Object Map so that subsequent requests with the same
      // ObjectId value will be delivered directly to that servant
      // without invoking the servant manager.  Only run if there
      // are no errors or if a restart is not required.
      if (!error && !wait_occurred_restart_call)
        {
          int result =
            this->poa_->
              rebind_using_user_id_and_system_id (servant,
                                                  poa_current_impl.object_id (),
                                                  system_id,
                                                  servant_upcall);
          if (result != 0)
            error = 1;
        }

      // If error occurred or a restart is required, etherealize
      // the incarnated servant.
      if (error || wait_occurred_restart_call)
        {
          CORBA::Boolean cleanup_in_progress = 0;
          this->etherealize_servant (poa_current_impl.object_id (),
                                     servant,
                                     cleanup_in_progress
                                     ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          // If error, throw exception.
          if (error)
            {
              ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                                0);
            }
          else
            {
              // We ended up waiting on a condition variable, the
              // POA state may have changed while we are waiting.
              // Therefore, we need to restart this call.
              return 0;
            }
        }
      else
        {
          // Increment the reference count on the servant upcall.
          servant_upcall.increment_servant_refcount ();
        }

      // Success
      return servant;
    }

    void
    Servant_Activator_Request_Processing_Strategy::etherealize_servant (
      const PortableServer::ObjectId& object_id,
      PortableServer::Servant servant,
      CORBA::Boolean cleanup_in_progress
      ACE_ENV_ARG_DECL)
    {
      CORBA::Boolean remaining_activations =
        this->poa_->servant_has_remaining_activations (servant);

      // ATTENTION: Trick locking here, see class header for details
      TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*this->poa_);
      ACE_UNUSED_ARG (non_servant_upcall);

      // @todo This is not according to the spec. According to 11.3.6.2 at the
      // end when etherealize returns a system exception the POA ignores the
      // exception
      this->servant_activator_->etherealize (object_id,
                                             this->poa_,
                                             servant,
                                             cleanup_in_progress,
                                             remaining_activations
                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

    PortableServer::Servant
    Servant_Activator_Request_Processing_Strategy::incarnate_servant (
      const PortableServer::ObjectId& object_id
      ACE_ENV_ARG_DECL)
    {
      PortableServer::Servant servant = 0;

      // ATTENTION: Trick locking here, see class header for details
      TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*this->poa_);
      ACE_UNUSED_ARG (non_servant_upcall);

      // @@
      // Invocations of incarnate on the servant manager are serialized.
      // Invocations of etherealize on the servant manager are serialized.
      // Invocations of incarnate and etherealize on the servant manager are mutually exclusive.
      servant = this->servant_activator_->incarnate (object_id,
                                                     this->poa_
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (servant == 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (CORBA::OMGVMCID | 7,
                                                CORBA::COMPLETED_NO),
                                                0);
        }
      else
        {
          return servant;
        }
    }

    void
    Servant_Activator_Request_Processing_Strategy::cleanup_servant (
      const PortableServer::ObjectId& object_id,
      PortableServer::Servant servant,
      CORBA::Boolean cleanup_in_progress
      ACE_ENV_ARG_DECL)
    {
      if (!CORBA::is_nil (this->servant_activator_))
        {
          this->etherealize_servant (object_id,
                                     servant,
                                     cleanup_in_progress
                                     ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

