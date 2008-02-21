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

#include "tao/PortableServer/RequestProcessingStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Location.h"
#include "tao/PortableServer/PortableServer.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class RequestProcessingStrategyServantManager
      : public RequestProcessingStrategy
    {
    public:
      RequestProcessingStrategyServantManager (void);

      PortableServer::Servant get_servant (void);

      void set_servant (PortableServer::Servant servant);

      void validate_servant_manager (
        PortableServer::ServantManager_ptr servant_manager);

      virtual PortableServer::Servant system_id_to_servant (
        const PortableServer::ObjectId &system_id);

      virtual PortableServer::Servant id_to_servant (
        const PortableServer::ObjectId &id);

      virtual PortableServer::ObjectId *servant_to_id (
        PortableServer::Servant servant);

      virtual ::PortableServer::RequestProcessingPolicyValue type() const;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_REQUESTPROCESSINGSTRATEGYSERVANTMANAGER_H */
