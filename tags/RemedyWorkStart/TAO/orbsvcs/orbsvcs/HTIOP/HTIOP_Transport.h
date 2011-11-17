// -*- C++ -*-

// ===================================================================
/**
 *  @file   HTIOP_Transport.h
 *
 *  $Id$
 *
 *  @author Originally by Fred Kuhns <fredk@cs.wustl.edu>
 *  @author Modified by Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_HTIOP_TRANSPORT_H
#define TAO_HTIOP_TRANSPORT_H
#include /**/ "ace/pre.h"

#include "orbsvcs/HTIOP/HTIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/HTIOPC.h"

#include "ace/HTBP/HTBP_Stream.h"

#include "tao/Transport.h"

#include "ace/Synch.h"
#include "ace/Svc_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decls.
class TAO_ORB_Core;
class TAO_Operation_Details;
class TAO_Acceptor;
class TAO_Adapter;

namespace TAO
{
  namespace HTIOP
  {
    class Connection_Handler;

    // Service Handler for this transport
    typedef ACE_Svc_Handler<ACE::HTBP::Stream, ACE_NULL_SYNCH>
    SVC_HANDLER;

    /**
     * @class Transport
     *
     * @brief Specialization of the base Transport class to handle the
     *  HTIOP protocol.
     *
     *
     *
     */

    class HTIOP_Export Transport : public TAO_Transport
    {
    public:

      /// Constructor.
      Transport (Connection_Handler *handler,
                 TAO_ORB_Core *orb_core);

      /// Default destructor.
      ~Transport (void);

    protected:

      /** @name Overridden Template Methods
       *
       * Please check the documentation in "tao/Transport.h" for more
       * details.
       */
      //@{

      virtual ACE_Event_Handler * event_handler_i (void);
      virtual TAO_Connection_Handler * invalidate_event_handler_i (void);

      virtual ssize_t send (iovec *iov, int iovcnt,
                            size_t &bytes_transferred,
                            const ACE_Time_Value *timeout = 0);

      virtual ssize_t recv (char *buf,
                            size_t len,
                            const ACE_Time_Value *s = 0);

      virtual int register_handler (void);


    public:
      /// @@TODO: These methods IMHO should have more meaningful
      /// names. The names seem to indicate nothing.
      virtual int send_request (TAO_Stub *stub,
                                TAO_ORB_Core *orb_core,
                                TAO_OutputCDR &stream,
                                TAO_Message_Semantics message_semantics,
                                ACE_Time_Value *max_wait_time);

      virtual int send_message (TAO_OutputCDR &stream,
                                TAO_Stub *stub = 0,
                                TAO_Message_Semantics message_semantics =
                                  TAO_Message_Semantics (),
                                ACE_Time_Value *max_time_wait = 0);

      virtual int tear_listen_point_list (TAO_InputCDR &cdr);

      virtual TAO_Connection_Handler * connection_handler_i (void);

      //@}

    private:

      /// Set the Bidirectional context info in the service context list
      void set_bidir_context_info (TAO_Operation_Details &opdetails);

      /// Add the listen points in <acceptor> to the <listen_point_list>
      /// if this connection is in the same interface as that of the
      /// endpoints in the <acceptor>
      int get_listen_point (::HTIOP::ListenPointList &listen_point_list,
                            TAO_Acceptor *acceptor);

      /// The connection service handler used for accessing lower layer
      /// communication protocols.
      Connection_Handler *connection_handler_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif  /* TRANSPORT_H */
