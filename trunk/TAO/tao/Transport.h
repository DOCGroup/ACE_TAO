// -*- C++ -*-

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

#include /**/ "ace/pre.h"

#include "tao/Transport_Cache_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Transport_Timer.h"
#include "tao/Incoming_Message_Queue.h"
#include "tao/Incoming_Message_Stack.h"
#include "tao/Message_Semantics.h"
#include "ace/Time_Value.h"
#include "ace/Basic_Stats.h"

struct iovec;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;
class TAO_Target_Specification;
class TAO_Operation_Details;
class TAO_Transport_Mux_Strategy;
class TAO_Wait_Strategy;
class TAO_Connection_Handler;
class TAO_GIOP_Message_Base;
class TAO_Codeset_Translator_Base;

class TAO_Queued_Message;
class TAO_Synch_Queued_Message;
class TAO_Resume_Handle;
class TAO_Stub;
class TAO_MMAP_Allocator;
class TAO_ServerRequest;

namespace TAO
{
  /**
   * @note Should this be in TAO namespace. Seems like a candidate
   * that should be in the transport
   */
  enum Connection_Role
    {
      TAO_UNSPECIFIED_ROLE = 0,
      TAO_SERVER_ROLE = 1,
      TAO_CLIENT_ROLE = 2
    };

  namespace Transport
  {
    /// Transport-level statistics. Initially introduced to support
    /// the "Transport Current" functionality.
    class Stats;

  /**
   * @struct Drain_Constraints
   *
   * @brief Encapsulate the flushing control parameters.
   *
   * At several points, the ORB needs to flush data from a transport to the
   * underlying I/O mechanisms.  How this data is flushed depends on the
   * context where the request is made, the ORB configuration and the
   * application level policies in effect.
   *
   * Some examples:
   *
   * # When idle, the ORB will want to send data on any socket that has
   *   space available.  In this case, the queue must be drained on
   *   a best-effort basis, without any blocking.
   * # If the ORB is configured to handle nested upcalls, any two-way
   *   request should block and push data to the underlying socket as fast
   *   as possible.
   * # In the same use-case, but now with a timeout policy in
   *   effect, the ORB will need to send the data use I/O operations with
   *   timeouts (as implemented by ACE::sendv()
   * # When the ORB is configured to support nested upcalls, any two-way,
   *   reliable oneway or similar should wait using the reactor or
   *   Leader-Follower implementation.  While still respecting the timeout
   *   policies.
   *
   * Instead of sprinkling if() statements throughput the critical path
   * trying to determine how the I/O operations should be performed, we
   * pass the information encapsulated in this class.  The caller into the
   * Transport object determines the right parameters to use, and the
   * Transport object simply obeys those instructions.
   */
  class Drain_Constraints : private ACE_Copy_Disabled
  {
  public:
    /// Default constructor
    Drain_Constraints()
      : timeout_(0)
      , block_on_io_(false)
    {
    }

    /// Constructor
    Drain_Constraints(
        ACE_Time_Value * timeout,
        bool block_on_io)
      : timeout_(timeout)
      , block_on_io_(block_on_io)
    {
    }

    /**
     * If true, then the ORB should block on I/O operations instead of
     * using non-blocking I/O.
     */
    bool block_on_io() const
    {
      return block_on_io_;
    }

    /**
     * The maximum time to block on I/O operations (or nested loops) based
     * on the current timeout policies.
     */
    ACE_Time_Value * timeout() const
    {
      return timeout_;
    }

  private:
    ACE_Time_Value * timeout_;
    bool block_on_io_;
  };
  }
}

/*
 * Specialization hook for the TAO's transport implementation.
 */
//@@ TAO_TRANSPORT_SPL_INCLUDE_FORWARD_DECL_ADD_HOOK

/**
 * @class TAO_Transport
 *
 * @brief Generic definitions for the Transport class.
 *
 * The transport object is created in the Service handler
 * constructor and deleted in the Service Handler's destructor!!
 *
 * The main responsibility of a Transport object is to encapsulate a
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
 *   write, blocking on the reactor or blocking on leader/follower.
 * - A per-message 'waiting object'
 * - A per-message timeout
 *
 * The Transport object provides a single method to send request
 * messages (send_request_message ()).
 *
 * <H3>The incoming data path:</H3>
 *
 * One of the main responsibilities of the transport is to read and
 * process the incoming GIOP message as quickly and efficiently as
 * possible. There are other forces that needs to be given due
 * consideration. They are
 *  - Multiple threads should be able to traverse along the same data
 *    path but should not be able to read from the same handle at the
 *    same time ie. the handle should not be shared between threads at
 *    any instant.
 *  - Reads on the handle could give one or more messages.
 *  - Minimize locking and copying overhead when trying to attack the
 *    above.
 *
 * <H3>Parsing messages (GIOP) & processing the message:</H3>
 *
 * The messages should be checked for validity and the right
 * information should be sent to the higher layer for processing. The
 * process of doing a sanity check and preparing the messages for the
 * higher layers of the ORB are done by the messaging protocol.
 *
 * <H3>Design forces and Challenges </H3>
 *
 * To keep things as efficient as possible for medium sized requests,
 * it would be good to minimize data copying and locking along the
 * incoming path ie. from the time of reading the data from the handle
 * to the application. We achieve this by creating a buffer on stack
 * and reading the data from the handle into the buffer. We then pass
 * the same data block (the buffer is encapsulated into a data block)
 * to the higher layers of the ORB. The problems stem from the
 * following
 *  (a) Data is bigger than the buffer that we have on stack
 *  (b) Transports like TCP do not guarantee availability of the whole
 *      chunk of data in one shot. Data could trickle in byte by byte.
 *  (c) Single read gives multiple messages
 *
 * We solve the problems as follows
 *
 *   (a) First do a read with the buffer on stack. Query the underlying
 *       messaging object whether the message has any incomplete
 *       portion. If so, data will be copied into new buffer being able
 *       to hold full message and is queued; succeeding events will read
 *       data from socket and write directly into this buffer.
 *       Otherwise, if if the message in local buffer is complete, we free
 *       the handle and then send the message to the higher layers of the
 *       ORB for processing.
 *
 *   (b) If buffer with incomplete message has been enqueued, while trying
 *       to do the above, the reactor will call us back when the handle
 *       becomes read ready. The read-operation will copy data directly
 *       into the enqueued buffer.  If the message has bee read completely
 *       the message is sent to the higher layers of the ORB for processing.
 *
 *   (c) If we get multiple messages (possible if the client connected
 *       to the server sends oneways or AMI requests), we parse and
 *       split the messages. Every message is put in the queue. Once
 *       the messages are queued, the thread picks up one message to
 *       send to the higher layers of the ORB. Before doing that, if
 *       it finds more messages, it sends a notify to the reactor
 *       without resuming the handle. The next thread picks up a
 *       message from the queue and processes that. Once the queue
 *       is drained the last thread resumes the handle.
 *
 * <H3>Sending Replies </H3>
 *
 * We could use the outgoing path of the ORB to send replies. This
 * would allow us to reuse most of the code in the outgoing data
 * path. We were doing this till TAO-1.2.3. We run in to
 * problems. When writing the reply the ORB gets flow controlled, and the
 * ORB tries to flush the message by going into the reactor. This
 * resulted in unnecessary nesting. The thread that gets into the
 * Reactor could potentially handle other messages (incoming or
 * outgoing) and the stack starts growing leading to crashes.
 *
 * <H4>Solution to the nesting problem </H4>
 *
 * The solution that we (plan to) adopt is pretty straight
 * forward. The thread sending replies will not block to send the
 * replies but queue the replies and return to the Reactor. (Note the
 * careful usages of the terms "blocking in the Reactor" as opposed to
 * "return back to the Reactor".
 *
 *
 * <B>See Also:</B>
 *
 * https://svn.dre.vanderbilt.edu/viewvc/Middleware/trunk/TAO/docs/pluggable_protocols/index.html?revision=HEAD
 *
 */
class TAO_Export TAO_Transport
{
public:

  /// Default creator, requires the tag value be supplied.
  TAO_Transport (CORBA::ULong tag,
                 TAO_ORB_Core *orb_core,
                 size_t input_cdr_size = ACE_CDR::DEFAULT_BUFSIZE);

  /// Destructor
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

  enum Drain_Result_Enum
    {
      DR_ERROR = -1,
      DR_OK = 0,
      DR_QUEUE_EMPTY = 1, // used internally, not returned from drain_queue()
      DR_WOULDBLOCK = 2
    };

  /// The handle_output and drain_queue* functions return objects of this
  /// struct instead of the enum value directly so the compiler will catch
  /// any uses that assign the return value to an int.
  struct Drain_Result
  {
    Drain_Result (Drain_Result_Enum dre) : dre_(dre) {}
    Drain_Result_Enum dre_;

    bool operator== (Drain_Result rhs) const
    {
      return this->dre_ == rhs.dre_;
    }

    bool operator!= (Drain_Result rhs) const
    {
      return this->dre_ != rhs.dre_;
    }
  };

  /// Callback method to reactively drain the outgoing data queue
  Drain_Result handle_output (TAO::Transport::Drain_Constraints const & c);

  /// Get the bidirectional flag
  int bidirectional_flag (void) const;

  /// Set the bidirectional flag
  void bidirectional_flag (int flag);

  /// Set the Cache Map entry
  void cache_map_entry (TAO::Transport_Cache_Manager::HASH_MAP_ENTRY *entry);

  /// Get the Cache Map entry
  TAO::Transport_Cache_Manager::HASH_MAP_ENTRY *cache_map_entry (void);

  /// Set and Get the identifier for this transport instance.
  /**
   * If not set, this will return an integer representation of
   * the <code>this</code> pointer for the instance on which
   * it's called.
   */
  size_t id (void) const;
  void id (size_t id);

  /**
   * Methods dealing with the role of the connection, e.g., CLIENT or SERVER.
   * See CORBA 2.6 Specification, Section 15.5.1 for origin of definitions.
   */
  TAO::Connection_Role opened_as (void) const;
  void opened_as (TAO::Connection_Role);

  /// Get and Set the purging order. The purging strategy uses the set
  /// version to set the purging order.
  unsigned long purging_order (void) const;
  void purging_order(unsigned long value);

  /// Check if there are messages pending in the queue
  /**
   * @return true if the queue is empty
   */
  bool queue_is_empty (void);

  /// Register with the reactor via the wait strategy
  bool register_if_necessary (void);


  /// Added event handler to the handlers set.
  /**
   * Called by the cache when the cache is closing.
   *
   * @param handlers The TAO_Connection_Handler_Set into which the
   *        transport should place its handler
   */
  void provide_handler (TAO::Connection_Handler_Set &handlers);

  /// Add event handlers corresponding to transports that have RW wait
  /// strategy to the handlers set.
  /**
   * Called by the cache when the ORB is shutting down.
   *
   * @param handlers The TAO_Connection_Handler_Set into which the
   *        transport should place its handler if the transport has RW
   *        strategy on.
   *
   * @return true indicates a handler was added to the handler set.
   *         false indocates that the transport did not have a
   *         blockable handler that could be added.
   */
  bool provide_blockable_handler (TAO::Connection_Handler_Set &handlers);

  /// Register the handler with the reactor.
  /**
   * Register the handler with the reactor. This method is used by the
   * Wait_On_Reactor strategy. The transport must register its event
   * handler with the ORB's Reactor.
   *
   * @todo I think this method is pretty much useless, the
   * connections are *always* registered with the Reactor, except in
   * thread-per-connection mode.  In that case putting the connection
   * in the Reactor would produce unpredictable results anyway.
   */
  virtual int register_handler (void);

  /// Remove the handler from the reactor.
  virtual int remove_handler (void);

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
   * @param iov contains the data that must be sent.
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
  virtual ssize_t send (iovec *iov,
                        int iovcnt,
                        size_t &bytes_transferred,
                        ACE_Time_Value const * timeout) = 0;

#if TAO_HAS_SENDFILE == 1
  /// Send data through zero-copy write mechanism, if available.
  /**
   * This method sends the data in the I/O vector through the platform
   * sendfile() function to perform a zero-copy write, if available.
   * Otherwise, the default fallback implementation simply delegates
   * to the TAO_Transport::send() method.
   *
   * @note This method is best used when sending very large blocks of
   *       data.
   */
  virtual ssize_t sendfile (TAO_MMAP_Allocator * allocator,
                            iovec * iov,
                            int iovcnt,
                            size_t &bytes_transferred,
                            TAO::Transport::Drain_Constraints const & dc);
#endif  /* TAO_HAS_SENDFILE==1 */


  /// Read len bytes from into buf.
  /**
   * This method serializes on handler_lock_, guaranteeing that only
   * thread can execute it on the same instance concurrently.
   *
   * @param buffer ORB allocated buffer where the data should be
   * @param timeout The ACE_Time_Value *s is just a place holder for now. It is
   * not clear this this is the best place to specify this. The actual
   * timeout values will be kept in the Policies.
   */
  virtual ssize_t recv (char *buffer,
                        size_t len,
                        const ACE_Time_Value *timeout = 0) = 0;

  /**
   * @name Control connection lifecycle
   *
   * These methods are routed through the TMS object. The TMS
   * strategies implement them correctly.
   */
  //@{

  /// Request has been just sent, but the reply is not received. Idle
  /// the transport now.
  bool idle_after_send (void);

  /// Request is sent and the reply is received. Idle the transport
  /// now.
  bool idle_after_reply (void);

  /// Call the implementation method after obtaining the lock.
  virtual void close_connection (void);

  //@}

  /** @name Template methods
   *
   * The Transport class uses the Template Method Pattern to implement
   * the protocol specific functionality.
   * Implementors of a pluggable protocol should override the
   * following methods with the semantics documented below.
   */
  /**
   * Initializing the messaging object. This would be used by the
   * connector side. On the acceptor side the connection handler
   * would take care of the messaging objects.
   */
  void messaging_init (TAO_GIOP_Message_Version const &version);

  /// Extracts the list of listen points from the @a cdr stream. The
  /// list would have the protocol specific details of the
  /// ListenPoints
  virtual int tear_listen_point_list (TAO_InputCDR &cdr);

  /// Hooks that can be overridden in concrete transports.
  /**
   * These hooks are invoked just after connection establishment (or
   * after a connection is fetched from cache). The
   * return value signifies whether the invoker should proceed  with
   * post connection establishment activities. Protocols like SSLIOP
   * need this to verify whether connections already established have
   * valid certificates. There are no pre_connect_hooks () since the
   * transport doesn't exist before a connection establishment. :-)
   *
   * @note The methods are not made const with a reason.
   */
  virtual bool post_connect_hook (void);

  /// Memory management routines.
  /**
   * Forwards to event handler.
   */
  ACE_Event_Handler::Reference_Count add_reference (void);
  ACE_Event_Handler::Reference_Count remove_reference (void);

  /// Return the messaging object that is used to format the data that
  /// needs to be sent.
  TAO_GIOP_Message_Base * messaging_object (void);

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
   * member that functions as an adapter between the ACE_Reactor
   * framework and the TAO pluggable protocol framework.
   * In all the protocols implemented so far this role is fullfilled
   * by an instance of ACE_Svc_Handler.
   *
   * @todo Since we only use a limited functionality of
   * ACE_Svc_Handler we could probably implement a generic
   * adapter class (TAO_Transport_Event_Handler or something), this
   * will reduce footprint and simplify the process of implementing a
   * pluggable protocol.
   *
   * @todo This method has to be renamed to event_handler()
   */
  virtual ACE_Event_Handler * event_handler_i (void) = 0;

  /// Is this transport really connected
  bool is_connected (void) const;

  /// Was a connection seen as closed during a read
  bool connection_closed_on_read (void) const;

  /// Perform all the actions when this transport get opened
  bool post_open (size_t id);

  /// do what needs to be done when closing the transport
  void pre_close (void);

  /// Get the connection handler for this transport
  TAO_Connection_Handler * connection_handler (void);

  /// Accessor for the output CDR stream
  TAO_OutputCDR &out_stream (void);

  /// Accessor for synchronizing Transport OutputCDR access
  TAO_SYNCH_MUTEX &output_cdr_lock (void);

  /// Can the transport be purged?
  bool can_be_purged (void);

  virtual void set_bidir_context_info (TAO_Operation_Details &opdetails);

  /*
   * Specialization hook to add public methods from
   * concrete transport implementations to TAO's transport
   * class
   */
  //@@ TAO_TRANSPORT_SPL_PUBLIC_METHODS_ADD_HOOK

protected:

  virtual TAO_Connection_Handler * connection_handler_i (void) = 0;

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

  /// Recache ourselves in the cache
  int recache_transport (TAO_Transport_Descriptor_Interface* desc);

  /// Callback to read incoming data
  /**
   * The ACE_Event_Handler adapter invokes this method as part of its
   * handle_input() operation.
   *
   * @todo the method name is confusing! Calling it handle_input()
   * would probably make things easier to understand and follow!
   *
   * Once a complete message is read the Transport class delegates on
   * the Messaging layer to invoke the right upcall (on the server) or
   * the TAO_Reply_Dispatcher (on the client side).
   *
   * @param max_wait_time In some cases the I/O is synchronous, e.g. a
   * thread-per-connection server or when Wait_On_Read is enabled.  In
   * those cases a maximum read time can be specified.
   */
  virtual int handle_input (TAO_Resume_Handle &rh,
                            ACE_Time_Value *max_wait_time = 0);

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
                            TAO_Message_Semantics message_semantics,
                            ACE_Time_Value *max_time_wait) = 0;

  /// This method formats the stream and then sends the message on the
  /// transport.
  /**
   * Once the ORB is prepared to receive a reply (see send_request()
   * above), and all the arguments have been marshaled the CDR stream
   * must be 'formatted', i.e. the message_size field in the GIOP
   * header can finally be set to the proper value.
   *
   */
  virtual int send_message (TAO_OutputCDR &stream,
                            TAO_Stub *stub = 0,
                            TAO_ServerRequest *request = 0,
                            TAO_Message_Semantics message_semantics = TAO_Message_Semantics (),
                            ACE_Time_Value *max_time_wait = 0) = 0;

  /// Sent the contents of @a message_block
  /**
   * @param stub The object reference used for this operation, useful
   *             to obtain the current policies.
   * @param message_semantics If this is set to TAO_TWO_REQUEST
   *        this method will block until the operation is completely
   *        written on the wire. If it is set to other values this
   *        operation could return.
   * @param message_block The CDR encapsulation of the GIOP message
   *             that must be sent.  The message may consist of
   *             multiple Message Blocks chained through the cont()
   *             field.
   * @param max_wait_time The maximum time that the operation can
   *             block, used in the implementation of timeouts.
   */
  virtual int send_message_shared (TAO_Stub *stub,
                                   TAO_Message_Semantics message_semantics,
                                   const ACE_Message_Block *message_block,
                                   ACE_Time_Value *max_wait_time);

protected:

  /// Process the message by sending it to the higher layers of the
  /// ORB.
  int process_parsed_messages (TAO_Queued_Data *qd,
                               TAO_Resume_Handle &rh);

  /// Implement send_message_shared() assuming the handler_lock_ is
  /// held.
  int send_message_shared_i (TAO_Stub *stub,
                             TAO_Message_Semantics message_semantics,
                             const ACE_Message_Block *message_block,
                             ACE_Time_Value *max_wait_time);

  /// Queue a message for @a message_block
  /// @param max_wait_time The maximum time that the operation can
  ///            block, used in the implementation of timeouts.
  /// @param back If true, the message will be pushed to the back of the queue.
  ///        If false, the message will be pushed to the front of the queue.
  int queue_message_i (const ACE_Message_Block *message_block,
                       ACE_Time_Value *max_wait_time, bool back=true);

  /**
   * @brief Re-factor computation of I/O timeouts based on operation
   * timeouts.
   * Depending on the wait strategy, we need to timeout I/O operations or
   * not.  For example, if we are using a non-blocking strategy, we want
   * to pass 0 to all I/O operations, and rely on the ACE_NONBLOCK
   * settings on the underlying sockets.  However, for blocking strategies
   * we want to pass the operation timeouts, to respect the application
   * level policies.
   *
   * This function was introduced as part of the fixes for bug 3647.
   */
  ACE_Time_Value const *io_timeout(
      TAO::Transport::Drain_Constraints const & dc) const;

public:
  /// Format and queue a message for @a stream
  /// @param max_wait_time The maximum time that the operation can
  ///            block, used in the implementation of timeouts.
  int format_queue_message (TAO_OutputCDR &stream,
                            ACE_Time_Value *max_wait_time,
                            TAO_Stub* stub);

  /**
   * This is a very specialized interface to send a simple chain of
   * messages through the Transport.  The only place we use this interface
   * is in GIOP_Message_Base.cpp, to send error messages (i.e., an
   * indication that we received a malformed GIOP message,) and to close
   * the connection.
   *
   */
  int send_message_block_chain (const ACE_Message_Block *message_block,
                                size_t &bytes_transferred,
                                ACE_Time_Value *max_wait_time = 0);

  /// Send a message block chain, assuming the lock is held
  int send_message_block_chain_i (const ACE_Message_Block *message_block,
                                  size_t &bytes_transferred,
                                  TAO::Transport::Drain_Constraints const & dc);

  /// Cache management
  int purge_entry (void);

  /// Cache management
  int make_idle (void);

  /// Cache management
  int update_transport (void);

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
  int handle_timeout (const ACE_Time_Value &current_time, const void* act);

  /// Accessor to recv_buffer_size_
  size_t recv_buffer_size (void) const;

  /// Accessor to sent_byte_count_
  size_t sent_byte_count (void) const;

  /// CodeSet Negotiation - Get the char codeset translator factory
  TAO_Codeset_Translator_Base *char_translator (void) const;

  /// CodeSet Negotiation - Get the wchar codeset translator factory
  TAO_Codeset_Translator_Base *wchar_translator (void) const;

  /// CodeSet negotiation - Set the char codeset translator factory
  void char_translator (TAO_Codeset_Translator_Base *);

  /// CodeSet negotiation - Set the wchar codeset translator factory
  void wchar_translator (TAO_Codeset_Translator_Base *);

  /// Use the Transport's codeset factories to set the translator for input
  /// and output CDRs.
  void assign_translators (TAO_InputCDR *, TAO_OutputCDR *);

  /// It is necessary to clear the codeset translator when a CDR stream
  /// is used for more than one GIOP message. This is required since the
  /// header must not be translated, whereas the body must be.
  void clear_translators (TAO_InputCDR *, TAO_OutputCDR *);

  /// Return true if the tcs has been set
  CORBA::Boolean is_tcs_set() const;

  /// Set the state of the first_request_ to flag.
  void first_request_sent (bool flag = false);

  /// Get the first request flag
  bool first_request () const;

  /// Notify all the components inside a Transport when the underlying
  /// connection is closed.
  void send_connection_closed_notifications (void);

  /// Transport statistics
  TAO::Transport::Stats* stats (void) const;

private:

  /// Helper method that returns the Transport Cache Manager.
  TAO::Transport_Cache_Manager &transport_cache_manager (void);

  /// Send some of the data in the queue.
  /**
   * As the outgoing data is drained this method is invoked to send as
   * much of the current message as possible.
   */
  Drain_Result drain_queue (TAO::Transport::Drain_Constraints const & dc);

  /// Implement drain_queue() assuming the lock is held
  Drain_Result drain_queue_i (TAO::Transport::Drain_Constraints const & dc);

  /// Check if there are messages pending in the queue
  /**
   * This version assumes that the lock is already held.  Use with
   * care!
   *
   * @return true if the queue is empty
   */
  bool queue_is_empty_i (void) const;

  /// A helper routine used in drain_queue_i()
  Drain_Result drain_queue_helper (int &iovcnt, iovec iov[],
      TAO::Transport::Drain_Constraints const & dc);

  /// These classes need privileged access to:
  /// - schedule_output_i()
  /// - cancel_output_i()
  friend class TAO_Reactive_Flushing_Strategy;
  friend class TAO_Leader_Follower_Flushing_Strategy;

  /// Needs priveleged access to
  /// event_handler_i ()
  friend class TAO_Thread_Per_Connection_Handler;

  /// Schedule handle_output() callbacks
  int schedule_output_i (void);

  /// Cancel handle_output() callbacks
  int cancel_output_i (void);

  /// Cleanup the queue.
  /**
   * Exactly @a byte_count bytes have been sent, the queue must be
   * cleaned up as potentially several messages have been completely
   * sent out.
   * It leaves on head_ the next message to send out.
   */
  void cleanup_queue (size_t byte_count);

  /// Cleanup the complete queue
  void cleanup_queue_i ();

  /// Check if the buffering constraints have been reached
  bool check_buffering_constraints_i (TAO_Stub *stub, bool &must_flush);

  /// Send a synchronous message, i.e. block until the message is on
  /// the wire
  int send_synchronous_message_i (const ACE_Message_Block *message_block,
                                  ACE_Time_Value *max_wait_time);

  /// Send a reply message, i.e. do not block until the message is on
  /// the wire, but just return after adding them to the queue.
  int send_reply_message_i (const ACE_Message_Block *message_block,
                            ACE_Time_Value *max_wait_time);

  /// Send an asynchronous message, i.e. do not block until the message is on
  /// the wire
  int send_asynchronous_message_i (TAO_Stub *stub,
                                   const ACE_Message_Block *message_block,
                                   ACE_Time_Value *max_wait_time);

  /// A helper method used by send_synchronous_message_i() and
  /// send_reply_message_i(). Reusable code that could be used by both
  /// the methods.
  int send_synch_message_helper_i (TAO_Synch_Queued_Message &s,
                                   ACE_Time_Value *max_wait_time);

  /// Check if the flush timer is still pending
  int flush_timer_pending (void) const;

  /// The flush timer expired or was explicitly cancelled, mark it as
  /// not pending
  void reset_flush_timer (void);

  /// Print out error messages if the event handler is not valid
  void report_invalid_event_handler (const char *caller);

  /// Is invoked by handle_input operation. It consolidate message on
  /// top of incoming_message_stack.  The amount of missing data is
  /// known and recv operation copies data directly into message buffer,
  /// as much as a single recv-invocation provides.
  int handle_input_missing_data (TAO_Resume_Handle &rh,
                                 ACE_Time_Value *max_wait_time,
                                 TAO_Queued_Data *q_data);

  /// Is invoked by handle_input operation. It parses new messages from input stream
  /// or consolidates messages whose header has been partially read, the message
  /// size being unknown so far. It parses as much data as a single recv-invocation provides.
  int handle_input_parse_data (TAO_Resume_Handle &rh,
                               ACE_Time_Value *max_wait_time);

  /// Is invoked by handle_input_parse_data. Parses all messages remaining
  /// in @a message_block.
  int handle_input_parse_extra_messages (ACE_Message_Block &message_block);

  /// @return -1 error, otherwise 0
  int consolidate_enqueue_message (TAO_Queued_Data *qd);

  /// @return -1 error, otherwise 0
  int consolidate_process_message (TAO_Queued_Data *qd, TAO_Resume_Handle &rh);

  /*
   * Process the message that is in the head of the incoming queue.
   * If there are more messages in the queue, this method calls
   * this->notify_reactor () to wake up a thread
   * @retval -1 on error
   * @retval 0 if successfully processing enqueued messages
   * @retval 1 if no message present in queue
   */
  int process_queue_head (TAO_Resume_Handle &rh);

  /*
   * This call prepares a new handler for the notify call and sends a
   * notify () call to the reactor.
   */
  int notify_reactor (void);

protected:
  /*
   * Same as notify_reactor above but does NOT first check for a
   * registered TAO_Wait_Strategy.
   */
  int notify_reactor_now (void);

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_Transport (const TAO_Transport &))
  ACE_UNIMPLEMENTED_FUNC (TAO_Transport &operator= (const TAO_Transport &))

  /// Assume the lock is held
  void send_connection_closed_notifications_i (void);

  /// Allocate a partial message block and store it in our
  /// partial_message_ data member.
  void allocate_partial_message_block (void);

  /**
   * Return true if blocking I/O should be used for sending synchronous
   * (two-way, reliable oneways, etc.) messages.  This is determined based
   * on the current flushing and waiting strategies.
   */
  bool using_blocking_io_for_synch_messages() const;

  /**
   * Return true if blocking I/O should be used for sending asynchronous
   * (AMI calls, non-blocking oneways, responses to operations, etc.)
   * messages.  This is determined based on the current flushing strategy.
   */
  bool using_blocking_io_for_asynch_messages() const;

  /*
   * Specialization hook to add concrete private methods from
   * TAO's protocol implementation onto the base Transport class
   */

  //@@ TAO_TRANSPORT_SPL_PRIVATE_METHODS_ADD_HOOK

protected:

  /// IOP protocol tag.
  CORBA::ULong const tag_;

  /// Global orbcore resource.
  TAO_ORB_Core * const orb_core_;

  /// Our entry in the cache. We don't own this. It is here for our
  /// convenience. We cannot just change things around.
  TAO::Transport_Cache_Manager::HASH_MAP_ENTRY *cache_map_entry_;

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

  TAO::Connection_Role opening_connection_role_;

  /// Implement the outgoing data queue
  TAO_Queued_Message *head_;
  TAO_Queued_Message *tail_;

  /// Queue of the consolidated, incoming messages..
  TAO_Incoming_Message_Queue incoming_message_queue_;

  /// Stack of incoming fragments, consolidated messages
  /// are going to be enqueued in "incoming_message_queue_"
  TAO::Incoming_Message_Stack incoming_message_stack_;

  /// The queue will start draining no later than <queeing_deadline_>
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
   * @c TAO_ORB_Core::resource_factory()->create_cached_connection_lock().
   * This way, one can use a lock appropriate for the type of system, i.e.,
   * a null lock for single-threaded systems, and a real lock for
   * multi-threaded systems.
   */
  mutable ACE_Lock *handler_lock_;

  /// A unique identifier for the transport.
  /**
   * This never *never* changes over the lifespan, so we don't have to worry
   * about locking it.
   *
   * HINT: Protocol-specific transports that use connection handler
   * might choose to set this to the handle for their connection.
   */
  size_t id_;

  /// Used by the LRU, LFU and FIFO Connection Purging Strategies.
  unsigned long purging_order_;

  /// Size of the buffer received.
  size_t recv_buffer_size_;

  /// Number of bytes sent.
  size_t sent_byte_count_;

  /// Is this transport really connected or not. In case of oneways with
  /// SYNC_NONE Policy we don't wait until the connection is ready and we
  /// buffer the requests in this transport until the connection is ready
  bool is_connected_;

  /// Track if connection was seen as closed during a read so that
  /// invocation can optionally be retried using a different profile.
  /// Note that this could result in violate the "at most once" CORBA
  /// semantics.
  bool connection_closed_on_read_;

private:

  /// Our messaging object.
  TAO_GIOP_Message_Base *messaging_object_;

  /// @@Phil, I think it would be nice if we could think of a way to
  /// do the following.
  /// We have been trying to use the transport for marking about
  /// translator factories and such! IMHO this is a wrong encapulation
  /// ie. trying to populate the transport object with these
  /// details. We should probably have a class something like
  /// TAO_Message_Property or TAO_Message_Translator or whatever (I am
  /// sure you get the idea) and encapsulate all these
  /// details. Coupling these seems odd. if I have to be more cynical
  /// we can move this to the connection_handler and it may more sense
  /// with the DSCP stuff around there. Do you agree?

  /// Additional member values required to support codeset translation
  TAO_Codeset_Translator_Base *char_translator_;
  TAO_Codeset_Translator_Base *wchar_translator_;

  /// The tcs_set_ flag indicates that negotiation has occured and so the
  /// translators are correct, since a null translator is valid if both ends
  /// are using the same codeset, whatever that codeset might be.
  CORBA::Boolean tcs_set_;

  /// First_request_ is true until the first request is sent or received. This
  /// is necessary since codeset context information is necessary only on the
  /// first request. After that, the translators are fixed for the life of the
  /// connection.
  bool first_request_;

  /// Holds the partial GIOP message (if there is one)
  ACE_Message_Block* partial_message_;

#if TAO_HAS_SENDFILE == 1
  /// mmap()-based allocator used to allocator output CDR buffers.
  /**
   * If this pointer is non-zero, sendfile() will be used to send data
   * in a TAO_OutputCDR stream instance.
   */
  TAO_MMAP_Allocator * const mmap_allocator_;
#endif  /* TAO_HAS_SENDFILE==1 */

#if TAO_HAS_TRANSPORT_CURRENT == 1
  /// Statistics
  TAO::Transport::Stats* stats_;
#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

  /// Indicate that flushing needs to be done in post_open()
  bool flush_in_post_open_;

  /// lock for synchronizing Transport OutputCDR access
  mutable TAO_SYNCH_MUTEX output_cdr_mutex_;

  /*
   * specialization hook to add class members from concrete
   * transport class onto the base transport class. Please
   * add any private members to this class *before* this hook.
   */
  //@@ TAO_TRANSPORT_SPL_DATA_MEMBERS_ADD_HOOK
};

/*
 * Hook to add external typedefs and specializations to
 * TAO's transport implementation.
 */

//@@ TAO_TRANSPORT_SPL_EXTERN_ADD_HOOK

#if TAO_HAS_TRANSPORT_CURRENT == 1
namespace TAO
{
  namespace Transport
  {
    /*
     * @class Stats
     *
     * @brief Used to collect stats on a transport.
     *
     * The base class in (potentialy) extensible hierarchy used to
     * specialize the information available for a specific protocol.
     *
     * This class is necessary for the implementation of the Transport
     * Current feature.
     *
     * <B>See Also:</B>
     *
     * https://svn.dre.vanderbilt.edu/viewvc/Middleware/trunk/TAO/docs/transport_current/index.html?revision=HEAD
     *
     */
    class TAO_Export Stats
    {
    public:
      Stats ();
      virtual ~Stats ();

      void messages_sent (size_t message_length);
      CORBA::LongLong messages_sent (void) const;
      CORBA::LongLong bytes_sent (void) const;

      void messages_received (size_t message_length);
      CORBA::LongLong messages_received (void) const;
      CORBA::LongLong bytes_received (void) const;

      void opened_since (const ACE_Time_Value& tv);
      const ACE_Time_Value& opened_since (void) const;

    private:
      // The bytes_rcvd_.samples_count() could have been used instead,
      // however there was a suspicion that 32 bits would be
      // insufficient.
      CORBA::LongLong messages_rcvd_;

      // The bytes_sent_.samples_count() could have been used instead,
      // however there was a suspicion that 32 bits would be
      // insufficient.
      CORBA::LongLong messages_sent_;

      ACE_Basic_Stats bytes_rcvd_;
      ACE_Basic_Stats bytes_sent_;

      ACE_Time_Value  opened_since_;
    };
  }
}
#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Transport.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_TRANSPORT_H */
