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
#include "Exception.h"

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
 *
 * The main responsability of a Transport object is to encapsulate a
 * connection, and provide a transport independent way to send and
 * receive data.  Since TAO is heavily based on the Reactor for all if
 * not all its I/O the Transport class is usually implemented with a
 * helper Connection Handler that adapts the generic Transport
 * interface to the Reactor types.
 *
 * <H3>The outgoing data path:</H3>
 *
 * One of the responsabilities of the TAO_Transport class is to send
 * out GIOP messages as efficiently as possible.  In most cases
 * messages are put out in FIFO order, the transport object will put
 * out the message using a single system call and return control to
 * the application.  However, for oneways and AMI requests it may be
 * more efficient (or required if the SYNC_NONE policy is in effect)
 * to queue the messages until a large enough data set can be put out.
 * Also, large messages may not be sent out without blocking, and in
 * some applications blocking for I/O is unacceptable.
 *
 * Therefore, the Transport class may need to use a queue to
 * temporarily hold the messages, and, in some configurations, it may
 * need to use the Reactor to concurrently drain such queues.
 *
 * <H4>Out of order messages:</H4> TAO provides explicit policies to
 * send 'urgent' messages.  Such messages may put at the head of the
 * queue. However, they cannot be sent immediately because the
 * transport may already be sending another message in a reactive
 * fashion.
 *
 * Consequently, the Transport must also keep a
 * <TT>current_message</TT>, if the current message is not null any
 * new messages must be queued.  Only once the current message is
 * completely sent we can take a message out of the queue.
 *
 * <H4>Waiting threads:</H4> One or more threads can be blocked
 * waiting for the connection to completely send the message.
 * The thread should return as soon as its message has been sent, so a
 * per-thread condition is required.  This suggest that simply using a
 * ACE_Message_Queue would not be enough:  there is a significant
 * amount of ancillary information, to keep on each message that the
 * Message_Block class does not provide room for.
 *
 * Also some applications may choose, for performance reasons or to
 * avoid complex concurrency scenarios due to nested upcalls, to
 * using blocking I/O
 * block the 
 *
 * <H4>Timeouts:</H4> Some or all messages could have a timeout period
 * attached to them.  The timeout source could either be some
 * high-level policy or maybe some strategy to prevent denial of
 * service attacks.  In any case the timeouts are per-message, and
 * later messages could have shorter timeouts.
 * In fact, some kind of scheduling (such as EDF) could be required in
 * a few applications.
 *
 * <H4>Conclusions:</H4> The outgoing data path consist in several
 * components:
 *
 * - A queue of pending messages
 * - A message currently being transmitted
 * - A per-transport 'send strategy' to choose between blocking on
 *   write, blocking on the reactor or blockin on leader/follower.
 * - A per-message 'waiting object'
 * - A per-meessage timeout
 *
 * The Transport object provides a single method to send messages
 * (send_message ()).
 *   
 * <H3>The incoming data path:</H3>
 *
 * @todo Document the incoming data path design forces.
 *
 *
 * <B>See Also:</B>
 *
 * http://ace.cs.wustl.edu/cvsweb/ace-latest.cgi/ACE_wrappers/TAO/docs/pluggable_protocols/index.html
 * 
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

  /// Return the protocol tag.
  /**
   * The OMG assigns unique tags (a 32-bit unsigned number) to each
   * protocol.  New protocol tags can be obtained free of charge from
   * the OMG, check the documents in corbafwd.h for more details.
   */
  CORBA::ULong tag (void) const;

  /// Access the ORB that owns this connection.
  TAO_ORB_Core *orb_core (void) const;

  /// Get the TAO_Tranport_Mux_Strategy used by this object.
  /**
   * The role of the TAO_Transport_Mux_Strategy is described in more
   * detail in that class' documentation.  Enough is to say that the
   * class is used to control how many threads can have pending
   * requests over the same connection. Multiplexing multiple threads
   * over the same connection conserves resources and is almost
   * required for AMI, but having only one pending request per
   * connection is more efficient and reduces the possibilities of
   * priority inversions.
   */
  TAO_Transport_Mux_Strategy *tms (void) const;

  /// Return the TAO_Wait_Strategy used by this object.
  /**
   * The role of the TAO_Wait_Strategy is described in more detail in
   * that class' documentation. Enough is to say that the ORB can wait
   * for a reply blocking on read(), using the Reactor to wait for
   * multiple events concurrently or using the Leader/Followers
   * protocol.
   */
  TAO_Wait_Strategy *wait_strategy (void) const;

  /// Send a request or queue it for later.
  /**
   * If the right policies are set queue the request for later.
   * Otherwise, or if the queue size has reached the configured
   * limits, start draining the queue.
   *
   * If any data is to be sent it blocks until the queue is completely
   * drained.
   *
   * @todo: this routine will probably go away as part of the
   * reorganization to support non-blocking writes.
   */
  ssize_t send_or_buffer (TAO_Stub *stub,
                          int two_way,
                          const ACE_Message_Block *mblk,
                          const ACE_Time_Value *s = 0);

  /**
   * Return the TSS leader follower condition variable used in the
   * Wait Strategy. Muxed Leader Follower implementation returns a
   * valid condition variable, others return 0.
   */
  virtual TAO_SYNCH_CONDITION *leader_follower_condition_variable (void);

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

  /**
   * @name Control connection lifecycle
   *
   * These methods are routed through the TMS object. The TMS
   * strategies implement them correctly.
   */
  //@{

  /// Request has been just sent, but the reply is not received. Idle
  /// the transport now.
  virtual int idle_after_send (void);

  /// Request is sent and the reply is received. Idle the transport
  /// now.
  virtual int idle_after_reply (void);

  //@}

  /** @name Template methods
   *
   * The Transport class uses the Template Method Pattern to implement
   * the protocol specific functionality.
   * Implementors of a pluggable protocol should override the 
   * following methods with the semantics documented below.
   */
  //@{

  /// Call the corresponding connection handler's <close>
  /// method.
  virtual void close_connection (void) = 0;

  /// The connection is now idle, i.e. not in used by any ORB thread.
  /// The protocol can take advantage of this to recycle the
  /// connection immediately or as an indication that it can be safely
  /// removed from the connection cache.
  ///
  /// @todo Bala: this looks like a generic method to me.  IMHO TAO's
  /// connection cache should only know about Transport objects, and
  /// this method can be implemented in the base class!
  virtual int idle (void) = 0;

  /// Return the file descriptor used for this connection.
  /**
   * @todo Someday we should be able to support protocols that do not
   * have or use file descriptors. But this will require implementing
   * non-reactive concurrency models for some connections.  Really
   * hard to do.  Meanwhile, look at the SHMIOP protocol for an
   * example on how to use file descriptors for signalling while the
   * actual data is transfer via shared memory (where there are no
   * file descriptors.)
   */
  virtual ACE_HANDLE handle (void) = 0;

  /// Return the event handler used to receive notifications from the
  /// Reactor.
  /**
   * Normally a concrete TAO_Transport object has-a ACE_Event_Handler
   * member that function as an adapter between the ACE_Reactor
   * framework and the TAO pluggable protocol framework.
   * In all the protocols implemented so far this role is fullfilled
   * by an instance of ACE_Svc_Handler.
   * 
   * @todo Since we only use a limited functionality of
   * ACE_Svc_Handler we could probably implement a generic
   * adapter class (TAO_Transport_Event_Handler or something), this
   * will reduce footprint and simplify the process of implementing a
   * pluggable protocol.
   */
  virtual ACE_Event_Handler *event_handler (void) = 0;

  /// Write the complete Message_Block chain to the connection.
  /**
   * Often the implementation simply forwards the arguments to the
   * underlying ACE_Svc_Handler class.  Using the code factored out
   * into ACE.
   *
   * Be careful with protocols that perform non-trivial
   * transformations of the data, such as SSLIOP or protocols that
   * compress the stream.
   *
   * @param mblk contains the data that must be sent.  For each
   * message block in the cont() chain all the data between rd_ptr()
   * and wr_ptr() should be delivered to the remote peer.
   *
   * @param timeout is the maximum time that the application is
   * willing to wait for the data to be sent, useful in platforms that
   * implement timed writes.
   * The timeout value is obtained from the policies set by the
   * application.
   *
   * @param bytes_transferred should return the total number of bytes
   * successfully transferred before the connection blocked.  This is
   * required because in some platforms and/or protocols multiple
   * system calls may be required to send the chain of message
   * blocks.  The first few calls can work successfully, but the final
   * one can fail or signal a flow control situation (via EAGAIN).
   * In this case the ORB expects the function to return -1, errno to
   * be appropriately set and this argument to return the number of
   * bytes already on the OS I/O subsystem.
   *
   */
  virtual ssize_t send (const ACE_Message_Block *mblk,
                        const ACE_Time_Value *timeout = 0,
                        size_t *bytes_transferred = 0) = 0;

  // Read len bytes from into buf.
  /**
   * @param buffer ORB allocated buffer where the data should be 
   * @@ The ACE_Time_Value *s is just a place holder for now.  It is
   * not clear this this is the best place to specify this.  The actual
   * timeout values will be kept in the Policies.
   */
  virtual ssize_t recv (char *buffer,
                        size_t len,
                        const ACE_Time_Value *timeout = 0) = 0;

  /// Fill into <output> the right headers to make a request.
  /**
   * @todo Bala: in the good old days it was decided that the
   * pluggable protocol framework would not raise exceptions.
   */
  virtual void start_request (TAO_ORB_Core *orb_core,
                              TAO_Target_Specification &spec,
                              TAO_OutputCDR &output,
                              CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  /// Fill into <output> the right headers to make a locate request.
  /**
   * @todo Bala: in the good old days it was decided that the
   * pluggable protocol framework would not raise exceptions.
   */
  virtual void start_locate (TAO_ORB_Core *orb_core,
                             TAO_Target_Specification &spec,
                             TAO_Operation_Details &opdetails,
                             TAO_OutputCDR &output,
                             CORBA::Environment &ACE_TRY_ENV =
                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  /// Prepare the waiting and demuxing strategy to receive a reply for
  /// a new request.
  /**
   * Preparing the ORB to receive the reply only once the request is
   * completely sent opens the system to some subtle race conditions:
   * suppose the ORB is running in a multi-threaded configuration,
   * thread A makes a request while thread B is using the Reactor to
   * process all incoming requests.
   * Thread A could be implemented as follows:
   * 1) send the request
   * 2) setup the ORB to receive the reply
   * 3) wait for the request
   *
   * but in this case thread B may receive the reply between step (1)
   * and (2), and drop it as an invalid or unexpected message.
   * Consequently the correct implementation is:
   * 1) setup the ORB to receive the reply
   * 2) send the request
   * 3) wait for the reply
   *
   * The following method encapsulates this idiom.
   *
   * @todo This is generic code, it should be factored out into the
   * Transport class.
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
   *
   * @todo: This looks like generic code too.
   */
  virtual CORBA::Boolean send_request_header (TAO_Operation_Details &opd,
                                              TAO_Target_Specification &spec,
                                              TAO_OutputCDR &msg) = 0;

  /// This method formats the stream and then sends the message on the
  /// transport.
  /**
   * Once the ORB is prepared to receive a reply (see send_request()
   * above), and all the arguments have been marshaled the CDR stream
   * must be 'formatted', i.e. the message_size field in the GIOP
   * header can finally be set to the proper value.
   *
   * @todo Another generic method, move to TAO_Transport.
   */
  virtual int send_message (TAO_OutputCDR &stream,
                            TAO_Stub *stub = 0,
                            int twoway = 1,
                            ACE_Time_Value *max_time_wait = 0) = 0;

  /// Callback to read incoming data
  /**
   * The ACE_Event_Handler adapter invokes this method as part of its
   * handle_input() operation.
   *
   * @todo: the method name is confusing! Calling it handle_input()
   * would probably make things easier to understand and follow!
   *
   * Once a complete message is read the Transport class delegates on
   * the Messaging layer to invoke the right upcall (on the server) or
   * the TAO_Reply_Dispatcher (on the client side).
   *
   * @param max_wait_time In some cases the I/O is synchronous, e.g. a
   * thread-per-connection server or when Wait_On_Read is enabled.  In
   * those cases a maximum read time can be specified.
   *
   * @param block Is deprecated and ignored.
   *
   */
  virtual int read_process_message (ACE_Time_Value *max_wait_time = 0,
                                    int block = 0) = 0;

  /// Register the handler with the reactor. 
  /**
   * This method is used by the Wait_On_Reactor strategy. The
   * transport must register its event handler with the ORB's Reactor.
   *
   * @todo: I think this method is pretty much useless, the
   * connections are *always* registered with the Reactor, except in
   * thread-per-connection mode.  In that case putting the connection
   * in the Reactor would produce unpredictable results anyway.
   */
  virtual int register_handler (void) = 0;

  //@}

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
