// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file Transport.h
 *
 *  $Id$
 *
 *  Define the interface for the Transport component in TAO's
 *  pluggable protocol framework.
 *
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_TRANSPORT_H
#define TAO_TRANSPORT_H
#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORB_Core;
class TAO_Target_Specification;
class TAO_Operation_Details;
class TAO_Transport_Mux_Strategy;
class TAO_Wait_Strategy;

#include "ace/Message_Queue.h"

typedef ACE_Message_Queue<ACE_NULL_SYNCH> TAO_Transport_Buffering_Queue;

/**
 * @class TAO_Transport
 *
 * @brief Generic definitions for the Transport class.
 *
 * The transport object is created in the Service handler
 * constructor and deleted in the Service Handler's destructor!!
 */
class TAO_Export TAO_Transport
{

  friend class TAO_Transport_Sync_Strategy;
  friend class TAO_Eager_Buffering_Sync_Strategy;
  friend class TAO_Delayed_Buffering_Sync_Strategy;

public:
  /// default creator, requres the tag value be supplied.
  TAO_Transport (CORBA::ULong tag,
                 TAO_ORB_Core *orb_core);

  /// destructor
  virtual ~TAO_Transport (void);

  /// The tag, each concrete class will have a specific tag value.
  CORBA::ULong tag (void) const;

  /// Call the corresponding connection handler's <close>
  /// method.
  virtual void close_connection (void) = 0;

  /// Idles the corresponding connection handler.
  virtual int idle (void) = 0;

  /// This method provides a way to gain access to the underlying file
  /// handle used by the reactor.
  virtual ACE_HANDLE handle (void) = 0;

  /// This method provides a way to gain access to the underlying event
  /// handler used by the reactor.
  virtual ACE_Event_Handler *event_handler (void) = 0;

  virtual ssize_t send (TAO_Stub *stub,
                        int two_way,
                        const ACE_Message_Block *mblk,
                        const ACE_Time_Value *s = 0) = 0;

  virtual ssize_t send (const ACE_Message_Block *mblk,
                        const ACE_Time_Value *s = 0,
                        size_t *bytes_transferred = 0) = 0;

  // Write the complete Message_Block chain to the connection.
  // @@ The ACE_Time_Value *s is just a place holder for now.  It is
  // not clear this this is the best place to specify this.  The actual
  // timeout values will be kept in the Policies.

  /// Write the contents of the buffer of length len to the connection.
  virtual ssize_t send (const u_char *buf,
                        size_t len,
                        const ACE_Time_Value *s = 0) = 0;

  /**
   * Read len bytes from into buf.
   * @@ The ACE_Time_Value *s is just a place holder for now.  It is
   * not clear this this is the best place to specify this.  The actual
   * timeout values will be kept in the Policies.
   */
  virtual ssize_t recv (char *buf,
                        size_t len,
                        const ACE_Time_Value *s = 0) = 0;

  /// Fill into <output> the right headers to make a request.
  virtual void start_request (TAO_ORB_Core *orb_core,
                              TAO_Target_Specification &spec,
                              TAO_OutputCDR &output,
                              CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  /// Fill into <output> the right headers to make a locate request.
  virtual void start_locate (TAO_ORB_Core *orb_core,
                             TAO_Target_Specification &spec,
                             TAO_Operation_Details &opdetails,
                             TAO_OutputCDR &output,
                             CORBA::Environment &ACE_TRY_ENV =
                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  /**
   * Depending on the concurrency strategy used by the transport it
   * may be required to setup state to receive a reply before the
   * request is sent.
   * Using this method, instead of send(), allows the transport (and
   * wait strategy) to take appropiate action.
   */
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &stream,
                            int twoway,
                            ACE_Time_Value *max_time_wait) = 0;

  /// This is a request for the transport object to write a request
  /// header before it sends out a request
  /**
   * @todo: Bala shouldn't this be <write_request_header> or maybe
   *   <init_request_header>, or <prepare_request_header>? Nothing is
   *   really sent at this point, right?
   */
  virtual CORBA::Boolean send_request_header (TAO_Operation_Details &opd,
                                              TAO_Target_Specification &spec,
                                              TAO_OutputCDR &msg) = 0;

  /// This method formats the stream and then sends the message on the
  /// transport.
  virtual int send_message (TAO_OutputCDR &stream,
                            TAO_Stub *stub = 0,
                            int twoway = 1,
                            ACE_Time_Value *max_time_wait = 0) = 0;

  /// Access the ORB that owns this connection.
  TAO_ORB_Core *orb_core (void) const;

  /// Get the TMS used by this Transport object.
  TAO_Transport_Mux_Strategy *tms (void) const;

  /// Return the Wait strategy used by the Transport.
  TAO_Wait_Strategy *wait_strategy (void) const;

  /**
   * Read and process the message on the connection. If <block> is 1,
   * then reply is read in a blocking manner. Once the message has
   * been successfully read, the message is processed by delegating
   * the responsibility to the underlying messaging object.
   */
  virtual int read_process_message (ACE_Time_Value *max_wait_time = 0,
                                    int block = 0) = 0;

  /**
   * Register the handler with the reactor. Will be called by the Wait
   * Strategy if Reactor is used  for that strategy.
   */
  virtual int register_handler (void) = 0;

  //@{ @name Control connection lifecycle

  /// These methods are routed through the TMS object. The TMS
  /// strategies implement them correctly.

  /// Request has been just sent, but the reply is not received. Idle
  /// the transport now.
  virtual int idle_after_send (void);

  /// Request is sent and the reply is received. Idle the transport
  /// now.
  virtual int idle_after_reply (void);

  //@}

  /**
   * Return the TSS leader follower condition variable used in the
   * Wait Strategy. Muxed Leader Follower implementation returns a
   * valid condition variable, others return 0.
   */
  virtual ACE_SYNCH_CONDITION *leader_follower_condition_variable (void);

  /// Queue for buffering transport messages.
  virtual TAO_Transport_Buffering_Queue &buffering_queue (void);

  /// Timer id associated with buffering.
  long buffering_timer_id (void) const;
  void buffering_timer_id (long);

  /// Timeout value associated with buffering.
  const ACE_Time_Value &buffering_timeout_value (void) const;
  void buffering_timeout_value (const ACE_Time_Value &time);

  /// Send any messages that have been buffered.
  ssize_t send_buffered_messages (const ACE_Time_Value *max_wait_time = 0);

  /**
   * Initialising the messaging object. This would be used by the
   * connector  side. On the acceptor side the connection handler
   * would take care of the messaging objects.
   */
  virtual int messaging_init (CORBA::Octet major,
                              CORBA::Octet minor) = 0;

  /// Get/Set the bidirectional flag
  virtual int bidirectional_flag (void) const;
  virtual void bidirectional_flag (int flag);

  /// @@ Bala: you must document this function!!
  /// @@ Bala: from the implementation in IIOP_Transport it looks more
  //     like it process the list of listening endpoints, or it is
  //     parsing it, but 'tearing'???
  virtual int tear_listen_point_list (TAO_InputCDR &cdr);

  /// Remove all messages from the outgoing queue.
  /**
   * @todo: shouldn't this be automated?
   */
  void dequeue_all (void);

protected:
  /// Remove the first message from the outgoing queue.
  void dequeue_head (void);

  /// Update the state of the outgoing queue, assuming that
  /// bytes_delivered bytes have been sent already.
  void reset_queued_message (ACE_Message_Block *message_block,
                             size_t bytes_delivered);

  /// Update the state of the outgoing queue, this time a complete
  /// message was sent.
  void reset_sent_message (ACE_Message_Block *message_block,
                           size_t bytes_delivered);

  /// Helper function used to implement the two methods above.
  void reset_message (ACE_Message_Block *message_block,
                      size_t bytes_delivered,
                      int queued_message);
private:
  /// Prohibited
  ACE_UNIMPLEMENTED_FUNC (TAO_Transport (const TAO_Transport&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Transport&))

protected:
  /// IOP protocol tag.
  CORBA::ULong tag_;

  /// Global orbcore resource.
  TAO_ORB_Core *orb_core_;

  /// Strategy to decide whether multiple requests can be sent over the
  /// same connection or the connection is exclusive for a request.
  TAO_Transport_Mux_Strategy *tms_;

  /// Strategy for waiting for the reply after sending the request.
  TAO_Wait_Strategy *ws_;

  /// Queue for buffering transport messages.
  TAO_Transport_Buffering_Queue *buffering_queue_;

  /// Buffering timer id.
  long buffering_timer_id_;

  /// Buffering timeout value.
  ACE_Time_Value buffering_timeout_value_;

  /// Use to check if bidirectional info has been synchronized with
  /// the peer.
  /**
   * Have we sent any info on bidirectional information or have we
   * received any info regarding making the connection served by this
   * transport bidirectional. 
   * The flag is used as follows:
   * + We dont want to send the bidirectional context info more than
   *   once on the connection. Why? Waste of marshalling and
   *   demarshalling time on the client.
   * + On the server side -- once a client that has established the
   *   connection asks the server to use the connection both ways, we
   *   *dont* want the server to go pack service info to the
   *   client. That would be *bad*..
   *   @@ Bala: what would this be *bad*? Exactly what breaks?  The
   *      protocol? Some GIOP rules?  Performance?
   *
   * The value of this flag will be 0 if the client sends info and 1
   * if the server receives the info.
   */
  int bidirectional_flag_;
};

#if defined (__ACE_INLINE__)
# include "Transport.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_TRANSPORT_H */
