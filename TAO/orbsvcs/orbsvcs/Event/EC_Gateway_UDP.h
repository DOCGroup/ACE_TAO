// -*- C++ -*-

/**
 *  @file   EC_Gateway_UDP.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 *
 * Define helper classes to propagate events between ECs using
 * either UDP or multicast.
 * The architecture is a bit complicated and deserves some
 * explanation: sending the events over UDP (or mcast) is easy, a
 * Consumer (TAO_ECG_UDP_Sender) subscribes for a certain set of
 * events, its push() method marshalls the event set into a CDR
 * stream that is sent using an ACE_SOCK_CODgram. The subscription
 * set and IP address can be configured.
 * Another helper class (TAO_ECG_UDP_Receiver) acts as a supplier of
 * events; it receives a callback when an event is available on an
 * ACE_SOCK_Dgram, it demarshalls the event and pushes it to the
 * EC.  Two ACE_Event_Handler classes are provided that can forward
 * the events to this Supplier: TAO_ECG_Mcast_EH can receive events
 * from a multicast group; TAO_ECG_UDP_EH can receive events from a
 * regular UDP socket.
 *
 * Matching of the events types carried by a multicast group (or IP
 * address) is up to the application. Gateway classes can be
 * implemented to automate this: the EC informs its gateways about
 * local changes in the subscriptions (for example), the Gateway
 * could then consult an administrative server that will inform it
 * which multicast groups carry those event types, it can then
 * create the proper event handlers and TAO_ECG_Receivers. An
 * analogous class can be implemented for the Supplier side.
 *
 * An alternative approach would be to look the current set of
 * multicast groups and the events carried on each, using that
 * information a suitable TAO_ECG_UDP_Receiver can be configured
 * (and of course the Senders on the client side).
 *
 * @todo The class makes an extra copy of the events, we need to
 * investigate if closer collaboration with its collocated EC could
 * be used to remove that copy.
 *
 *
 */

#ifndef TAO_EC_GATEWAY_UDP_H
#define TAO_EC_GATEWAY_UDP_H
#include "ace/pre.h"

#include "orbsvcs/RtecUDPAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventChannelAdminS.h"

#include "orbsvcs/Event/event_export.h"

#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/SOCK_CODgram.h"
#include "ace/Hash_Map_Manager.h"

class TAO_ECG_UDP_Out_Endpoint;

// ****************************************************************

/**
 * @class TAO_ECG_UDP_Out_Endpoint
 *
 * @brief Maintains information about an outgoing endpoint.
 *
 * UDP senders can share a single endpoint to send UDP packets,
 * but there is more state associated with this endpoint than its
 * datagram SAP; for instance we need to keep track of the request
 * id.
 */
class TAO_RTEvent_Export TAO_ECG_UDP_Out_Endpoint
{
public:
  /// Constructor
  TAO_ECG_UDP_Out_Endpoint (void);

  /// Constructor
  ~TAO_ECG_UDP_Out_Endpoint (void);

  /// Obtain the datagram associated with this endpoint. Clients of
  /// this class must open, and register (if necessary) this datagram.
  ACE_SOCK_Dgram& dgram (void);

  /// Obtain the next request id.
  CORBA::ULong next_request_id (void);

  /// The endpoint can detect if a data-gram was sent by itself, this
  /// is useful to ignore or remove messages sent by the same process.
  CORBA::Boolean is_loopback (const ACE_INET_Addr& from);

private:
  /// The request id....
  ACE_Atomic_Op<TAO_SYNCH_MUTEX,CORBA::ULong> request_id_generator_;

  /// The datagram....
  ACE_SOCK_Dgram dgram_;

  /// cache the port-number so we can quickly determine if an event is
  /// coming from another endpoint.
  u_short port_number_;

  /// Keep the list of local interfaces, needed for the is_loopback
  /// method.
  size_t if_count_;
  ACE_INET_Addr* ifs_;
};

// ****************************************************************

/**
 * @class TAO_ECG_UDP_Request_Index
 *
 * @brief Index to the request map.
 *
 * This object is used to index the map of incomplete (due to
 * fragmentation) requests.
 */
class TAO_RTEvent_Export TAO_ECG_UDP_Request_Index
{
public:
  /// default copy ctor, dtor and operator=
  TAO_ECG_UDP_Request_Index (void);
  TAO_ECG_UDP_Request_Index (const ACE_INET_Addr& from,
                             CORBA::ULong request_id);

  // The ACE_INLINE macros here are to keep g++ 2.7.X happy,
  // otherwise it thinks they are used as inline functions before
  // beign used as such.... Apparently in the template code for the
  // Hash_Map_Manager.
  /// Return a hash value...
  ACE_INLINE u_long hash (void) const;

  /// Compare
  ACE_INLINE int operator== (const TAO_ECG_UDP_Request_Index& rhs) const;
  ACE_INLINE int operator!= (const TAO_ECG_UDP_Request_Index& rhs) const;

  ACE_INET_Addr from;
  CORBA::ULong request_id;
};

// ****************************************************************

/**
 * @class TAO_ECG_UDP_Request_Entry
 *
 * @brief Keeps information about an incomplete request.
 *
 * When a request arrives in fragments this object is used to
 * keep track of the incoming data.
 */
class TAO_RTEvent_Export TAO_ECG_UDP_Request_Entry
{
public:
  enum {
    ECG_DEFAULT_FRAGMENT_BUFSIZ = 8
  };

  // TAO_ECG_UDP_Request_Entry (void);
  // TAO_ECG_UDP_Request_Entry (const TAO_ECG_UDP_Request_Entry& rhs);
  // TAO_ECG_UDP_Request_Entry& operator=(const TAO_ECG_UDP_Request_Entry& rhs);
  ~TAO_ECG_UDP_Request_Entry (void);

  /// Initialize the fragment, allocating memory, etc.
  TAO_ECG_UDP_Request_Entry (CORBA::Boolean byte_order,
                             CORBA::ULong request_id,
                             CORBA::ULong request_size,
                             CORBA::ULong fragment_count);

  /// Validate a fragment, it should be rejected if it is invalid..
  int validate_fragment (CORBA::Boolean byte_order,
                         CORBA::ULong request_size,
                         CORBA::ULong fragment_size,
                         CORBA::ULong fragment_offset,
                         CORBA::ULong fragment_id,
                         CORBA::ULong fragment_count) const;

  /// Has <fragment_id> been received?
  int test_received (CORBA::ULong fragment_id) const;

  /// Mark <fragment_id> as received, reset timeout counter...
  void mark_received (CORBA::ULong fragment_id);

  /// Is the message complete?
  int complete (void) const;

  /// Return a buffer for the fragment at offset <fragment_offset>
  char* fragment_buffer (CORBA::ULong fragment_offset);

  /// Decode the events, the message must be complete.
  void decode (RtecEventComm::EventSet& event
               TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Increment the timeout counter...
  void inc_timeout (void);

  /// Get the timeout counter....
  int get_timeout (void) const;

private:
  /// This attributes should remain constant in all the fragments, used
  /// for validation....
  CORBA::Boolean byte_order_;
  CORBA::ULong request_id_;
  CORBA::ULong request_size_;
  CORBA::ULong fragment_count_;

  CORBA::ULong timeout_counter_;
  ACE_Message_Block payload_;

  /// This is a bit vector, used to keep track of the received buffers.
  CORBA::ULong* received_fragments_;
  int own_received_fragments_;
  CORBA::ULong received_fragments_size_;
  CORBA::ULong default_received_fragments_[ECG_DEFAULT_FRAGMENT_BUFSIZ];
};

// ****************************************************************

class TAO_ECG_UDP_Receiver;

/**
 * @class TAO_ECG_UDP_TH
 *
 * @brief Timer Handler for the UDP Receivers.
 *
 * This object receives timer events from the reactor and forwards
 * them to the UDP_Receiver; which uses those events to expire old
 * messages that did not receive all their fragments.
 */
class TAO_RTEvent_Export TAO_ECG_UDP_TH : public ACE_Event_Handler
{
public:
  TAO_ECG_UDP_TH (TAO_ECG_UDP_Receiver *recv);

  // Reactor callbacks
  virtual int handle_timeout (const ACE_Time_Value& tv,
                              const void* act);

private:
  /// We callback to this object when a message arrives.
  TAO_ECG_UDP_Receiver* receiver_;
};

// ****************************************************************

/**
 * @class TAO_ECG_UDP_Receiver
 *
 * @brief Decodes events from an ACE_SOCK_Dgram and pushes them to the
 * Event_Channel.
 *
 * This supplier receives events from an ACE_SOCK_Dgram, either
 * from a UDP socket or a Mcast group, decodes them and push them
 * to the EC.
 * = REASSEMBLY
 * Whenever an incomplete fragment is received (one with
 * fragment_count > 1) we allocate an entry for the message in an
 * map indexed by (host,port,request_id).  The entry contains the
 * buffer, a bit vector to keep track of the fragments received
 * so far, and a timeout counter.  This timeout counter is set to
 * 0 on each (new) fragment arrival, and incremented on a regular
 * basis.  If the counter reaches a maximum value the message is
 * dropped.
 * Once all the fragments have been received the message is sent
 * up, and the memory reclaimed. The entry is *not* removed until
 * the timer expires (to handle re-transmitions).
 */
class TAO_RTEvent_Export TAO_ECG_UDP_Receiver : public POA_RtecEventComm::PushSupplier
{
public:
  TAO_ECG_UDP_Receiver (void);

  /**
   * To do its job this class requires to know the local EC it will
   * connect to; it also requires to build an RT_Info for the local
   * scheduler.
   * The <reactor> is used to receive timeout events..
   * The <ignore_from> endpoint is used to remove events generated by
   * the same process.
   * @todo part of the RT_Info is hardcoded, we need to make it
   * parametric.
   */
  void init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
             TAO_ECG_UDP_Out_Endpoint* ignore_from,
             RtecUDPAdmin::AddrServer_ptr addr_server,
             ACE_Reactor *reactor,
             const ACE_Time_Value &expire_interval,
             int max_timeout
             TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Disconnect and shutdown the gateway, no further connectsions
  void shutdown (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// Connect to the EC using the given publications lists.
  void open (RtecEventChannelAdmin::SupplierQOS& pub
             TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Disconnect to the EC.
  virtual void close (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /**
   * The Event_Handlers call this method when data is available at the
   * socket, the <dgram> must be ready for reading and contain a full
   * event.
   */
  int handle_input (ACE_SOCK_Dgram& dgram);

  /// The timer has expired, must update all the unreceived
  /// messages...
  int handle_timeout (const ACE_Time_Value& tv,
                      const void* act);

  // The PushSupplier method.
  virtual void disconnect_push_supplier (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Call the RtecUDPAdmin::AddrServer
  void get_addr (const RtecEventComm::EventHeader& header,
                 RtecUDPAdmin::UDP_Addr_out addr
                 TAO_ENV_ARG_DECL_WITH_DEFAULTS);

private:
  /// The remote and the local EC, so we can reconnect when the list changes.
  RtecEventChannelAdmin::EventChannel_var lcl_ec_;

  /// We talk to the EC (as a consumer) using this proxy.
 RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;

  /// Ignore any events coming from this IP addres.
  TAO_ECG_UDP_Out_Endpoint* ignore_from_;

  /// The server used to map event types into multicast groups.
  RtecUDPAdmin::AddrServer_var addr_server_;

  typedef ACE_Hash_Map_Manager<TAO_ECG_UDP_Request_Index,
                               TAO_ECG_UDP_Request_Entry*,
                               TAO_SYNCH_MUTEX> Request_Map;
  typedef ACE_Hash_Map_Entry<TAO_ECG_UDP_Request_Index,
                             TAO_ECG_UDP_Request_Entry*> Request_Map_Entry;

  /// The map containing all the incoming requests which have been
  /// partially received.
  Request_Map request_map_;

  /// To receive the timeouts..
  TAO_ECG_UDP_TH timeout_handler_;

  /// The reactor we are using for the timeout handler...
  ACE_Reactor* reactor_;

  /// How many timeouts before we expire a message...
  int max_timeout_;
};

// ****************************************************************

/**
 * @class TAO_ECG_UDP_EH
 *
 * @brief Event Handler for UDP messages.
 *
 * This object receives callbacks from the Reactor when data is
 * available on a UDP socket, it forwards to the ECG_UDP_Receiver
 * which reads the events and transform it into an event.
 */
class TAO_RTEvent_Export TAO_ECG_UDP_EH : public ACE_Event_Handler
{
public:
  TAO_ECG_UDP_EH (TAO_ECG_UDP_Receiver *recv);

  /// Open the datagram and register with this->reactor()
  int open (const ACE_INET_Addr& ipaddr,
            int reuse_addr = 0);

  /// Close the datagram and unregister with the reactor.
  int close (void);

  /**
   * Obtain the dgram, this is one of those "controlled violations of
   * type safety", allowing the user to setup options and gain access
   * to low-level features.
   */
  ACE_SOCK_Dgram &dgram (void);

  // Reactor callbacks
  virtual int handle_input (ACE_HANDLE fd);
  virtual ACE_HANDLE get_handle (void) const;

private:
  /// The datagram used to receive the data.
  ACE_SOCK_Dgram dgram_;

  /// We callback to this object when a message arrives.
  TAO_ECG_UDP_Receiver* receiver_;
};

#if defined(__ACE_INLINE__)
#include "EC_Gateway_UDP.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_EVENT_CHANNEL_UDP_H */
