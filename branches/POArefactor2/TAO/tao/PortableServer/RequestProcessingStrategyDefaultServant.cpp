// -*- C++ -*-

//=============================================================================
/**
 *  @file    Request_Processing_Strategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "tao/PortableServer/RequestProcessingStrategyDefaultServant.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/POA.h"
#include "tao/PortableServer/ServantManagerC.h"
#include "tao/PortableServer/Servant_Base.h"

ACE_RCSID (PortableServer,
           Request_Processing,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Default_Servant_Request_Processing_Strategy::Default_Servant_Request_Processing_Strategy (void) :
      poa_ (0)
    {
    }

    Default_Servant_Request_Processing_Strategy::~Default_Servant_Request_Processing_Strategy (void)
    {
      this->default_servant_ = 0;
    }

    void
    Default_Servant_Request_Processing_Strategy::strategy_init(
      TAO_POA *poa,
      ServantRetentionStrategy* servant_retention_strategy)
    {
      ACE_UNUSED_ARG (servant_retention_strategy);
      poa_ = poa;
    }

    PortableServer::ServantManager_ptr
    Default_Servant_Request_Processing_Strategy::get_servant_manager (
      ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        PortableServer::ServantManager::_nil ());
    }

    void
    Default_Servant_Request_Processing_Strategy::set_servant_manager (
      PortableServer::ServantManager_ptr imgr
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (imgr);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    PortableServer::Servant
    Default_Servant_Request_Processing_Strategy::get_servant (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      // This operation returns the default servant associated with the
      // POA.
      return this->default_servant_.in ();
    }

    void
    Default_Servant_Request_Processing_Strategy::set_servant (
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
    Default_Servant_Request_Processing_Strategy::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      ACE_UNUSED_ARG (system_id);
      ACE_UNUSED_ARG (servant);

      if (this->default_servant_.in () == 0)
        {
          return TAO_SERVANT_NOT_FOUND;
        }
      else
        {
          return TAO_DEFAULT_SERVANT;
        }
    }

    PortableServer::Servant
    Default_Servant_Request_Processing_Strategy::locate_servant (
      const char *operation,
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
      int &wait_occurred_restart_call
      ACE_ENV_ARG_DECL)
    {
      ACE_UNUSED_ARG (operation);
      ACE_UNUSED_ARG (system_id);
      ACE_UNUSED_ARG (servant_upcall);
      ACE_UNUSED_ARG (poa_current_impl);
      ACE_UNUSED_ARG (wait_occurred_restart_call);

      // If the POA has the USE_DEFAULT_SERVANT policy, a default servant
      // has been associated with the POA so the POA will invoke the
      // appropriate method on that servant. If no servant has been
      // associated with the POA, the POA raises the OBJ_ADAPTER system
      // exception.
      PortableServer::Servant result = this->default_servant_.in ();
      if (result == 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }
      else
        {
          // Success
          return result;
        }
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

