// This may look like C, but it's really -*- C++ -*-
// $Id$



// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Pluggable.h
//
// = DESCRIPTION
//     Interface for the TAO pluggable protocol framework.
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PLUGGABLE_H
#define TAO_PLUGGABLE_H

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Message_Queue.h"
#include "tao/Sequence.h"
#include "tao/Typecode.h"
#include "tao/IOPC.h"


// Forward declarations.
class ACE_Addr;
class ACE_Reactor;
class TAO_ORB_Core;

class TAO_Stub;
class TAO_Profile;
class TAO_MProfile;
class TAO_Resource_Factory;

class TAO_Reply_Dispatcher;
class TAO_Transport_Mux_Strategy;
class TAO_Wait_Strategy;

class TAO_Pluggable_Messaging_Interface;
class TAO_Target_Specification;
class TAO_Operation_Details;

typedef ACE_Message_Queue<ACE_NULL_SYNCH> TAO_Transport_Buffering_Queue;

class TAO_Export TAO_Transport
{
  // = TITLE
  //   Generic definitions for the Transport class.
  //
  // = DESCRIPTION
  //   The transport object is created in the Service handler
  //   constructor and deleted in the Service Handler's destructor!!

  friend class TAO_Transport_Sync_Strategy;
  friend class TAO_Eager_Buffering_Sync_Strategy;
  friend class TAO_Delayed_Buffering_Sync_Strategy;

public:
  TAO_Transport (CORBA::ULong tag,
                 TAO_ORB_Core *orb_core);
  // default creator, requres the tag value be supplied.

  virtual ~TAO_Transport (void);
  // destructor

  CORBA::ULong tag (void) const;
  // The tag, each concrete class will have a specific tag value.

  virtual void close_connection (void) = 0;
  // Call the corresponding connection handler's <close>
  // method.

  virtual int idle (void) = 0;
  // Idles the corresponding connection handler.

  virtual ACE_HANDLE handle (void) = 0;
  // This method provides a way to gain access to the underlying file
  // handle used by the reactor.

  virtual ACE_Event_Handler *event_handler (void) = 0;
  // This method provides a way to gain access to the underlying event
  // handler used by the reactor.

  virtual ssize_t send (TAO_Stub *stub,
                        int two_way,
                        const ACE_Message_Block *mblk,
                        const ACE_Time_Value *s = 0) = 0;
  virtual ssize_t send (const ACE_Message_Block *mblk,
                        const ACE_Time_Value *s = 0) = 0;
  // Write the complete Message_Block chain to the connection.
  // @@ The ACE_Time_Value *s is just a place holder for now.  It is
  // not clear this this is the best place to specify this.  The actual
  // timeout values will be kept in the Policies.

  virtual ssize_t send (const u_char *buf,
                        size_t len,
                        const ACE_Time_Value *s = 0) = 0;
  // Write the contents of the buffer of length len to the connection.

  virtual ssize_t recv (char *buf,
                        size_t len,
                        const ACE_Time_Value *s = 0) = 0;
  // Read len bytes from into buf.
  // @@ The ACE_Time_Value *s is just a place holder for now.  It is
  // not clear this this is the best place to specify this.  The actual
  // timeout values will be kept in the Policies.


  virtual void start_request (TAO_ORB_Core *orb_core,
                              TAO_Target_Specification &spec,
                              TAO_OutputCDR &output,
                              CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Fill into <output> the right headers to make a request.

  virtual void start_locate (TAO_ORB_Core *orb_core,
                             TAO_Target_Specification &spec,
                             TAO_Operation_Details &opdetails,
                             TAO_OutputCDR &output,
                             CORBA::Environment &ACE_TRY_ENV =
                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Fill into <output> the right headers to make a locate request.

  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &stream,
                            int twoway,
                            ACE_Time_Value *max_time_wait) = 0;
  // Depending on the concurrency strategy used by the transport it
  // may be required to setup state to receive a reply before the
  // request is sent.
  // Using this method, instead of send(), allows the transport (and
  // wait strategy) to take appropiate action.

  virtual CORBA::Boolean 
  send_request_header (TAO_Operation_Details &op_details,
                       TAO_Target_Specification &spec,
                       TAO_OutputCDR &msg) = 0;
  // This is a request for the transport object to write a request
  // header before it sends out a request
                                
  TAO_ORB_Core *orb_core (void) const;
  // Access the ORB that owns this connection.

  TAO_Transport_Mux_Strategy *tms (void) const;
  // Get the TMS used by this Transport object.

  TAO_Wait_Strategy *wait_strategy (void) const;
  // Return the Wait strategy used by the Transport.

  virtual int handle_client_input (int block = 0,
                                   ACE_Time_Value *max_wait_time = 0);
  // Read and handle the reply. Returns 0 when there is Short Read on
  // the connection. Returns 1 when the full reply is read and
  // handled. Returns -1 on errors.
  // If <block> is 1, then reply is read in a blocking manner.

  virtual int register_handler (void);
  // Register the handler with the reactor. Will be called by the Wait
  // Strategy if Reactor is used  for that strategy. Default
  // implementation out here returns -1 setting <errno> to ENOTSUP.

  // = Setting the Transport object in Idle state. These methods are
  //   routed through the TMS object. The TMS strategies implement the
  //   methods accordingly.

  virtual int idle_after_send (void);
  // Request has been just sent, but the reply is not received. Idle
  // the transport now.

  virtual int idle_after_reply (void);
  // Request is sent and the reply is received. Idle the transport
  // now.

  virtual ACE_SYNCH_CONDITION *leader_follower_condition_variable (void);
  // Return the TSS leader follower condition variable used in the
  // Wait Strategy. Muxed Leader Follower implementation returns a
  // valid condition variable, others return 0.

  virtual TAO_Transport_Buffering_Queue &buffering_queue (void);
  // Queue for buffering transport messages.

  long buffering_timer_id (void) const;
  void buffering_timer_id (long);
  // Timer id associated with buffering.

  const ACE_Time_Value &buffering_timeout_value (void) const;
  void buffering_timeout_value (const ACE_Time_Value &time);
  // Timeout value associated with buffering.

  ssize_t send_buffered_messages (const ACE_Time_Value *max_wait_time = 0);
  // Send any messages that have been buffered.

  virtual int
  messaging_init (CORBA::Octet major,
                  CORBA::Octet minor);
  // Initialising the messaging object. This would be used by the
  // connector  side. On the acceptor side the connection handler
  // would take care of the messaging objects.

protected:

  void dequeue_head (void);

  void dequeue_all (void);

  void reset_queued_message (ACE_Message_Block *message_block,
                             size_t bytes_delivered);

  void reset_sent_message (ACE_Message_Block *message_block,
                           size_t bytes_delivered);

  void reset_message (ACE_Message_Block *message_block,
                      size_t bytes_delivered,
                      int queued_message);

  CORBA::ULong tag_;
  // IOP protocol tag.

  TAO_ORB_Core *orb_core_;
  // Global orbcore resource.

  TAO_Transport_Mux_Strategy *tms_;
  // Strategy to decide whether multiple requests can be sent over the
  // same connection or the connection is exclusive for a request.

  TAO_Wait_Strategy *ws_;
  // Strategy for waiting for the reply after sending the request.

  TAO_Transport_Buffering_Queue *buffering_queue_;
  // Queue for buffering transport messages.

  long buffering_timer_id_;
  // Buffering timer id.

  ACE_Time_Value buffering_timeout_value_;
  // Buffering timeout value.
};

// ****************************************************************

class TAO_Export TAO_Acceptor
{
  // = TITLE
  //   Abstract Acceptor class used for pluggable protocols.
  //
  // = DESCRIPTION
  //   Base class for the Acceptor bridge calss.
public:
  TAO_Acceptor (CORBA::ULong tag);

  virtual ~TAO_Acceptor (void);
  // Destructor

  CORBA::ULong tag (void) const;
  // The tag, each concrete class will have a specific tag value.

  CORBA::Short priority (void) const;
  // The priority for this endpoint.

  virtual int open (TAO_ORB_Core *orb_core,
                    int version_major,
                    int version_minor,
                    const char *address,
                    const char *options = 0) = 0;
  // Method to initialize acceptor for address.

  virtual int open_default (TAO_ORB_Core *orb_core,
                            const char *options = 0) = 0;
  // Open an acceptor on the default endpoint for this protocol

  virtual int close (void) = 0;
  // Closes the acceptor

  virtual int create_mprofile (const TAO_ObjectKey &object_key,
                               TAO_MProfile &mprofile) = 0;
  // Create the corresponding profile for this endpoint.

  virtual int is_collocated (const TAO_Profile* profile) = 0;
  // Return 1 if the <profile> has the same endpoint as the acceptor.

  virtual CORBA::ULong endpoint_count (void) = 0;
  // Returns the number of endpoints this acceptor is listening on.  This
  // is used for determining how many profiles will be generated
  // for this acceptor.

protected:
  CORBA::Short priority_;
  // The priority for this endpoint

private:
  CORBA::ULong tag_;
  // IOP protocol tag.

};

class TAO_Export TAO_Connector
{
  // = TITLE
  //   Generic Connector interface definitions.
  //
  // = DESCRIPTION
  //   Base class for connector bridge object.
public:

  TAO_Connector (CORBA::ULong tag);
  // default constructor.

  virtual ~TAO_Connector (void);
  // the destructor.

  CORBA::ULong tag (void) const;
  // The tag identifying the specific ORB transport layer protocol.
  // For example TAO_TAG_IIOP_PROFILE = 0.  The tag is used in the
  // IOR to identify the type of profile included. IOR -> {{tag0,
  // profile0} {tag1, profole1} ...}  GIOP.h defines typedef
  // CORBA::ULong TAO_IOP_Profile_ID;

  int make_mprofile (const char *ior,
                     TAO_MProfile &mprofile,
                     CORBA::Environment &ACE_TRY_ENV);
  // Parse a string containing a URL style IOR and return an
  // MProfile.

  virtual int open (TAO_ORB_Core *orb_core) = 0;
  //  Initialize object and register with reactor.

  virtual int close (void) = 0;
  // Shutdown Connector bridge and concreate Connector.

  virtual int connect (TAO_Profile *profile,
                       TAO_Transport *&,
                       ACE_Time_Value *max_wait_time) = 0;
  // To support pluggable we need to abstract away the connect()
  // method so it can be called from the GIOP code independant of the
  // actual transport protocol in use.

  virtual int preconnect (const char *preconnections) = 0;
  // Initial set of connections to be established.

  virtual TAO_Profile *create_profile (TAO_InputCDR& cdr) = 0;
  // Create a profile for this protocol and initialize it based on the
  // encapsulation in <cdr>

  virtual int check_prefix (const char *endpoint) = 0;
  // Check that the prefix of the provided endpoint is valid for use
  // with a given pluggable protocol.

  virtual char object_key_delimiter (void) const = 0;
  // Return the object key delimiter to use or expect.

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  virtual int purge_connections (void) = 0;
  // Purge "old" connections.
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

protected:
  virtual void make_profile (const char *endpoint,
                             TAO_Profile *&,
                             CORBA::Environment &ACE_TRY_ENV) = 0;
  // Create a profile with a given endpoint.

private:
  CORBA::ULong tag_;
  // IOP protocol tag.
};

#if defined (__ACE_INLINE__)
# include "tao/Pluggable.i"
#endif /* __ACE_INLINE__ */

#endif  /* TAO_PLUGGABLE_H */
