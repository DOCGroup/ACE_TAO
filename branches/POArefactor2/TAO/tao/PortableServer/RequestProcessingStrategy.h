// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingStrategy.h
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
#include "Policy_Strategy.h"
#include "PortableServerC.h"
#include "ace/Service_Config.h"

#include "Servant_Location.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace PortableServer
{
  class ServantManager;
  typedef ServantManager *ServantManager_ptr;
}

namespace TAO
{
  namespace Portable_Server
  {
    class Servant_Upcall;
    class POA_Current_Impl;
  }
}

class TAO_POA;

namespace TAO
{
  namespace Portable_Server
  {
    class ServantRetentionStrategy;

    class TAO_PortableServer_Export RequestProcessingStrategy
      : public virtual Policy_Strategy
    {
    public:
      RequestProcessingStrategy (void);

      virtual ~RequestProcessingStrategy (void);

      virtual void strategy_init(TAO_POA *poa ACE_ENV_ARG_DECL);

      virtual void strategy_cleanup(ACE_ENV_SINGLE_ARG_DECL);

      virtual PortableServer::ServantManager_ptr get_servant_manager (
        ACE_ENV_SINGLE_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual void set_servant_manager (
        PortableServer::ServantManager_ptr imgr
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy)) = 0;

      virtual PortableServer::Servant get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy)) = 0;

      virtual void set_servant (PortableServer::Servant servant ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy)) = 0;

      virtual TAO_SERVANT_LOCATION locate_servant (
        const PortableServer::ObjectId &system_id,
        PortableServer::Servant &servant
        ACE_ENV_ARG_DECL) = 0;

      virtual PortableServer::Servant locate_servant (const char *operation,
                      const PortableServer::ObjectId &system_id,
                      TAO::Portable_Server::Servant_Upcall &servant_upcall,
                      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
                      int &wait_occurred_restart_call
                      ACE_ENV_ARG_DECL) = 0;

      virtual void cleanup_servant (
        PortableServer::Servant servant,
        PortableServer::ObjectId user_id
        ACE_ENV_ARG_DECL) = 0;

      /**
       * @todo Check usage of reference, it is not used, so it could be just
       * system_id, but then change the method name to system_id_to_servant,
       * or something like that
       */
      virtual PortableServer::Servant reference_to_servant (
        CORBA::Object_ptr reference,
        PortableServer::ObjectId system_id
        ACE_ENV_ARG_DECL) = 0;

      virtual void etherealize_objects (CORBA::Boolean etherealize_objects) = 0;

      virtual PortableServer::ObjectId *servant_to_id (
        PortableServer::Servant servant
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ServantNotActive,
                           PortableServer::POA::WrongPolicy)) = 0;
    protected:
      TAO_POA* poa_;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_REQUEST_PROCESSING_STRATEGY_H */
