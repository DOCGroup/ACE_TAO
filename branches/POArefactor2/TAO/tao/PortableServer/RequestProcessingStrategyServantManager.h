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

#ifndef TAO_REQUESTPROCESSINGSTRATEGYSERVANTMANAGER_H
#define TAO_REQUESTPROCESSINGSTRATEGYSERVANTMANAGER_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "RequestProcessingStrategy.h"

#include "Servant_Location.h"

#define TAO_PORTABLESERVER_SAFE_INCLUDE
#include "PortableServerC.h"
#undef TAO_PORTABLESERVER_SAFE_INCLUDE

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Servant_Manager_Request_Processing_Strategy :
       public virtual RequestProcessingStrategy
    {
    public:
      virtual ~Servant_Manager_Request_Processing_Strategy (void);

      PortableServer::Servant
      get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      void
      set_servant (PortableServer::Servant servant
                   ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_REQUESTPROCESSINGSTRATEGYSERVANTMANAGER_H */
