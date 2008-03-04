// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_Strategy_Base.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_FW_CUSTOM_SERVANT_DISPATCHING_STRATEGY_H
#define TAO_CSD_FW_CUSTOM_SERVANT_DISPATCHING_STRATEGY_H

#include /**/ "ace/pre.h"

#include "tao/CSD_Framework/CSD_FW_Export.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Upcall.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/TAO_Server_Request.h"
#include "tao/LocalObject.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_Framework/CSD_FrameworkC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Root_POA;
namespace PortableServer
{
  class POAManager;
}

namespace TAO
{
  namespace CSD
  {
    /**
    * @class Strategy_Base
    *
    * @brief Base class for all Custom Servant Dispatching Strategies.
    *
    * This class serves as the base class for all "custom" strategies that
    * perform servant dispatching.  An instance of (a subclass of) this class
    * can be applied to a POA object.  Any servant requests for the POA will
    * be "dispatched" to this strategy object.
    *
    */
    class TAO_CSD_FW_Export Strategy_Base
      : public CSD_Framework::Strategy,
        public TAO_Local_RefCounted_Object
    {
    public:

      /// Result Type for dispatching method(s).
      enum DispatchResult
      {
        // The request dispatching has been handled.
        DISPATCH_HANDLED,

        // The request dispatching has been rejected.
        DISPATCH_REJECTED,

        // Defer to "default" dispatching strategy (use the caller's thread).
        DISPATCH_DEFERRED
      };

      /// Virtual Destructor.
      virtual ~Strategy_Base();

      /// This method is invoked to "attach" this strategy object to
      /// the supplied POA.  Returns true for success, and false for failure.
      CORBA::Boolean apply_to(PortableServer::POA_ptr poa );

    protected:
      /// Default Constructor.
      Strategy_Base();

      /// Subclass provides implementation to dispatch a remote request.
      virtual DispatchResult dispatch_remote_request_i
                            (TAO_ServerRequest&              server_request,
                              const PortableServer::ObjectId& object_id,
                              PortableServer::POA_ptr         poa,
                              const char*                     operation,
                              PortableServer::Servant         servant) = 0;

      /// Subclass provides implementation to dispatch a collocated request.
      virtual DispatchResult dispatch_collocated_request_i
                          (TAO_ServerRequest&              server_request,
                          const PortableServer::ObjectId& object_id,
                          PortableServer::POA_ptr         poa,
                          const char*                     operation,
                          PortableServer::Servant         servant) = 0;

      /// Event - The POA has been activated.
      virtual bool poa_activated_event_i() = 0;

      /// Event - The POA has been deactivated.
      virtual void poa_deactivated_event_i() = 0;

      /// Event - A servant has been activated.
      virtual void servant_activated_event_i
                                  (PortableServer::Servant servant,
                                   const PortableServer::ObjectId& oid);

      /// Event - A servant has been deactivated.
      virtual void servant_deactivated_event_i
                                  (PortableServer::Servant servant,
                                  const PortableServer::ObjectId& oid);

    private:

      /// Only our friend, the proxy, is allowed to invoke our private operations.
      /// This allows us to not pollute the public interface of the CSD Strategy_Base
      /// subclasses with methods that should never be called (except by the
      /// proxy, of course).
      friend class Strategy_Proxy;

      /// This CSD Strategy_Base has been asked to dispatch a (collocated or remote)
      /// request.
      void dispatch_request(TAO_ServerRequest& server_request,
                            ::TAO::Portable_Server::Servant_Upcall& upcall);

      /// Event - The POA has been activated. This happens when the POA_Manager
      ///         is activated.
      bool poa_activated_event();

      /// Event - The POA has been deactivated.  This happens when the
      ///         POAManager is deactivated, or when the POA is destroyed.
      void poa_deactivated_event();

      /// Event - A servant has been activated.
      void servant_activated_event(PortableServer::Servant servant,
                                  const PortableServer::ObjectId& oid);

      /// Event - A servant has been deactivated.  This also occurs when
      ///         the POA is destroyed.
      void servant_deactivated_event(PortableServer::Servant servant,
                                     const PortableServer::ObjectId& oid);

      /// The POA to which this strategy has been applied.
      ::PortableServer::POA_var poa_;

      /// This flag indicates that the POA is currently active (true) or
      /// currently inactive (false).
      bool poa_activated_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_Framework/CSD_Strategy_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_FW_CUSTOM_SERVANT_DISPATCHING_STRATEGY_H */
