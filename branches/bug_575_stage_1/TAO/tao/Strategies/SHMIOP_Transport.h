// This may look like C, but it's really -*- C++ -*-
// $Id$

// ===================================================================
/**
 *  @file   SHMIOP_Transport.h
 *
 *  $Id$
 *
 *  @author Originally by Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Modified by Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_SHMIOP_TRANSPORT_H
#define TAO_SHMIOP_TRANSPORT_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "strategies_export.h"
#include "ace/Svc_Handler.h"
#include "ace/MEM_Stream.h"
#include "tao/Transport.h"

class TAO_SHMIOP_Connection_Handler;
class TAO_Pluggable_Messaging;
class TAO_Target_Specification;
class Tao_Operation_Details;

typedef ACE_Svc_Handler<ACE_MEM_STREAM, ACE_NULL_SYNCH>
         TAO_SHMIOP_SVC_HANDLER;

/**
 * @class TAO_SHMIOP_Transport
 *
 * @brief Specialization of the base TAO_Transport class to handle the
 *  SHMIOP protocol.
 *
 */

class TAO_Strategies_Export TAO_SHMIOP_Transport : public TAO_Transport
{
public:

  /// Constructor.
  TAO_SHMIOP_Transport (TAO_SHMIOP_Connection_Handler *handler,
                        TAO_ORB_Core *orb_core,
                        CORBA::Boolean flag);

  /// Default destructor.
  ~TAO_SHMIOP_Transport (void);

protected:
  /** @name Overridden Template Methods
   *
   * These are implementations of template methods declared by TAO_Transport.
   */
  //@{

  virtual ACE_Event_Handler *event_handler_i (void);

  virtual TAO_Pluggable_Messaging *messaging_object (void);

  /// Write the complete Message_Block chain to the connection.
  virtual ssize_t send_i (iovec *iov, int iovcnt,
                          size_t &bytes_transferred,
                          const ACE_Time_Value *timeout = 0);

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

private:
  /// Process the message that we have read
  int process_message (void);

private:
  /// The connection service handler used for accessing lower layer
  /// communication protocols.
  TAO_SHMIOP_Connection_Handler *connection_handler_;

  /// Our messaging object.
  TAO_Pluggable_Messaging *messaging_object_;
};



#if defined (__ACE_INLINE__)
#include "SHMIOP_Transport.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#include "ace/post.h"
#endif /* TAO_SHMIOP_TRANSPORT_H */
