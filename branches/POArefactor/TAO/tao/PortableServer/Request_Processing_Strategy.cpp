// -*- C++ -*-

//=============================================================================
/**
 *  @file    Request_Processing_Strategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "Request_Processing_Strategy.h"
#include "ServantActivatorC.h"
#include "ServantLocatorC.h"

ACE_RCSID (PortableServer,
           Request_Processing,
           "$Id$")


#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Request_Processing_Strategy::~Request_Processing_Strategy()
    {
    }

    void
    Request_Processing_Strategy::strategy_init(CORBA::PolicyList *policy_list)
    {
      // dependent on type create the correct strategy.
    }

    AOM_Only_Request_Processing_Strategy::~AOM_Only_Request_Processing_Strategy()
    {
    }

    PortableServer::ServantManager_ptr
    AOM_Only_Request_Processing_Strategy::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        PortableServer::ServantManager::_nil ());
    }

      void
      AOM_Only_Request_Processing_Strategy::set_servant_manager (PortableServer::ServantManager_ptr imgr
                           ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
      {
        ACE_UNUSED_ARG (imgr);

        ACE_THROW (PortableServer::POA::WrongPolicy ());
      }

      PortableServer::Servant
      AOM_Only_Request_Processing_Strategy::get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::NoServant,
                         PortableServer::POA::WrongPolicy))
      {
        ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                          0);
      }

      void
      AOM_Only_Request_Processing_Strategy::set_servant (PortableServer::Servant servant
                   ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
      {
        ACE_UNUSED_ARG (servant);

        ACE_THROW (PortableServer::POA::WrongPolicy ());
      }

      TAO_SERVANT_LOCATION
      AOM_Only_Request_Processing_Strategy::locate_servant (const PortableServer::ObjectId &system_id,
                        PortableServer::Servant &servant
                        ACE_ENV_ARG_DECL)
      {
        // todo code from POA::locate_servant_i, should handle this dependent on servant_retation
        /**

  if (this->cached_policies_.servant_retention () == PortableServer::RETAIN)
    {
      // Find user id from system id.
      PortableServer::ObjectId user_id;
      if (this->active_object_map ().
          find_user_id_using_system_id (system_id,
                                        user_id) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            TAO_SERVANT_NOT_FOUND);
        }

      TAO_Active_Object_Map::Map_Entry *entry = 0;
      int result = this->active_object_map ().
        find_servant_using_system_id_and_user_id (system_id,
                                                  user_id,
                                                  servant,
                                                  entry);
      if (result == 0)
        {
          // Success
          return TAO_SERVANT_FOUND;
        }

    }
      return TAO_SERVANT_NOT_FOUND;

         */
    }

    Default_Servant_Request_Processing_Strategy::~Default_Servant_Request_Processing_Strategy (void)
    {
    }

    PortableServer::ServantManager_ptr
    Default_Servant_Request_Processing_Strategy::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        PortableServer::ServantManager::_nil ());
    }

    void
    Default_Servant_Request_Processing_Strategy::set_servant_manager (PortableServer::ServantManager_ptr imgr
                         ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (imgr);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    PortableServer::Servant
    Default_Servant_Request_Processing_Strategy::get_servant (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::NoServant,
                       PortableServer::POA::WrongPolicy))
    {
      // This operation returns the default servant associated with the
      // POA.
      PortableServer::Servant result = this->default_servant_.in ();
      if (result != 0)
        {
// todo
/*          // A recursive thread lock without using a recursive thread
          // lock.  Non_Servant_Upcall has a magic constructor and
          // destructor.  We unlock the Object_Adapter lock for the
          // duration of the servant activator upcalls; reacquiring once
          // the upcalls complete.  Even though we are releasing the lock,
          // other threads will not be able to make progress since
          // <Object_Adapter::non_servant_upcall_in_progress_> has been
          // set.
          TAO_Object_Adapter::Non_Servant_Upcall non_servant_upcall (*this);
          ACE_UNUSED_ARG (non_servant_upcall);

          // The POA invokes _add_ref once on the Servant before returning
          // it. If the application uses reference counting, the caller of
          // get_servant is responsible for invoking _remove_ref once on
          // the returned Servant when it is finished with it. A
          // conforming caller need not invoke _remove_ref on the returned
          // Servant if the type of the Servant uses the default reference
          // counting inherited from ServantBase.
          result->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);*/

          return result;
        }
      else
        // If no servant has been associated with the POA, the NoServant
        // exception is raised.
        {
          ACE_THROW_RETURN (PortableServer::POA::NoServant (),
                            0);
        }
    }

    void
    Default_Servant_Request_Processing_Strategy::set_servant (PortableServer::Servant servant
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
// todo
/*
          TAO_Object_Adapter::Non_Servant_Upcall non_servant_upcall (*this);
          ACE_UNUSED_ARG (non_servant_upcall);

          servant->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;*/
        }
    }

    TAO_SERVANT_LOCATION
    Default_Servant_Request_Processing_Strategy::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant
      ACE_ENV_ARG_DECL)
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

    Servant_Manager_Request_Processing_Strategy::~Servant_Manager_Request_Processing_Strategy (void)
    {
    }

    PortableServer::Servant
    Servant_Manager_Request_Processing_Strategy::get_servant (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::NoServant,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

    void
    Servant_Manager_Request_Processing_Strategy::set_servant (PortableServer::Servant servant
                 ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (servant);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    Servant_Activator_Request_Processing_Strategy::~Servant_Activator_Request_Processing_Strategy (void)
    {
    }

    PortableServer::ServantManager_ptr
    Servant_Activator_Request_Processing_Strategy::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      return PortableServer::ServantManager::_duplicate (this->servant_activator_.in ());
    }

    void
    Servant_Activator_Request_Processing_Strategy::set_servant_manager (PortableServer::ServantManager_ptr imgr
                         ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      this->servant_activator_ = PortableServer::ServantActivator::_narrow (imgr
                                                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // @bala Shouldn't we use an extra variable, in case a set_servant_manager
      // wit a wrong type, we set this wrong type and after that throw the exception
      // shouldn't we keep the servant_activator_ valid?
      if (CORBA::is_nil (this->servant_activator_.in ()))
        {
          ACE_THROW (PortableServer::POA::WrongPolicy ());
        }
    }

    TAO_SERVANT_LOCATION
    Servant_Activator_Request_Processing_Strategy::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant
      ACE_ENV_ARG_DECL)
    {
      ACE_UNUSED_ARG (system_id);
      ACE_UNUSED_ARG (servant);

      if (CORBA::is_nil (this->servant_activator_.in ()))
        {
          return TAO_SERVANT_NOT_FOUND;
        }
      else
        {
          // Success
          return TAO_SERVANT_MANAGER;
        }
    }

    Servant_Locator_Request_Processing_Strategy::~Servant_Locator_Request_Processing_Strategy (void)
    {
    }

    PortableServer::ServantManager_ptr
    Servant_Locator_Request_Processing_Strategy::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
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
      this->servant_locator_ = PortableServer::ServantLocator::_narrow (imgr
                                                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // @bala Shouldn't we use an extra variable, in case a set_servant_manager
      // wit a wrong type, we set this wrong type and after that throw the exception
      // shouldn't we keep the servant_activator_ valid?
      if (CORBA::is_nil (this->servant_locator_.in ()))
        {
          ACE_THROW (PortableServer::POA::WrongPolicy ());
        }
    }

    TAO_SERVANT_LOCATION
    Servant_Locator_Request_Processing_Strategy::locate_servant (const PortableServer::ObjectId &system_id,
                      PortableServer::Servant &servant
                      ACE_ENV_ARG_DECL)
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
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

