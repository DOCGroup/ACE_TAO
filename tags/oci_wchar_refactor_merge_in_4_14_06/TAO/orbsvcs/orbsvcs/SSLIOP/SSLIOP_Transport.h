// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_Transport.h
 *
 *  $Id$
 *
 *  SSLIOP Transport specific processing.
 *
 *  @author Carlos O'Ryan <coryan@ece.uci.edu>
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_TRANSPORT_H
#define TAO_SSLIOP_TRANSPORT_H

#include /**/ "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Transport.h"
#include "tao/operation_details.h"
#include "tao/GIOP_Message_Version.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/IIOPC.h"

#include "ace/SSL/SSL_SOCK_Stream.h"

#include "ace/Svc_Handler.h"

// Forward decls.
class TAO_ORB_Core;
class TAO_Pluggable_Messaging;
class TAO_Acceptor;

namespace TAO
{
  namespace SSLIOP
  {
    typedef ACE_Svc_Handler<ACE_SSL_SOCK_STREAM, ACE_NULL_SYNCH> SVC_HANDLER;

    class Handler_Base;
    class Connection_Handler;

    /**
     * @class Transport
     *
     * @brief SSLIOP-specific transport implementation.
     *
     * SSLIOP-specific transport implementation.
     */
    class TAO_SSLIOP_Export Transport : public TAO_Transport
    {
    public:

      /// Constructor.
      Transport (Connection_Handler *handler,
                 TAO_ORB_Core *orb_core,
                 CORBA::Boolean flag);

      /// Default destructor.
      ~Transport (void);

      /// Overload of the handle_input () in the TAO_Transport
      /// class. This is required to set up the state guard. The
      /// thread-per-connection and wait on RW strategies call this
      /// handle_input ().
      virtual int handle_input (TAO_Resume_Handle &rh,
                                ACE_Time_Value *max_wait_time = 0,
                                int block = 0);

    protected:
      /** @name Overridden Template Methods
       *
       * These are implementations of template methods declared by
       * TAO_Transport.
       */
      //@{
      virtual ACE_Event_Handler * event_handler_i (void);
      virtual TAO_Connection_Handler *connection_handler_i (void);

      virtual TAO_Pluggable_Messaging *messaging_object (void);

      /// Write the complete Message_Block chain to the connection.
      virtual ssize_t send (iovec *iov, int iovcnt,
                            size_t &bytes_transferred,
                            const ACE_Time_Value *timeout = 0);

      /// Read len bytes from into buf.
      virtual ssize_t recv (char *buf,
                            size_t len,
                            const ACE_Time_Value *s = 0);

    public:
      /// @todo These methods IMHO should have more meaningful
      ///       names. The names seem to indicate nothing.
      virtual int send_request (TAO_Stub *stub,
                                TAO_ORB_Core *orb_core,
                                TAO_OutputCDR &stream,
                                int message_semantics,
                                ACE_Time_Value *max_wait_time);

      virtual int send_message (TAO_OutputCDR &stream,
                                TAO_Stub *stub = 0,
                                int message_semantics =
                                  TAO_Transport::TAO_TWOWAY_REQUEST,
                                ACE_Time_Value *max_time_wait = 0);

      virtual int generate_request_header (TAO_Operation_Details &opdetails,
                                           TAO_Target_Specification &spec,
                                           TAO_OutputCDR &msg);

      /// Initialising the messaging object
      virtual int messaging_init (CORBA::Octet major,
                                  CORBA::Octet minor);

      /// Open teh service context list and process it.
      virtual int tear_listen_point_list (TAO_InputCDR &cdr);
      //@}

    private:

      /// Set the Bidirectional context info in the service context
      /// list.
      void set_bidir_context_info (TAO_Operation_Details &opdetails);

      /// Add the listen points in @a acceptor to the @a
      /// listen_point_list if this connection is in the same
      /// interface as that of the endpoints in the @a acceptor.
      int get_listen_point (IIOP::ListenPointList &listen_point_list,
                            TAO_Acceptor *acceptor);

    private:

      /// The connection service handler used for accessing lower layer
      /// communication protocols.
      Connection_Handler *connection_handler_;

      /// Our messaging object.
      TAO_Pluggable_Messaging *messaging_object_;
    };

  }  // End SSLIOP namespace.
}  // End TAO namespace.

#include /**/ "ace/post.h"
#endif  /* TAO_SSLIOP_TRANSPORT_H */
