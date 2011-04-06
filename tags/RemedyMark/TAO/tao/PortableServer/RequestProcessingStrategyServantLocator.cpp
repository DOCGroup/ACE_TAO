// $Id$

#include "tao/orbconf.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/ORB_Constants.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/PortableServer/RequestProcessingStrategyServantLocator.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/Servant_Upcall.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/Servant_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategyServantLocator::RequestProcessingStrategyServantLocator (void)
    {
    }

    void
    RequestProcessingStrategyServantLocator::strategy_cleanup(void)
    {
      {
        Non_Servant_Upcall non_servant_upcall (*this->poa_);
        ACE_UNUSED_ARG (non_servant_upcall);

        this->servant_locator_ = PortableServer::ServantLocator::_nil ();
      }

      RequestProcessingStrategy::strategy_cleanup ();
    }

    PortableServer::ServantManager_ptr
    RequestProcessingStrategyServantLocator::get_servant_manager (void)
    {
      return PortableServer::ServantManager::_duplicate (this->servant_locator_.in ());
    }

    void
    RequestProcessingStrategyServantLocator::set_servant_manager (
      PortableServer::ServantManager_ptr imgr)
    {
      // This operation sets the default servant manager associated with the
      // POA. This operation may only be invoked once after a POA has been
      // created. Attempting to set the servant manager after one has already
      // been set will result in the BAD_INV_ORDER system exception with
      // standard minor code 6 being raised (see 11.3.9.12 of the corba spec)
      if (!CORBA::is_nil (this->servant_locator_.in ()))
        {
          throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 6,
                                        CORBA::COMPLETED_NO);
        }

      this->servant_locator_ = PortableServer::ServantLocator::_narrow (imgr);

      this->validate_servant_manager (this->servant_locator_.in ());
    }

    TAO_SERVANT_LOCATION
    RequestProcessingStrategyServantLocator::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant)
    {
      TAO_SERVANT_LOCATION  location =
        this->poa_->servant_present (system_id, servant);

      if (location == TAO_SERVANT_NOT_FOUND)
        {
          if (!CORBA::is_nil (this->servant_locator_.in ()))
            {
              location = TAO_SERVANT_MANAGER;
            }
        }

      return location;
    }

    PortableServer::Servant
    RequestProcessingStrategyServantLocator::locate_servant (
      const char *operation,
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
      bool &/*wait_occurred_restart_call*/)
    {
      PortableServer::Servant servant = this->poa_->find_servant (system_id,
                                                                  servant_upcall,
                                                                  poa_current_impl);

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

      this->validate_servant_manager (this->servant_locator_.in ());

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
      servant =
        this->servant_locator_->preinvoke (poa_current_impl.object_id (),
                                           this->poa_,
                                           operation,
                                           cookie);

      if (servant == 0)
        {
          throw ::CORBA::OBJ_ADAPTER (CORBA::OMGVMCID | 7, CORBA::COMPLETED_NO);
        }

      // Remember the cookie
      servant_upcall.locator_cookie (cookie);

      // Remember operation name.
      servant_upcall.operation (operation);

      // Success
      return servant;
    }

    void
    RequestProcessingStrategyServantLocator::cleanup_servant (
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
    RequestProcessingStrategyServantLocator::etherealize_objects (
      CORBA::Boolean /*etherealize_objects*/)
    {
    }

    void
    RequestProcessingStrategyServantLocator::post_invoke_servant_cleanup(
      const PortableServer::ObjectId &system_id,
      const TAO::Portable_Server::Servant_Upcall &servant_upcall)
    {
      // @todo This method seems to misbehave according to the corba spec, see
      // section 11.3.7.2. It says that when postinvoke raises an system
      // exception the methods normal return is overrriden, the request completes
      // with the exception

      if (!CORBA::is_nil (this->servant_locator_.in ()) &&
          servant_upcall.servant())
        {
          try
            {
              servant_locator_->postinvoke (system_id,
                                            this->poa_,
                                            servant_upcall.operation (),
                                            servant_upcall.locator_cookie (),
                                            servant_upcall.servant ());
            }
          catch (const ::CORBA::Exception&)
            {
              // Ignore errors from servant locator ....
            }
        }
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */

