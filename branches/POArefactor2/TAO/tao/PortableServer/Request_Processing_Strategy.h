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

#ifndef TAO_REQUEST_PROCESSING_STRATEGY_H
#define TAO_REQUEST_PROCESSING_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "PolicyFactory.h"
#include "PortableServerC.h"
#include "Policy_Strategy.h"
#include "ace/Service_Config.h"

#include "Object_Adapter.h" // for TAO_SERVANT_LOCATION, move this enum to another file

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace CORBA
{
  class PolicyError;
  class PolicyList;
}

namespace PortableServer
{
  class ServantActivator;

  typedef ServantActivator *ServantActivator_ptr;
  typedef TAO_Objref_Var_T<ServantActivator> ServantActivator_var;

  class ServantLocator;

  typedef ServantLocator *ServantLocator_ptr;
  typedef TAO_Objref_Var_T<ServantLocator> ServantLocator_var;
}

class TAO_POA;

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Request_Processing_Strategy :
       public virtual Policy_Strategy
    {
    public:
      virtual ~Request_Processing_Strategy (void);

      void strategy_init(TAO_POA *poa, CORBA::PolicyList *policy_list);

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

      virtual
      TAO_SERVANT_LOCATION
      locate_servant (const PortableServer::ObjectId &system_id,
                      PortableServer::Servant &servant
                      ACE_ENV_ARG_DECL) = 0;
    protected:
      TAO_POA* poa_;
    };

    class TAO_PortableServer_Export AOM_Only_Request_Processing_Strategy :
       public virtual Request_Processing_Strategy
    {
    public:
      virtual ~AOM_Only_Request_Processing_Strategy (void);

      PortableServer::ServantManager_ptr
      get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      void
      set_servant_manager (PortableServer::ServantManager_ptr imgr
                           ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      PortableServer::Servant
      get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::NoServant,
                         PortableServer::POA::WrongPolicy));

      void
      set_servant (PortableServer::Servant servant
                   ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      virtual
      TAO_SERVANT_LOCATION
      locate_servant (const PortableServer::ObjectId &system_id,
                        PortableServer::Servant &servant
                        ACE_ENV_ARG_DECL);

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
                         PortableServer::POA::WrongPolicy));

      void
      set_servant_manager (PortableServer::ServantManager_ptr imgr
                           ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      PortableServer::Servant
      get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::NoServant,
                         PortableServer::POA::WrongPolicy));

      void
      set_servant (PortableServer::Servant servant
                   ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      virtual
      TAO_SERVANT_LOCATION
      locate_servant (const PortableServer::ObjectId &system_id,
                        PortableServer::Servant &servant
                        ACE_ENV_ARG_DECL);

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
                         PortableServer::POA::WrongPolicy));

      void
      set_servant (PortableServer::Servant servant
                   ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));
    };

    class TAO_PortableServer_Export Servant_Activator_Request_Processing_Strategy :
       public virtual Servant_Manager_Request_Processing_Strategy
    {
    public:
      virtual ~Servant_Activator_Request_Processing_Strategy (void);

      PortableServer::ServantManager_ptr
      get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      void
      set_servant_manager (PortableServer::ServantManager_ptr imgr
                           ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      virtual
      TAO_SERVANT_LOCATION
      locate_servant (const PortableServer::ObjectId &system_id,
                        PortableServer::Servant &servant
                        ACE_ENV_ARG_DECL);

    private:
      PortableServer::ServantActivator_var servant_activator_;
    };

    class TAO_PortableServer_Export Servant_Locator_Request_Processing_Strategy :
       public virtual Servant_Manager_Request_Processing_Strategy
    {
    public:
      virtual ~Servant_Locator_Request_Processing_Strategy (void);

      PortableServer::ServantManager_ptr
      get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      void
      set_servant_manager (PortableServer::ServantManager_ptr imgr
                           ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      virtual
      TAO_SERVANT_LOCATION
      locate_servant (const PortableServer::ObjectId &system_id,
                        PortableServer::Servant &servant
                        ACE_ENV_ARG_DECL);

    private:
      PortableServer::ServantLocator_var servant_locator_;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_REQUEST_PROCESSING_STRATEGY_H */
