// -*- C++ -*-

//=============================================================================
/**
 *  @file Request_Processing_Strategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ID_ASSIGNMENT_STRATEGY_H
#define TAO_ID_ASSIGNMENT_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "PolicyFactory.h"
#include "PortableServerC.h"
#include "PolicyStrategy.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace CORBA
{
  class PolicyError;
  class PolicyList;
}

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Request_Processing_Strategy :
       public virtual Policy_Strategy
    {
    public:
      virtual ~Thread_Strategy (void);

      void init(CORBA::PolicyList *policy_list)
      {
        // dependent on type create the correct strategy.
      }

      virtual
      PortableServer::ServantManager_ptr
      get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy)) = 0;

      virtual
      void
      set_servant_manager (PortableServer::ServantManager_ptr imgr
                           ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy)) = 0;

      virtual
      PortableServer::Servant
      get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::NoServant,
                         PortableServer::POA::WrongPolicy)) = 0;

      virtual
      void
      set_servant (PortableServer::Servant servant
                   ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy)) = 0;
    };

    class TAO_PortableServer_Export AOM_Only_Request_Processing_Strategy :
       public virtual Request_Processing_Strategy
    {
    public:
      virtual ~AOM_Only_Request_Processing_Strategy (void);

      PortableServer::ServantManager_ptr
      get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
      {
        ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                          PortableServer::ServantManager::_nil ());
      }

      void
      set_servant_manager (PortableServer::ServantManager_ptr imgr
                           ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
      {
        ACE_THROW (PortableServer::POA::WrongPolicy ());
      }

      PortableServer::Servant
      get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::NoServant,
                         PortableServer::POA::WrongPolicy))
      {
        ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                          PortableServer::Servant::_nil ());
      }

      void
      set_servant (PortableServer::Servant servant
                   ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
      {
        ACE_UNUSED_ARG (servant);

        ACE_THROW (PortableServer::POA::WrongPolicy ());
      }

    private:
    };

    class TAO_PortableServer_Export Default_Servant_Request_Processing_Strategy :
       public virtual Request_Processing_Strategy
    {
    public:
      virtual ~Default_Servant_Request_Processing_Strategy (void);

      PortableServer::ServantManager_ptr
      get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
      {
        ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                          PortableServer::ServantManager::_nil ());
      }

      void
      set_servant_manager (PortableServer::ServantManager_ptr imgr
                           ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
      {
        ACE_THROW (PortableServer::POA::WrongPolicy ());
      }

      PortableServer::Servant
      get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::NoServant,
                         PortableServer::POA::WrongPolicy))
      {
        // This operation returns the default servant associated with the
        // POA.
        PortableServer::Servant result = this->default_servant_.in ();
        if (result != 0)
          {
            // A recursive thread lock without using a recursive thread
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
            ACE_CHECK_RETURN (0);

            return result;
          }
        else
          // If no servant has been associated with the POA, the NoServant
          // exception is raised.
          {
            ACE_THROW_RETURN (PortableServer::POA::NoServant (),
                              PortableServer::Servant::_nil());
          }
      }

      void
      set_servant (PortableServer::Servant servant
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
            TAO_Object_Adapter::Non_Servant_Upcall non_servant_upcall (*this);
            ACE_UNUSED_ARG (non_servant_upcall);

            servant->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_CHECK;
          }
      }

    private:
      PortableServer::ServantBase_var default_servant_;
    };

    class TAO_PortableServer_Export Servant_Manager_Request_Processing_Strategy :
       public virtual Request_Processing_Strategy
    {
    public:
      virtual ~Servant_Manager_Request_Processing_Strategy (void);

      PortableServer::Servant
      get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::NoServant,
                         PortableServer::POA::WrongPolicy))
      {
        ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                          PortableServer::Servant::_nil ());
      }

      void
      set_servant (PortableServer::Servant servant
                   ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
      {
        ACE_UNUSED_ARG (servant);

        ACE_THROW (PortableServer::POA::WrongPolicy ());
      }

    };

    class TAO_PortableServer_Export Servant_Activator_Request_Processing_Strategy :
       public virtual Servant_Manager_Request_Processing_Strategy
    {
    public:
      virtual ~Servant_Manager_Request_Processing_Strategy (void);

      PortableServer::ServantManager_ptr
      get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
      {
        return PortableServer::ServantManager::_duplicate (this->servant_activator_.in ());
      }

      void
      set_servant_manager (PortableServer::ServantManager_ptr imgr
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

    private:
      PortableServer::ServantActivator_var servant_activator_;
    };

    class TAO_PortableServer_Export Servant_Locator_Request_Processing_Strategy :
       public virtual Servant_Manager_Request_Processing_Strategy
    {
    public:
      virtual ~Servant_Manager_Request_Processing_Strategy (void);

      PortableServer::ServantManager_ptr
      get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
      {
        return PortableServer::ServantManager::_duplicate (this->servant_locator_.in ());
      }

      void
      set_servant_manager (PortableServer::ServantManager_ptr imgr
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

    private:
      PortableServer::ServantLocator_var servant_locator_;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_ID_ASSIGNMENT_STRATEGY_H */
