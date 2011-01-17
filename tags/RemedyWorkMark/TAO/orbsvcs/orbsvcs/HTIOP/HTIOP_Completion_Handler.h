// -*- C++ -*-

// ===================================================================
/**
 *  @file   HTIOP_Completion_Handler.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
// ===================================================================

#ifndef HTIOP_COMPLETION_HANDLER_H
#define HTIOP_COMPLETION_HANDLER_H
#include /**/ "ace/pre.h"

#include "ace/Acceptor.h"
#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"
#include "tao/Acceptor_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/HTIOP/HTIOP_Export.h"
#include "orbsvcs/HTIOP/HTIOP_Connection_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;
// ****************************************************************

namespace TAO
{
  namespace HTIOP
  {

    /**
     * @class Completion_Handler
     *
     * @brief Helps the HTIOP Acceptor complete a connection.
     *
     * The Completion Handler owns a newly accepted connection until the
     * first data read. At that point, the session and data flow direction
     * are known so the stream may be handed off to either an existing
     * session, or to a newly created one.
     */

    typedef TAO_Creation_Strategy<Connection_Handler> CREATION_STRATEGY2;
    typedef TAO_Concurrency_Strategy<Connection_Handler> CONCURRENCY_STRATEGY2;

    typedef ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_NULL_SYNCH> COMPLETION_BASE;

    class HTIOP_Export Completion_Handler : public COMPLETION_BASE
    {

    public:
      Completion_Handler (ACE_Thread_Manager* t = 0);
      /// Constructor. <arg> parameter is used by the Acceptor to pass the
      /// protocol configuration properties for this connection.
      Completion_Handler (TAO_ORB_Core *orb_core,
                          CORBA::Boolean = 0 );

      /// Destructor.
      ~Completion_Handler (void);

      /// Called by the <Strategy_Acceptor> when the handler is completely
      /// connected.  Argument is unused.
      virtual int open (void *);

      //@{
      /** @name Event Handler overloads
       */
      virtual int handle_input (ACE_HANDLE);

      virtual int resume_handler (void);
      virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
      //@}

    private:
      TAO_ORB_Core *orb_core_;
      ACE::HTBP::Channel *channel_;

      CREATION_STRATEGY2 *creation_strategy_;
      CONCURRENCY_STRATEGY2 *concurrency_strategy_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* HTIOP_COMPLETION_HANDLER_H */
