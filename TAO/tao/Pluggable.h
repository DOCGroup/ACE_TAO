// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     Pluggable.h
 *
 *  $Id$
 *
 *   Interface for the TAO pluggable protocol framework.
 *
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_PLUGGABLE_H
#define TAO_PLUGGABLE_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Sequence.h"
#include "tao/Typecode.h"
#include "tao/IOPC.h"
#include "ace/Message_Queue.h"

// Forward declarations.
class ACE_Addr;
class ACE_Reactor;
class TAO_ORB_Core;

class TAO_Stub;
class TAO_Endpoint;
class TAO_Profile;
class TAO_MProfile;
class TAO_Resource_Factory;

class TAO_Reply_Dispatcher;
class TAO_Transport_Mux_Strategy;
class TAO_Wait_Strategy;

class TAO_Pluggable_Messaging_Interface;
class TAO_Target_Specification;
class TAO_Operation_Details;
class TAO_Connection_Descriptor_Interface;
class TAO_Connection_Handler;


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
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Fill into <output> the right headers to make a locate request.
  virtual void start_locate (TAO_ORB_Core *orb_core,
                             TAO_Target_Specification &spec,
                             TAO_Operation_Details &opdetails,
                             TAO_OutputCDR &output,
                             CORBA::Environment &ACE_TRY_ENV =
                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

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
  virtual CORBA::Boolean
  send_request_header (TAO_Operation_Details &op_details,
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
                                    int block = 0);

  /**
   * Register the handler with the reactor. Will be called by the Wait
   * Strategy if Reactor is used  for that strategy. Default
   * implementation out here returns -1 setting <errno> to ENOTSUP.
   */
  virtual int register_handler (void);

  // = Setting the Transport object in Idle state. These methods are
  //   routed through the TMS object. The TMS strategies implement the
  //   methods accordingly.

  /// Request has been just sent, but the reply is not received. Idle
  /// the transport now.
  virtual int idle_after_send (void);

  /// Request is sent and the reply is received. Idle the transport
  /// now.
  virtual int idle_after_reply (void);

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
  virtual int
  messaging_init (CORBA::Octet major,
                  CORBA::Octet minor);

  void dequeue_all (void);

  /// Return the TAO_ORB_Core
  TAO_ORB_Core *orb_core (void);

protected:

  void dequeue_head (void);

  void reset_queued_message (ACE_Message_Block *message_block,
                             size_t bytes_delivered);

  void reset_sent_message (ACE_Message_Block *message_block,
                           size_t bytes_delivered);

  void reset_message (ACE_Message_Block *message_block,
                      size_t bytes_delivered,
                      int queued_message);
private:
  // Prohibited
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
};

// ****************************************************************

/**
 * @class TAO_Acceptor
 *
 * @brief Abstract Acceptor class used for pluggable protocols.
 *
 * Base class for the Acceptor bridge calss.
 */
class TAO_Export TAO_Acceptor
{
public:
  TAO_Acceptor (CORBA::ULong tag);

  /// Destructor
  virtual ~TAO_Acceptor (void);

  /// The tag, each concrete class will have a specific tag value.
  CORBA::ULong tag (void) const;

  /// The priority for this endpoint.
  CORBA::Short priority (void) const;

  /// Method to initialize acceptor for address.
  virtual int open (TAO_ORB_Core *orb_core,
                    int version_major,
                    int version_minor,
                    const char *address,
                    const char *options = 0) = 0;

  /**
   * Open an acceptor with the given protocol version on a default
   * endpoint
   * @@ This method should be pure virtual, but in order to maintain
   *    some form of backward compatibilty, a default implementation
   *    is provided.  Ideally, that default implementation should be
   *    removed in the near future.
   */
  virtual int open_default (TAO_ORB_Core *,
                            int version_major,
                            int version_minor,
                            const char *options = 0) = 0;

  /// Closes the acceptor
  virtual int close (void) = 0;

  /// Create the corresponding profile for this endpoint.
  virtual int create_mprofile (const TAO_ObjectKey &object_key,
                               TAO_MProfile &mprofile) = 0;

  /// Return 1 if the <endpoint> has the same address as the acceptor.
  virtual int is_collocated (const TAO_Endpoint* endpoint) = 0;

  /**
   * Returns the number of endpoints this acceptor is listening on.  This
   * is used for determining how many profiles will be generated
   * for this acceptor.
   */
  virtual CORBA::ULong endpoint_count (void) = 0;

  /**
   * This method fetches the <key> from the <profile>. Protocols that
   * are pluggable can send data that are specific in the
   * <profile_data> field encapsulated as a octet stream. This method
   * allows those  protocols to get the object key from the
   * encapsulation.
   */
  virtual int object_key (IOP::TaggedProfile &profile,
                          TAO_ObjectKey &key) = 0;

protected:
  /// The priority for this endpoint
  CORBA::Short priority_;

private:
  /// IOP protocol tag.
  CORBA::ULong tag_;

};

/**
 * @class TAO_Connector
 *
 * @brief Generic Connector interface definitions.
 *
 * Base class for connector bridge object.
 */
class TAO_Export TAO_Connector
{
public:

  /// default constructor.
  TAO_Connector (CORBA::ULong tag);

  /// the destructor.
  virtual ~TAO_Connector (void);

  /**
   * The tag identifying the specific ORB transport layer protocol.
   * For example TAO_TAG_IIOP_PROFILE = 0.  The tag is used in the
   * IOR to identify the type of profile included. IOR -> {{tag0,
   * profile0} {tag1, profole1} ...}  GIOP.h defines typedef
   * CORBA::ULong TAO_IOP_Profile_ID;
   */
  CORBA::ULong tag (void) const;

  /// Parse a string containing a URL style IOR and return an
  /// MProfile.
  int make_mprofile (const char *ior,
                     TAO_MProfile &mprofile,
                     CORBA::Environment &ACE_TRY_ENV);

  ///  Initialize object and register with reactor.
  virtual int open (TAO_ORB_Core *orb_core) = 0;

  /// Shutdown Connector bridge and concreate Connector.
  virtual int close (void) = 0;

  /**
   * To support pluggable we need to abstract away the connect()
   * method so it can be called from the GIOP code independant of the
   * actual transport protocol in use.
   */
  virtual int connect (TAO_Connection_Descriptor_Interface *desc,
                       TAO_Transport *&,
                       ACE_Time_Value *max_wait_time,
                       CORBA::Environment &ACE_TRY_ENV) = 0;

  /// Initial set of connections to be established.
  virtual int preconnect (const char *preconnections) = 0;

  /// Create a profile for this protocol and initialize it based on the
  /// encapsulation in <cdr>
  virtual TAO_Profile *create_profile (TAO_InputCDR& cdr) = 0;

  /// Check that the prefix of the provided endpoint is valid for use
  /// with a given pluggable protocol.
  virtual int check_prefix (const char *endpoint) = 0;

  /// Return the object key delimiter to use or expect.
  virtual char object_key_delimiter (void) const = 0;

protected:
  /// Create a profile with a given endpoint.
  virtual void make_profile (const char *endpoint,
                             TAO_Profile *&,
                             CORBA::Environment &ACE_TRY_ENV) = 0;

  /// Set the ORB Core pointer
  void orb_core (TAO_ORB_Core *orb_core);

  /// Return the TAO_ORB_Core pointer
  TAO_ORB_Core *orb_core (void);

private:

  /// IOP protocol tag.
  CORBA::ULong tag_;

  /// Pointer to our ORB core
  TAO_ORB_Core *orb_core_;
};

#if defined (__ACE_INLINE__)
# include "tao/Pluggable.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_PLUGGABLE_H */
