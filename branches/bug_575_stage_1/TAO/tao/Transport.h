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

#include "Exception.h"
#include "Transport_Descriptor_Interface.h"
#include "Transport_Cache_Manager.h"
#include "Transport_Timer.h"
#include "ace/Strategies.h"
#include "Incoming_Message_Queue.h"

class TAO_ORB_Core;
class TAO_Target_Specification;
class TAO_Operation_Details;
class TAO_Transport_Mux_Strategy;
class TAO_Wait_Strategy;
class TAO_Connection_Handler;
class TAO_Pluggable_Messaging;

class TAO_Queued_Message;
class TAO_Resume_Handle;

class TAO_Export TAO_Synch_Refcountable : private ACE_Refcountable
{
public:
  virtual ~TAO_Synch_Refcountable (void);

  int increment (void);
  int decrement (void);

  int refcount (void) const;

protected:
  TAO_Synch_Refcountable (ACE_Lock *lock, int refcount);

  ACE_Lock *refcount_lock_;
};

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
 * One of the responsibilities of the TAO_Transport class is to send
 * out GIOP messages as efficiently as possible.  In most cases
 * messages are put out in FIFO order, the transport object will put
 * out the message using a single system call and return control to
 * the application.  However, for oneways and AMI requests it may be
 * more efficient (or required if the SYNC_NONE policy is in effect)
 * to queue the messages until a large enough data set is available.
 * Another reason to queue is that some applications cannot block for
 * I/O, yet they want to send messages so large that a single write()
 * operation would not be able to cope with them.  In such cases we
 * need to queue the data and use the Reactor to drain the queue.
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
 * Consequently, the Transport must also know if the head of the queue
 * has been partially sent.  In that case new messages can only follow
 * the head. Only once the head is completely sent we can start
 * sending new messages.
 *
 * <H4>Waiting threads:</H4> One or more threads can be blocked
 * waiting for the connection to completely send the message.
 * The thread should return as soon as its message has been sent, so a
 * per-thread condition is required.  This suggest that simply using a
 * ACE_Message_Queue would not be enough:  there is a significant
 * amount of ancillary information, to keep on each message that the
 * Message_Block class does not provide room for.
 *
 * Blocking I/O is still attractive for some applications.  First, my
 * eliminating the Reactor overhead performance is improved when
 * sending large blocks of data.  Second, using the Reactor to send
 * out data opens the door for nested upcalls, yet some applications
 * cannot deal with the reentrancy issues in this case.
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
 * - A per-message timeout
 *
 * The Transport object provides a single method to send messages
 * (send_message ()).
 *
 * <H3>The incoming data path:</H3>
 *
 * One of the main responsibilities of the transport is to read and
 * process the incoming GIOP message as quickly and efficiently as
 * possible. There are other forces that needs to be given due
 * consideration. They are
 *  - Multiple threads should  be able to tarverse along the same data
 *    path but should not be able to read from the same handle at the
 *    same time ie. the handle should not be shared between threads at
 *    any instant.
 *  - Reads on the handle could give one or more messages.
 *  - Minimise locking and copying overhead when trying to attack the
 *    above.
 *
 * <H3> Parsing messages (GIOP) & processing the message:</H3>
 *
 * The messages should be checked for validity and the right
 * information should be sent to the higher layer for processing. The
 * process of doing a sanity check and preparing the messages for the
 * higher layers of the ORB are done by the messaging protocol.
 *
 * <H3> Design forces and Challenges </H3>
 *
 * To keep things as efficient as possible for medium sized requests,
 * it would be good to minimise data copying and locking along the
 * incoming path ie. from the time of reading the data from the handle
 * to the application. We achieve this by creating a buffer on stack
 * and reading the data from the handle into the buffer. We then pass
 * the same data block (the buffer is encapsulated into a data block)
 * to the higher layers of the ORB. The problems stem from the
 * following
 *  (a) Data is bigger than the buffer that we have on stack
 *  (b) Transports like TCP do not guarentee availability of the whole
 *      chunk of data in one shot. Data could trickle in byte by byte.
 *  (c) Single read gives multiple messages
 *
 *
 * <B>See Also:</B>
 *
 * http://ace.cs.wustl.edu/cvsweb/ace-latest.cgi/ACE_wrappers/TAO/docs/pluggable_protocols/index.html
 *
 */
class TAO_Export TAO_Transport : private TAO_Synch_Refcountable
{
public:
  /// default creator, requres the tag value be supplied.
  TAO_Transport (CORBA::ULong tag,
                 TAO_ORB_Core *orb_core);

  /// destructor
  virtual ~TAO_Transport (void);

  /// Return the protocol tag.
  /**
   * The OMG assigns unique tags (a 32-bit unsigned number) to each
   * protocol. New protocol tags can be obtained free of charge from
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

  /// Callback method to reactively drain the outgoing data queue
  int handle_output (void);

  /**
   * Return the TSS leader follower condition variable used in the
   * Wait Strategy. Muxed Leader Follower implementation returns a
   * valid condition variable, others return 0.
   */
  virtual TAO_SYNCH_CONDITION *leader_follower_condition_variable (void);

  /**
   * Initialising the messaging object. This would be used by the
   * connector side. On the acceptor side the connection handler
   * would take care of the messaging objects.
   */
  virtual int messaging_init (CORBA::Octet major,
                              CORBA::Octet minor) = 0;

  /// Get/Set the bidirectional flag
  virtual int bidirectional_flag (void) const;
  virtual void bidirectional_flag (int flag);

  /// Fill in a handle_set with any associated handler's reactor handle.
  /**
   * Called by the cache when the cache is closing in order to fill
   * in a handle_set in a lock-safe manner.
   *
   * @param reactor_registered the ACE_Handle_Set into which the
   *        transport should place any handle registered with the reactor
   *
   * @param unregistered the TAO_EventHandlerSet into which the
   *        transport should place any event handler that is not registered
   *        with anyone
   */
  void provide_handle (ACE_Handle_Set &reactor_registered,
                       TAO_EventHandlerSet &unregistered);

  /// Extracts the list of listen points from the <cdr> stream. The
  /// list would have the protocol specific details of the
  /// ListenPoints
  virtual int tear_listen_point_list (TAO_InputCDR &cdr);

  /// Remove all messages from the outgoing queue.
  /**
   * @todo: shouldn't this be automated?
   */
  void dequeue_all (void);

  /// Check if there are messages pending in the queue
  /**
   * @return 1 if the queue is empty
   */
  int queue_is_empty (void);

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
  // @@ lockme
  int register_handler (void);

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

  /// Call the implementation method after obtaining the lock.
  virtual void close_connection (void);

  //@}

  /// Write the complete Message_Block chain to the connection.
  /**
   * This method serializes on handler_lock_, guaranteeing that only
   * thread can execute it on the same instance concurrently.
   *
   * Often the implementation simply forwards the arguments to the
   * underlying ACE_Svc_Handler class. Using the code factored out
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
   * This call can also fail if the transport instance is no longer
   * associated with a connection (e.g., the connection handler closed
   * down).  In that case, it returns -1 and sets errno to
   * <code>ENOENT</code>.
   */
  ssize_t send (iovec *iov, int iovcnt,
                size_t &bytes_transferred,
                const ACE_Time_Value *timeout = 0);

  /// Read len bytes from into buf.
  /**
   * This method serializes on handler_lock_, guaranteeing that only
   * thread can execute it on the same instance concurrently.
   *
   * @param buffer ORB allocated buffer where the data should be
   * @@ The ACE_Time_Value *s is just a place holder for now.  It is
   * not clear this this is the best place to specify this.  The actual
   * timeout values will be kept in the Policies.
   */
  ssize_t recv (char *buffer,
                size_t len,
                const ACE_Time_Value *timeout = 0);


  /// Return the identifier for this transport instance.
  /**
   * If not set, this will return an integer representation of
   * the <code>this</code> pointer for the instance on which
   * it's called.
   */
  int id (void) const;
  /// Set the identifier for this transport instance.
  void id (int id);

  /// Return the order for the purging strategy.
  unsigned long purging_order (void) const;

  /// Allow the purging strategy to set the order.
  void purging_order(unsigned long value);

protected:
  /** @name Template methods
   *
   * The Transport class uses the Template Method Pattern to implement
   * the protocol specific functionality.
   * Implementors of a pluggable protocol should override the
   * following methods with the semantics documented below.
   */
  //@{

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
  // @@ this is broken once we add the lock b/c it returns the thing
  // we're trying to lock down! (CJC)
  virtual ACE_Event_Handler *event_handler_i (void) = 0;

  /// Return the messaging object that is used to format the data that
  /// needs to be sent.
  virtual TAO_Pluggable_Messaging * messaging_object (void) = 0;

  /// Write the complete iovec chain to the connection.
  /**
   * Often the implementation simply forwards the arguments to the
   * underlying ACE_Svc_Handler class. Using the code factored out
   * into ACE.
   *
   * Be careful with protocols that perform non-trivial
   * transformations of the data, such as SSLIOP or protocols that
   * compress the stream.
   *
   * @param iov contains the data that must be sent.
   *
   * @param iovcnt is the number of iovec structures in the list
   * where iov points.
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
   * @param timeout is the maximum time that the application is
   * willing to wait for the data to be sent, useful in platforms that
   * implement timed writes.
   * The timeout value is obtained from the policies set by the
   * application.
   *
   */
  virtual ssize_t send_i (iovec *iov, int iovcnt,
                          size_t &bytes_transferred,
                          const ACE_Time_Value *timeout = 0) = 0;

  // Read len bytes from into buf.
  /**
   * @param buffer ORB allocated buffer where the data should be
   * @@ The ACE_Time_Value *s is just a place holder for now.  It is
   * not clear this this is the best place to specify this.  The actual
   * timeout values will be kept in the Policies.
   */
  virtual ssize_t recv_i (char *buffer,
                          size_t len,
                          const ACE_Time_Value *timeout = 0) = 0;

  /// This class needs priviledged access to
  /// - close_connection_i()
  friend class TAO_Transport_Cache_Manager;

  /// Call the corresponding connection handler's <close>
  /// method.
  virtual void close_connection_i (void);

public:

  /// This is a request for the transport object to write a
  /// LocateRequest header before it is sent out.
  int generate_locate_request (TAO_Target_Specification &spec,
                               TAO_Operation_Details &opdetails,
                               TAO_OutputCDR &output);

  /// This is a request for the transport object to write a request
  /// header before it sends out the request
  virtual int generate_request_header (TAO_Operation_Details &opd,
                                       TAO_Target_Specification &spec,
                                       TAO_OutputCDR &msg);

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
  virtual int handle_input_i (TAO_Resume_Handle &rh,
                              ACE_Time_Value *max_wait_time = 0,
                              int block = 0);


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
  // @nolock b/c this calls send_or_buffer
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &stream,
                            int is_synchronous,
                            ACE_Time_Value *max_time_wait) = 0;



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
  // @@ lockme
  virtual int send_message (TAO_OutputCDR &stream,
                            TAO_Stub *stub = 0,
                            int is_synchronous = 1,
                            ACE_Time_Value *max_time_wait = 0) = 0;

protected:
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
  // @@ lockme
  virtual int register_handler_i (void) = 0;

  /// Called by <code>connection_handler_closing()</code> to signal
  /// that the protocol-specific transport should dissociate itself
  /// with the protocol-specific connection handler.
  /**
   * Typically, this just sets the pointer to the associated connection
   * handler to zero, although it could also clear out any additional
   * resources associated with the handler association.
   */
  virtual void transition_handler_state_i (void) = 0;


  /// Called by the handle_input_i  (). This method is used to parse
  /// message just read. It also decides whether the message
  // @@Bala: Documentation???
  int parse_consolidate_messages (ACE_Message_Block &bl,
                                  TAO_Resume_Handle &rh,
                                  ACE_Time_Value *time);

  int parse_incoming_messages (ACE_Message_Block &message_block);

  size_t missing_data (ACE_Message_Block &message_block);

  int check_message_integrity (ACE_Message_Block &message_block);

  int consolidate_message (ACE_Message_Block &incoming,
                           ssize_t missing_data,
                           TAO_Resume_Handle &rh,
                           ACE_Time_Value *max_wait_time);

  int consolidate_message_queue (ACE_Message_Block &incoming,
                                 ssize_t missing_data,
                                 TAO_Resume_Handle &rh,
                                 ACE_Time_Value *max_wait_time);

  int consolidate_extra_messages (ACE_Message_Block &incoming,
                                  TAO_Resume_Handle &rh);

  /// @@ Bala: Documentation
  virtual int process_parsed_messages (TAO_Queued_Data *qd);

public:
  /// Method for the connection handler to signify that it
  /// is being closed and destroyed.
  virtual void connection_handler_closing (void);

  // Maintain reference counting with these
  static TAO_Transport* _duplicate (TAO_Transport* transport);
  static void release (TAO_Transport* transport);

  /// recache ourselves in the cache
  int recache_transport (TAO_Transport_Descriptor_Interface* desc);

  /// Set/Get the Cache Map entry
  void cache_map_entry (TAO_Transport_Cache_Manager::HASH_MAP_ENTRY *entry);
  TAO_Transport_Cache_Manager::HASH_MAP_ENTRY *cache_map_entry (void);

  /// Send a message block chain,
  int send_message_block_chain (const ACE_Message_Block *message_block,
                                size_t &bytes_transferred,
                                ACE_Time_Value *max_wait_time = 0);

  /// Sent the contents of <message_block>
  /**
   * @todo This method name sucks, but send_message() was already
   *       taken by other silly methods!
   *
   * @param stub The object reference used for this operation, useful
   *             to obtain the current policies.
   * @param is_synchronous If set this method will block until the
   *             operation is completely written on the wire
   * @param message_block The CDR encapsulation of the GIOP message
   *             that must be sent.  The message may consist of
   *             multiple Message Blocks chained through the cont()
   *             field.
   * @param max_wait_time The maximum time that the operation can
   *             block, used in the implementation of timeouts.
   *
   */
  /// the twoway flag or by the current policies in the stub.
  int send_message_i (TAO_Stub *stub,
                      int is_synchronous,
                      const ACE_Message_Block *message_block,
                      ACE_Time_Value *max_wait_time);

  /// Send a message block chain, assuming the lock is held
  int send_message_block_chain_i (const ACE_Message_Block *message_block,
                                  size_t &bytes_transferred,
                                  ACE_Time_Value *max_wait_time);
  /// Cache management
  void mark_invalid (void);

  /// Cache management
  int make_idle (void);

  /// The timeout callback, invoked when any of the timers related to
  /// this transport expire.
  /**
   * @param current_time The current time as reported from the Reactor
   * @param act The Asynchronous Completion Token.  Currently it is
   *            interpreted as follows:
   * - If the ACT is the address of this->current_deadline_ the
   *   queueing timeout has expired and the queue should start
   *   flushing.
   *
   * @return Returns 0 if there are no problems, -1 if there is an
   *         error
   *
   * @todo In the future this function could be used to expire
   *       messages (oneways) that have been sitting for too long on
   *       the queue.
   */
  int handle_timeout (const ACE_Time_Value &current_time,
                      const void* act);

  // @@ Bala : Add documentation
  //  int process_message (ACE_Message_Block &message_block) = 0;

private:
  /// Send some of the data in the queue.
  /**
   * As the outgoing data is drained this method is invoked to send as
   * much of the current message as possible.
   *
   * Returns 0 if there is more data to send, -1 if there was an error
   * and 1 if the message was completely sent.
   */
  int drain_queue (void);

  /// Implement drain_queue() assuming the lock is held
  int drain_queue_i (void);

  /// This class needs priviledged access to
  /// - queue_is_empty_i()
  /// - drain_queue_i()
  friend class TAO_Block_Flushing_Strategy;

  /// Check if there are messages pending in the queue
  /**
   * This version assumes that the lock is already held.  Use with
   * care!
   *
   * @return 1 if the queue is empty
   */
  int queue_is_empty_i (void);

  /// A helper routine used in drain_queue_i()
  int drain_queue_helper (int &iovcnt, iovec iov[]);

  /// This class needs privileged access to:
  /// - schedule_output_i()
  /// - cancel_output_i()
  friend class TAO_Reactive_Flushing_Strategy;

  /// Schedule handle_output() callbacks
  int schedule_output_i (void);

  /// Cancel handle_output() callbacks
  int cancel_output_i (void);

  /// Cleanup the queue.
  /**
   * Exactly <byte_count> bytes have been sent, the queue must be
   * cleaned up as potentially several messages have been completely
   * sent out.
   * It leaves on head_ the next message to send out.
   */
  void cleanup_queue (size_t byte_count);

  /// Copy the contents of a message block into a Queued_Message
  /// TAO_Queued_Message *copy_message_block (const ACE_Message_Block *mb);

  /// Check if the buffering constraints have been reached
  int check_buffering_constraints_i (TAO_Stub *stub, int &must_flush);

  /// Send a synchronous message, i.e. block until the message is on
  /// the wire
  int send_synchronous_message_i (const ACE_Message_Block *message_block,
                                  ACE_Time_Value *max_wait_time);

  /// Check if the flush timer is still pending
  int flush_timer_pending (void) const;

  /// The flush timer expired or was explicitly cancelled, mark it as
  /// not pending
  void reset_flush_timer (void);

  /// Check if the underlying event handler is still valid.
  /**
   * @return Returns -1 if not, 0 if it is.
   */
  int check_event_handler_i (const char *caller);

  /// Print out error messages if the event handler is not valid
  void report_invalid_event_handler (const char *caller);

  /// Process the message that is in the head of the incoming queue.
  /// If there are more messages in the queue, this method sends a
  /// notify () to the reactor to send a next thread along.
  int process_queue_head (TAO_Resume_Handle &rh);

  /// Prohibited
  ACE_UNIMPLEMENTED_FUNC (TAO_Transport (const TAO_Transport&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Transport&))

protected:
  /// IOP protocol tag.
  CORBA::ULong tag_;

  /// Global orbcore resource.
  TAO_ORB_Core *orb_core_;

  /// Our entry in the cache. We dont own this. It is here for our
  /// convinience. We cannot just change things around.
  TAO_Transport_Cache_Manager::HASH_MAP_ENTRY *cache_map_entry_;

  /// Strategy to decide whether multiple requests can be sent over the
  /// same connection or the connection is exclusive for a request.
  TAO_Transport_Mux_Strategy *tms_;

  /// Strategy for waiting for the reply after sending the request.
  TAO_Wait_Strategy *ws_;

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
   *   *dont* want the server to pack service info to the client. That
   *   is not allowed. We need a flag to prevent such a things from
   *   happening.
   *
   * The value of this flag will be 0 if the client sends info and 1
   * if the server receives the info.
   */
  int bidirectional_flag_;

  /// Implement the outgoing data queue
  TAO_Queued_Message *head_;
  TAO_Queued_Message *tail_;

  /// @@Bala: Docu??
  TAO_Incoming_Message_Queue incoming_message_queue_;

  /// The queue will start draining no later than <queing_deadline_>
  /// *if* the deadline is
  ACE_Time_Value current_deadline_;

  /// The timer ID
  long flush_timer_id_;

  /// The adapter used to receive timeout callbacks from the Reactor
  TAO_Transport_Timer transport_timer_;

  /// Lock that insures that activities that *might* use handler-related
  /// resources (such as a connection handler) get serialized.
  /**
   * This is an <code>ACE_Lock</code> that gets initialized from
   * <code>TAO_ORB_Core::resource_factory()->create_cached_connection_lock ()</code>.
   * This way, one can use a lock appropriate for the type of system, i.e.,
   * a null lock for single-threaded systems, and a real lock for
   * multi-threaded systems.
   */
  ACE_Lock *handler_lock_;

  /// A unique identifier for the transport.
  /**
   * This never *never*
   * changes over the lifespan, so we don't have to worry
   * about locking it.
   *
   * HINT: Protocol-specific transports that use connection handler
   * might choose to set this to the handle for their connection.
   */
  int id_;

  /// Used by the LRU, LFU and FIFO Connection Purging Strategies.
  unsigned long purging_order_;
};

#if defined (__ACE_INLINE__)
# include "Transport.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_TRANSPORT_H */
