// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_Strategy_Proxy.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_SERVANT_DISPATCHING_STRATEGY_PROXY_H
#define TAO_SERVANT_DISPATCHING_STRATEGY_PROXY_H

#include /**/ "ace/pre.h"

#include "tao/CSD_Framework/CSD_FW_Export.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Upcall.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_Framework/CSD_Strategy_Base.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ServerRequest;

namespace TAO
{
  namespace CSD
  {
    /**
    * @class Strategy_Proxy
    *
    * @brief Proxy class for the Custom Servant Dispatching Strategy.
    *
    * If no custom servant dispatching strategy is provided to the proxy,
    * then the "default servant dispatching strategy" logic is used.
    */
    class TAO_CSD_FW_Export Strategy_Proxy
    {
    public:

      /// Default Constructor.
      Strategy_Proxy();

      /// Destructor.
      ~Strategy_Proxy();

      /// Mutator to provide the proxy with a CSD Strategy object.
      /// A return value of true indicates success, and false indicates
      /// failure to set the custom strategy on the proxy object.
      bool custom_strategy(CSD_Framework::Strategy_ptr strategy);

      /// Invoked by the Object_Adapter using an ORB thread.
      ///
      /// If the proxy object holds a custom strategy object, then this method
      /// will simply delegate to the custom strategy object.  Otherwise,
      /// this method will perform the "default servant dispatching strategy"
      /// logic, preserving the original logic path as it was prior to the
      /// introduction of the Custom Servant Dispatching feature.
      ///
      /// This method will be inlined (if inlining is turned on during the build).
      ///
      /// The added cost to the original logic path will be this method
      /// invocation + one conditional (an is_nil() call/comparison for truth on
      /// the smart pointer to the custom dispatching strategy object).
      void dispatch_request(TAO_ServerRequest& server_request,
                            TAO::Portable_Server::Servant_Upcall& upcall);


      /// Event - The POA has been (or is being) activated.
      bool poa_activated_event();

      /// Event - The POA has been deactivated.
      void poa_deactivated_event();

      /// Event - A servant has been activated.
      void servant_activated_event(PortableServer::Servant servant,
                                  const PortableServer::ObjectId& oid);

      /// Event - A servant has been deactivated.
      void servant_deactivated_event(PortableServer::Servant servant,
                                    const PortableServer::ObjectId& oid);

    private:

      /// Smart Pointer to a custom servant dispatching strategy object.
      /// This smart pointer will be in the "nil" state when the "default"
      /// strategy is to be applied.
      CSD_Framework::Strategy_var  strategy_;
      TAO::CSD::Strategy_Base *strategy_impl_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_Framework/CSD_Strategy_Proxy.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif
