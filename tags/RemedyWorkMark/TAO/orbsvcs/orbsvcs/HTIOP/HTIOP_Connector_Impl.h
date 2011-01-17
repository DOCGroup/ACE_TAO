// -*- C++ -*-

// ===================================================================
/**
 *  @file   HTIOP_Connector_Impl.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
// ===================================================================

#ifndef HTIOP_CONNECTOR_IMPL_H
#define HTIOP_CONNECTOR_IMPL_H
#include /**/ "ace/pre.h"

#include "orbsvcs/HTIOP/HTIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "ace/Strategies_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Thread_Manager;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

namespace TAO
{
  namespace HTIOP
  {

    /**
     * @class HTIOP_Connect_Creation_Strategy
     *
     * @brief Creation strategy helper
     *
     * Creates the HTIOP_*_Connection_Handler object for the HTIOP_Connector
     * objects. This template class can now be used by all the Connector
     * objects instead of having to duplicate code. This class can be used
     * to set any required properties on the connection handlers at
     * creation time.
     *
     */

    template <class SVC_HANDLER>
    class Connect_Creation_Strategy : public ACE_Creation_Strategy<SVC_HANDLER>
    {
    public:

      /// Constructor. <arg> parameter is used to pass any special
      /// state/info to the service handler upon creation.  Currently used
      /// by HTIOP to pass protocol configuration properties.
      Connect_Creation_Strategy (ACE_Thread_Manager * = 0,
                                 TAO_ORB_Core* orb_core = 0,
                                 void *arg = 0,
                                 CORBA::Boolean flag = 0);

      /// Makes HTIOP_Client_Connection_Handlers
      virtual int make_svc_handler (SVC_HANDLER *&sh);


    private:

      /// Pointer to the ORB_Core on which we are activated
      TAO_ORB_Core* orb_core_;

      /// Some info/state to be passed to the service handler we create.
      void *arg_;
    };



    /**
     * @class HTIOP_Connect_Concurrency_Strategy
     *
     * @brief Concurrency strategy helper
     *
     * Activates the Svc_Handler, and then if the correct wait strategy is
     * in use registers the handler with the reactor.
     *
     */

    template <class SVC_HANDLER>
    class Connect_Concurrency_Strategy :
      public ACE_Concurrency_Strategy<SVC_HANDLER>
    {
    public:

      /// Constructor.
      Connect_Concurrency_Strategy (TAO_ORB_Core *orb_core);


      /// Activates the Svc_Handler, and if the right wait strategy is in
      /// use, registers the handle with the reactor.
      int activate_svc_handler (SVC_HANDLER *svc_handler,
                                void *arg);

    private:

      /// Pointer to the ORB Core.
      TAO_ORB_Core *orb_core_;

    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/HTIOP/HTIOP_Connector_Impl.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("HTIOP_Connector_Impl.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /*HTIOP_CONNECTOR_IMPL_H*/
