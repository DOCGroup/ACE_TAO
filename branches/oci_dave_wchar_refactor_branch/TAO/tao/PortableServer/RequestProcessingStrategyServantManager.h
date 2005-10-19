// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingStrategyServantManager.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_REQUESTPROCESSINGSTRATEGYSERVANTMANAGER_H
#define TAO_REQUESTPROCESSINGSTRATEGYSERVANTMANAGER_H
#include /**/ "ace/pre.h"

#include "RequestProcessingStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Servant_Location.h"
#include "PortableServer.h"

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class RequestProcessingStrategyServantManager
      : public RequestProcessingStrategy
    {
    public:
      RequestProcessingStrategyServantManager (void);

      PortableServer::Servant get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      void set_servant (PortableServer::Servant servant ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      void validate_servant_manager (
        PortableServer::ServantManager_ptr servant_manager
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

      virtual PortableServer::ObjectId *servant_to_id (
        PortableServer::Servant servant
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::ServantNotActive,
                           PortableServer::POA::WrongPolicy));

      virtual ::PortableServer::RequestProcessingPolicyValue type() const;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_REQUESTPROCESSINGSTRATEGYSERVANTMANAGER_H */
