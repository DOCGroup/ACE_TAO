// This may look like C, but it's really -*- C++ -*-
// -*- C++ -*-
// ===================================================================
/**
 *  @file   DIOP_Transport.h
 *
 *  $Id$
 *
 *  @author Originally by Fred Kuhns <fredk@cs.wustl.edu>
 *  @author Modified by Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_DIOP_TRANSPORT_H
#define TAO_DIOP_TRANSPORT_H
#include "ace/pre.h"

#include "tao/Transport.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "ace/SOCK_Stream.h"
#include "ace/Synch.h"
#include "ace/Svc_Handler.h"
#include "diop_export.h"

// Forward decls.
class TAO_DIOP_Connection_Handler;
class TAO_ORB_Core;
class TAO_Operation_Details;
class TAO_Pluggable_Messaging;
class TAO_Acceptor;

// Service Handler for this transport
typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
        TAO_DIOP_SVC_HANDLER;

/**
 * @class TAO_DIOP_Transport
 *
 * @brief Specialization of the base TAO_Transport class to handle the
 *  DIOP protocol.
 *
 *
 *
 */

class TAO_DIOP_Export TAO_DIOP_Transport : public TAO_Transport
{
public:

  /// Constructor.
  TAO_DIOP_Transport (TAO_DIOP_Connection_Handler *handler,
                      TAO_ORB_Core *orb_core,
                      CORBA::Boolean flag);

  /// Default destructor.
  ~TAO_DIOP_Transport (void);

protected:
  /** @name Overridden Template Methods
   *
   * These are implementations of template methods declared by TAO_Transport.
   */
  //@{

  virtual ACE_Event_Handler *event_handler_i (void);

  virtual TAO_Pluggable_Messaging *messaging_object (void);

  /// Write the complete Message_Block chain to the connection.
  virtual ssize_t send_i (const ACE_Message_Block *mblk,
                          const ACE_Time_Value *s = 0,
                          size_t *bytes_transferred = 0);


  /// Read len bytes from into buf.
  virtual ssize_t recv_i (char *buf,
                          size_t len,
                          const ACE_Time_Value *s = 0);

  /// Read and process the message from the connection. The processing
  /// of the message is done by delegating the work to the underlying
  /// messaging object
  virtual int read_process_message (ACE_Time_Value *max_time_value = 0,
                                    int block =0);

  virtual int register_handler_i (void);

  /// Method to do whatever it needs to do when the connection
  /// handler is being closed and destroyed.
  virtual void transition_handler_state_i (void);

public:
  /// @@TODO: These methods IMHO should have more meaningful
  /// names. The names seem to indicate nothing.
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &stream,
                            int twoway,
                            ACE_Time_Value *max_wait_time);

  virtual int send_message (TAO_OutputCDR &stream,
                            TAO_Stub *stub = 0,
                            int twoway = 1,
                            ACE_Time_Value *max_time_wait = 0);

  /// Initialising the messaging object
  virtual int messaging_init (CORBA::Octet major,
                              CORBA::Octet minor);

  //@}

private:
  /// Process the message that we have read
  int process_message (void);

  // @@ Frank : Not needed
  /*
  /// Set the Bidirectional context info in the service context list
  void set_bidir_context_info (TAO_Operation_Details &opdetails);

  /// Add the listen points in <acceptor> to the <listen_point_list>
  /// if this connection is in the same interface as that of the
  /// endpoints in the <acceptor>
  int get_listen_point (DIOP::ListenPointList &listen_point_list,
                        TAO_Acceptor *acceptor);
  */
private:

  /// The connection service handler used for accessing lower layer
  /// communication protocols.
  TAO_DIOP_Connection_Handler *connection_handler_;

  /// Our messaging object.
  TAO_Pluggable_Messaging *messaging_object_;
};

#if defined (__ACE_INLINE__)
#include "DIOP_Transport.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_DIOP_TRANSPORT_H */
