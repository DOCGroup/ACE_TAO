// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingStrategyAOMOnly.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_REQUESTPROCESSINGSTRATEGYAOMONLY_H
#define TAO_REQUESTPROCESSINGSTRATEGYAOMONLY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/RequestProcessingStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Location.h"
#include "tao/PortableServer/PortableServer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    /**
     * @class RequestProcessingStrategyAOMOnly
     *
     * Request Processing Strategy which only uses the Active Object Map (AOM)
     */
    class RequestProcessingStrategyAOMOnly
      : public RequestProcessingStrategy
    {
    public:
      RequestProcessingStrategyAOMOnly (void);

#if (TAO_HAS_MINIMUM_POA == 0)

      PortableServer::ServantManager_ptr
      get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      void
      set_servant_manager (PortableServer::ServantManager_ptr imgr
                           ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      void
      set_servant (PortableServer::Servant servant
                   ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

      PortableServer::Servant
      get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      virtual
      TAO_SERVANT_LOCATION
      locate_servant (const PortableServer::ObjectId &system_id,
                      PortableServer::Servant &servant
                      ACE_ENV_ARG_DECL);

      virtual
      PortableServer::Servant
      locate_servant (const char *operation,
                      const PortableServer::ObjectId &system_id,
                      TAO::Portable_Server::Servant_Upcall &servant_upcall,
                      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
                      bool &wait_occurred_restart_call
                      ACE_ENV_ARG_DECL);

      virtual PortableServer::Servant system_id_to_servant (
        const PortableServer::ObjectId &system_id
        ACE_ENV_ARG_DECL);

      virtual PortableServer::Servant id_to_servant (
        const PortableServer::ObjectId &id
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ObjectNotActive,
                           PortableServer::POA::WrongPolicy));

      virtual void cleanup_servant (
        PortableServer::Servant servant,
        const PortableServer::ObjectId &user_id
        ACE_ENV_ARG_DECL);

      virtual void etherealize_objects (CORBA::Boolean etherealize_objects);

      virtual PortableServer::ObjectId *servant_to_id (
        PortableServer::Servant servant
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ServantNotActive,
                           PortableServer::POA::WrongPolicy));

      virtual void post_invoke_servant_cleanup(
        const PortableServer::ObjectId &system_id,
        const TAO::Portable_Server::Servant_Upcall &servant_upcall);

      virtual ::PortableServer::RequestProcessingPolicyValue type() const;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_REQUESTPROCESSINGSTRATEGYAOMONLY_H */
