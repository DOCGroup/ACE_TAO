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
#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/PortableServer/RequestProcessingStrategyServantLocator.h"
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
    Servant_Locator_Request_Processing_Strategy::Servant_Locator_Request_Processing_Strategy (void) :
      poa_ (0)
    {
    }

    Servant_Locator_Request_Processing_Strategy::~Servant_Locator_Request_Processing_Strategy (void)
    {
    }

    void
    Servant_Locator_Request_Processing_Strategy::strategy_init(
      TAO_POA *poa,
      ServantRetentionStrategy* servant_retention_strategy)
    {
      ACE_UNUSED_ARG (servant_retention_strategy);
      poa_ = poa;
    }

    PortableServer::ServantManager_ptr
    Servant_Locator_Request_Processing_Strategy::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      return PortableServer::ServantManager::_duplicate (this->servant_locator_.in ());
    }

    void
    Servant_Locator_Request_Processing_Strategy::set_servant_manager (PortableServer::ServantManager_ptr imgr
                         ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      // This operation sets the default servant manager associated with the
      // POA. This operation may only be invoked once after a POA has been
      // created. Attempting to set the servant manager after one has already
      // been set will result in the BAD_INV_ORDER system exception with
      // standard minor code 6 being raised (see 11.3.9.12 of the corba spec)
      if (!CORBA::is_nil (this->servant_locator_.in ()))
        {
          ACE_THROW (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 6,
                                           CORBA::COMPLETED_NO));
        }

      this->servant_locator_ = PortableServer::ServantLocator::_narrow (imgr
                                                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->validate_servant_manager (this->servant_locator_.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

    TAO_SERVANT_LOCATION
    Servant_Locator_Request_Processing_Strategy::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      ACE_UNUSED_ARG (system_id);
      ACE_UNUSED_ARG (servant);

      if (CORBA::is_nil (this->servant_locator_.in ()))
        {
          return TAO_SERVANT_NOT_FOUND;
        }
      else
        {
          // Success
          return TAO_SERVANT_MANAGER;
        }
    }

    PortableServer::Servant
    Servant_Locator_Request_Processing_Strategy::locate_servant (
      const char *operation,
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
      int &wait_occurred_restart_call
      ACE_ENV_ARG_DECL)
    {
      ACE_UNUSED_ARG (wait_occurred_restart_call);
      ACE_UNUSED_ARG (system_id);

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

      this->validate_servant_manager (this->servant_locator_.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // No serialization of invocations of preinvoke or
      // postinvoke may be assumed; there may be multiple
      // concurrent invocations of preinvoke for the same
      // ObjectId.
      //
      // The same thread will be used to preinvoke the object,
      // process the request, and postinvoke the object.

      // @@ Note that it is possible for some other thread to
      // reset the servant locator once the lock is released.
      // However, this possiblility also exists for postinvoke()
      // which is also called outside the lock.

      // Release the object adapter lock.
      this->poa_->object_adapter().lock ().release ();

      // We have released the object adapter lock.  Record this
      // for later use.
      servant_upcall.state (TAO::Portable_Server::Servant_Upcall::OBJECT_ADAPTER_LOCK_RELEASED);

      PortableServer::ServantLocator::Cookie cookie = 0;
      PortableServer::Servant servant =
        this->servant_locator_->preinvoke (poa_current_impl.object_id (),
                                           this->poa_,
                                           operation,
                                           cookie
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (servant == 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (CORBA::OMGVMCID | 7,
                                                CORBA::COMPLETED_NO),
                                                0);
        }

      // Remember to invoke <postinvoke> on the given locator
      servant_upcall.servant_locator (servant_locator_.in());

      // Remember the cookie
      servant_upcall.locator_cookie (cookie);

      // Remember operation name.
      servant_upcall.operation (operation);

      // Success
      return servant;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

