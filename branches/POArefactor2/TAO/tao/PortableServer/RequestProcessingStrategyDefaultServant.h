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

#ifndef TAO_REQUESTPROCESSINGSTRATEGYDEFAULTSERVANT_H
#define TAO_REQUESTPROCESSINGSTRATEGYDEFAULTSERVANT_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "RequestProcessingStrategy.h"
#include "Servant_Base.h"

#include "Servant_Location.h"

#define TAO_PORTABLESERVER_SAFE_INCLUDE
#include "PortableServerC.h"
#undef TAO_PORTABLESERVER_SAFE_INCLUDE

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

class TAO_POA;

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Default_Servant_Request_Processing_Strategy
      : public virtual RequestProcessingStrategy
    {
    public:
      Default_Servant_Request_Processing_Strategy (void);

      virtual ~Default_Servant_Request_Processing_Strategy (void);

      virtual void strategy_init(TAO_POA *poa);

      PortableServer::ServantManager_ptr
      get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      void set_servant_manager (
        PortableServer::ServantManager_ptr imgr
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy));

      PortableServer::Servant get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy));

      void set_servant (
        PortableServer::Servant servant
        ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::SystemException,
                           PortableServer::POA::WrongPolicy));

      virtual TAO_SERVANT_LOCATION locate_servant (
        const PortableServer::ObjectId &system_id,
        PortableServer::Servant &servant
        ACE_ENV_ARG_DECL);

      virtual PortableServer::Servant locate_servant (
        const char *operation,
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall,
        TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
        int &wait_occurred_restart_call
        ACE_ENV_ARG_DECL);

      virtual void cleanup_servant (
        const PortableServer::ObjectId& object_id,
        PortableServer::Servant servant,
        CORBA::Boolean cleanup_in_progress
        ACE_ENV_ARG_DECL);

      virtual PortableServer::Servant reference_to_servant (
        CORBA::Object_ptr reference,
        PortableServer::ObjectId system_id
        ACE_ENV_ARG_DECL);
    private:
      TAO_POA* poa_;
      PortableServer::ServantBase_var default_servant_;
    };

  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_REQUESTPROCESSINGSTRATEGYDEFAULTSERVANT_H */
