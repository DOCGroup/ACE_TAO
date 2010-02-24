// -*- C++ -*-

//=============================================================================
/**
 *  @file   HTIOP_Acceptor_Impl.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================


#ifndef HTIOP_ACCEPTOR_IMPL_H
#define HTIOP_ACCEPTOR_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
class TAO_ORB_Core;

namespace TAO
{
  namespace HTIOP
  {
    /**
     * @class Creation_Strategy
     *
     * @brief Creates a Svc_Handler and set the ORB_Core pointer on it.
     */
    template <class SVC_HANDLER>
    class Creation_Strategy : public ACE_Creation_Strategy<SVC_HANDLER>
    {
    public:
      /**
       * Constructor. <arg> parameter is used to pass any special
       * state/info to the service handler upon creation.

      */
      Creation_Strategy (TAO_ORB_Core *orb_core);

      /// Create a SVC_HANDLER  and set the ORB_Core pointer on it.
      int make_svc_handler (SVC_HANDLER *&sh);

    protected:
      /// Pointer to the ORB Core.
      TAO_ORB_Core *orb_core_;
    };

    template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
    class Accept_Strategy : public ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>
    {
    public:
      /// Constructor.
      Accept_Strategy (TAO_ORB_Core *orb_core);

      /// Initialize the <peer_acceptor_> with <local_addr>.  If the
      /// process runs out of handles, purge some "old" connections.
      int open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                bool restart = false);

      /// Delegates to the <accept> method of the PEER_ACCEPTOR. If the
      /// process runs out of handles, purge some "old" connections.
      int accept_svc_handler (SVC_HANDLER *svc_handler);

    protected:
      /// Base class.
      typedef ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> ACCEPT_STRATEGY_BASE;

      /// Pointer to the ORB Core.
      TAO_ORB_Core *orb_core_;
    };

    /**
     * @class Concurrency_Strategy
     *
     * @brief Activates the Svc_Handler.
     */
    template <class SVC_HANDLER>
    class Concurrency_Strategy : public ACE_Concurrency_Strategy<SVC_HANDLER>
    {
    public:
      /// Constructor.
      Concurrency_Strategy (TAO_ORB_Core *orb_core);

      /**
       * Activates the Svc_Handler.
       */
      int activate_svc_handler (SVC_HANDLER *svc_handler,
                                void *arg);

    protected:
      /// Base class.
      typedef ACE_Concurrency_Strategy<SVC_HANDLER> CONCURRENCY_STRATEGY_BASE;

      /// Pointer to the ORB Core.
      TAO_ORB_Core *orb_core_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/HTIOP/HTIOP_Acceptor_Impl.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("HTIOP_Acceptor_Impl.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* HTIOP_AccePTOR_IMPL_H */
