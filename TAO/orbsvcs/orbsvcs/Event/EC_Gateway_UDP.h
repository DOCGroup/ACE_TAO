/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   EC_Gateway_UDP
//
// = AUTHOR
//   Carlos O'Ryan
//
// = DESCRIPTION
//   Define helper classes to propagate events between ECs using
//   either UDP or multicast.
//   The architecture is a bit complicated and deserves some
//   explanation: sending the events over UDP (or mcast) is easy, a
//   Consumer (TAO_ECG_UDP_Sender) subscribes for a certain set of
//   events, its push() method marshalls the event set into a CDR
//   stream that is sent using an ACE_SOCK_CODgram. The subscription
//   set and IP address can be configured.
//   Another helper class (TAO_ECG_UDP_Receiver) acts as a supplier of
//   events; it receives a callback when an event is available on an
//   ACE_SOCK_Dgram, it demarshalls the event and pushes it to the
//   EC.  Two ACE_Event_Handler classes are provided that can forward
//   the events to this Supplier: TAO_ECG_Mcast_EH can receive events
//   from a multicast group; TAO_ECG_UDP_EH can receive events from a
//   regular UDP socket.
//
//   Matching of the events types carried by a multicast group (or IP
//   address) is up to the application. Gateway classes can be
//   implemented to automate this: the EC informs its gateways about
//   local changes in the subscriptions (for example), the Gateway
//   could then consult an administrative server that will inform it
//   which multicast groups carry those event types, it can then
//   create the proper event handlers and TAO_ECG_Receivers. An
//   analogous class can be implemented for the Supplier side.
//
//   An alternative approach would be to look the current set of
//   multicast groups and the events carried on each, using that
//   information a suitable TAO_ECG_UDP_Receiver can be configured
//   (and of course the Senders on the client side).
//
// = TODO
//   The class makes an extra copy of the events, we need to
//   investigate if closer collaboration with its collocated EC could
//   be used to remove that copy.
//
// ============================================================================

#ifndef TAO_EC_GATEWAY_UDP_H
#define TAO_EC_GATEWAY_UDP_H

#include "ace/SOCK_CODgram.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Dgram_Mcast.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/RtecUDPAdminS.h"
#include "orbsvcs/orbsvcs_export.h"

class TAO_ECG_UDP_Out_Endpoint;

class TAO_ORBSVCS_Export TAO_ECG_UDP_Sender : public POA_RtecEventComm::PushConsumer
{
  //
  // = TITLE
  //   Send events received from a "local" EC using UDP.
  //
  // = DESCRIPTION
  //   This class connect as a consumer to an EventChannel
  //   and it sends the events using UDP, the UDP address can be a
  //   normal IP address or it can be a multicast group.
  //   The UDP address is obtained from a RtecUDPAdmin::AddrServer
  //   class.
  //   It marshalls the events using TAO CDR classes.
  //
  // = MESSAGE FORMAT
  //
  //   The messages header are encapsulated using CDR, with the
  //   following format:
  //
  //   struct Header {
  //     octet byte_order_flags;
  //     // bit 0 represents the byte order as in GIOP 1.1
  //     // bit 1 is set if this is the last fragment
  //
  //     unsigned long request_id;
  //     // The request ID, senders must not send two requests with
  //     // the same ID, senders can be distinguished using recvfrom..
  //
  //     unsigned long request_size;
  //     // The size of this request, this can be used to pre-allocate
  //     // the request buffer.
  //
  //     unsgined long fragment_size;
  //     // The size of this fragment, excluding the header...
  //
  //     unsigned long fragment_offset;
  //     // Where does this fragment fit in the complete message...
  //
  //     unsigned long fragment_id;
  //     // The ID of this fragment...
  //
  //     unsigned long fragment_count;
  //     // The total number of fragments to expect in this request
  //     // @@ TODO This could be eliminated if efficient reassembly
  //     // could be implemented without it.
  //
  //     octet padding[4];
  //     // Ensures the header ends at an 8-byte boundary.
  //   }; // size (in CDR stream) = 32
  //
  //
public:
  TAO_ECG_UDP_Sender (void);

  enum {
    ECG_HEADER_SIZE = 32,
    ECG_MIN_MTU = 32 + 8,
    ECG_MAX_MTU = 65536, // Really optimistic...
    ECG_DEFAULT_MTU = 1024
  };

  int get_local_addr (ACE_INET_Addr& addr);
  // Get the local endpoint used to send the events.

  void init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
             RtecUDPAdmin::AddrServer_ptr addr_server,
             TAO_ECG_UDP_Out_Endpoint* endpoint,
             CORBA::Environment &_env);
  // To do its job this class requires to know the local EC it will
  // connect to; it also requires to build an RT_Info for the local
  // scheduler.
  // It only keeps a copy of its SupplierProxy, used for later
  // connection and disconnections.
  // @@ TODO part of the RT_Info is hardcoded, we need to make it
  // parametric.

  int mtu (CORBA::ULong mtu);
  CORBA::ULong mtu (void) const;
  // The sender may need to fragment the message, otherwise the
  // network may drop the packets.
  // Setting the MTU can fail if the value is too small (at least the
  // header + 8 bytes must fit).

  void shutdown (CORBA::Environment&);
  // Disconnect and shutdown the sender, no further connections will
  // work unless init() is called again.

  void open (RtecEventChannelAdmin::ConsumerQOS& sub,
             CORBA::Environment& env);
  // Connect (or reconnect) to the EC with the given subscriptions.

  void close (CORBA::Environment& _env);
  // Disconnect from the EC, but reconnection is still possible.

  virtual void disconnect_push_consumer (CORBA::Environment &);
  virtual void push (const RtecEventComm::EventSet &events,
                     CORBA::Environment &);
  // The PushConsumer methods.

private:
  ACE_SOCK_Dgram& dgram (void);
  // Return the datagram...

  void send_fragment (const RtecUDPAdmin::UDP_Addr& udp_addr,
                      CORBA::ULong request_id,
                      CORBA::ULong request_size,
                      CORBA::ULong fragment_size,
                      CORBA::ULong fragment_offset,
                      CORBA::ULong fragment_id,
                      CORBA::ULong fragment_count,
                      iovec iov[],
                      int iovcnt,
                      CORBA::Environment& _env);
  // Send one fragment, the first entry in the iovec is used to send
  // the header, the rest of the iovec array should contain pointers
  // to the actual data.

  CORBA::ULong compute_fragment_count (const ACE_Message_Block* begin,
                                       const ACE_Message_Block* end,
                                       int iov_size,
                                       CORBA::ULong max_fragment_payload,
                                       CORBA::ULong& total_length);
  // Count the number of fragments that will be required to send the
  // message blocks in the range [begin,end)
  // The maximum fragment payload (i.e. the size without the header is
  // also required); <total_length> returns the total message size.

private:
  RtecEventChannelAdmin::EventChannel_var lcl_ec_;
  // The remote and the local EC, so we can reconnect when the
  // subscription list changes.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC (as a consumer) using this proxy.

  RtecUDPAdmin::AddrServer_var addr_server_;
  // We query this object to determine where are the events sent.

  TAO_ECG_UDP_Out_Endpoint *endpoint_;
  // The datagram used to sendto(), this object is *not* owned by the
  // UDP_Sender.

  CORBA::ULong mtu_;
  // The MTU for this sender...
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_ECG_UDP_Out_Endpoint
{
  //
  // = TITLE
  //   Maintains information about an outgoing endpoint.
  //
  // = DESCRIPTION
  //   UDP senders can share a single endpoint to send UDP packets,
  //   but there is more state associated with this endpoint than its
  //   datagram SAP; for instance we need to keep track of the request
  //   id.
public:
  TAO_ECG_UDP_Out_Endpoint (void);
  // Constructor

  ~TAO_ECG_UDP_Out_Endpoint (void);
  // Constructor

  ACE_SOCK_Dgram& dgram (void);
  // Obtain the datagram associated with this endpoint. Clients of
  // this class must open, and register (if necessary) this datagram.

  CORBA::ULong next_request_id (void);
  // Obtain the next request id.

  CORBA::Boolean is_loopback (const ACE_INET_Addr& from);
  // The endpoint can detect if a data-gram was sent by itself, this
  // is useful to ignore or remove messages sent by the same process.

private:
  ACE_Atomic_Op<ACE_SYNCH_MUTEX,CORBA::ULong> request_id_generator_;
  // The request id....

  ACE_SOCK_Dgram dgram_;
  // The datagram....

  u_short port_number_;
  // cache the port-number so we can quickly determine if an event is
  // coming from another endpoint.

  size_t if_count_;
  ACE_INET_Addr* ifs_;
  // Keep the list of local interfaces, needed for the is_loopback
  // method.
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_ECG_UDP_Request_Index
{
  // = TITLE
  //   Index to the request map.
  //
  // = DESCRIPTION
  //   This object is used to index the map of incomplete (due to
  //   fragmentation) requests.
  //
public:
  TAO_ECG_UDP_Request_Index (void);
  TAO_ECG_UDP_Request_Index (const ACE_INET_Addr& from,
                             CORBA::ULong request_id);
  // default copy ctor, dtor and operator=

  ACE_INLINE u_long hash (void) const;
  // Return a hash value...

  ACE_INLINE int operator== (const TAO_ECG_UDP_Request_Index& rhs) const;
  ACE_INLINE int operator!= (const TAO_ECG_UDP_Request_Index& rhs) const;
  // Compare

  ACE_INET_Addr from;
  CORBA::ULong request_id;
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_ECG_UDP_Request_Entry
{
  // = TITLE
  //   Keeps information about an incomplete request.
  //
  // = DESCRIPTION
  //   When a request arrives in fragments this object is used to
  //   keep track of the incoming data.
  //
public:
  enum {
    ECG_DEFAULT_FRAGMENT_BUFSIZ = 8
  };

  // TAO_ECG_UDP_Request_Entry (void);
  // TAO_ECG_UDP_Request_Entry (const TAO_ECG_UDP_Request_Entry& rhs);
  // TAO_ECG_UDP_Request_Entry& operator=(const TAO_ECG_UDP_Request_Entry& rhs);
  ~TAO_ECG_UDP_Request_Entry (void);

  TAO_ECG_UDP_Request_Entry (CORBA::Boolean byte_order,
                             CORBA::ULong request_id,
                             CORBA::ULong request_size,
                             CORBA::ULong fragment_count);
  // Initialize the fragment, allocating memory, etc.

  int validate_fragment (CORBA::Boolean byte_order,
                         CORBA::ULong request_size,
                         CORBA::ULong fragment_size,
                         CORBA::ULong fragment_offset,
                         CORBA::ULong fragment_id,
                         CORBA::ULong fragment_count) const;
  // Validate a fragment, it should be rejected if it is invalid..

  int test_received (CORBA::ULong fragment_id) const;
  // Has <fragment_id> been received?

  void mark_received (CORBA::ULong fragment_id);
  // Mark <fragment_id> as received, reset timeout counter...

  int complete (void) const;
  // Is the message complete?

  char* fragment_buffer (CORBA::ULong fragment_offset);
  // Return a buffer for the fragment at offset <fragment_offset>

  void decode (RtecEventComm::EventSet& event,
               CORBA::Environment& _env);
  // Decode the events, the message must be complete.

  void inc_timeout (void);
  // Increment the timeout counter...

  int get_timeout (void) const;
  // Get the timeout counter....

private:
  CORBA::Boolean byte_order_;
  CORBA::ULong request_id_;
  CORBA::ULong request_size_;
  CORBA::ULong fragment_count_;
  // This attributes should remain constant in all the fragments, used
  // for validation....

  CORBA::ULong timeout_counter_;
  ACE_Message_Block payload_;

  CORBA::ULong* received_fragments_;
  int own_received_fragments_;
  CORBA::ULong received_fragments_size_;
  CORBA::ULong default_received_fragments_[ECG_DEFAULT_FRAGMENT_BUFSIZ];
  // This is a bit vector, used to keep track of the received buffers.
};

// ****************************************************************

class TAO_ECG_UDP_Receiver;

class TAO_ORBSVCS_Export TAO_ECG_UDP_TH : public ACE_Event_Handler
{
  //
  // = TITLE
  //   Timer Handler for the UDP Receivers.
  //
  // = DESCRIPTION
  //   This object receives timer events from the reactor and forwards
  //   them to the UDP_Receiver; which uses those events to expire old
  //   messages that did not receive all their fragments.
  //
public:
  TAO_ECG_UDP_TH (TAO_ECG_UDP_Receiver *recv);

  // Reactor callbacks
  virtual int handle_timeout (const ACE_Time_Value& tv,
                              const void* act);

private:
  TAO_ECG_UDP_Receiver* receiver_;
  // We callback to this object when a message arrives.
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_ECG_UDP_Receiver : public POA_RtecEventComm::PushSupplier
{
  //
  // = TITLE
  //   Decodes events from an ACE_SOCK_Dgram and pushes them to the
  //   Event_Channel.
  //
  // = DESCRIPTION
  //   This supplier receives events from an ACE_SOCK_Dgram, either
  //   from a UDP socket or a Mcast group, decodes them and push them
  //   to the EC.
  //
  // = REASSEMBLY
  //
  //   Whenever an incomplete fragment is received (one with
  //   fragment_count > 1) we allocate an entry for the message in an
  //   map indexed by (host,port,request_id).  The entry contains the
  //   buffer, a bit vector to keep track of the fragments received
  //   so far, and a timeout counter.  This timeout counter is set to
  //   0 on each (new) fragment arrival, and incremented on a regular
  //   basis.  If the counter reaches a maximum value the message is
  //   dropped.
  //   Once all the fragments have been received the message is sent
  //   up, and the memory reclaimed. The entry is *not* removed until
  //   the timer expires (to handle re-transmitions).
  //
public:
  TAO_ECG_UDP_Receiver (void);

  void init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
             TAO_ECG_UDP_Out_Endpoint* ignore_from,
             RtecUDPAdmin::AddrServer_ptr addr_server,
             ACE_Reactor *reactor,
             const ACE_Time_Value &expire_interval,
             int max_timeout,
             CORBA::Environment &_env);
  // To do its job this class requires to know the local EC it will
  // connect to; it also requires to build an RT_Info for the local
  // scheduler.
  // The <reactor> is used to receive timeout events..
  // The <ignore_from> endpoint is used to remove events generated by
  // the same process.
  // @@ TODO part of the RT_Info is hardcoded, we need to make it
  // parametric.

  void shutdown (CORBA::Environment&);
  // Disconnect and shutdown the gateway, no further connectsions

  void open (RtecEventChannelAdmin::SupplierQOS& pub,
             CORBA::Environment &env);
  // Connect to the EC using the given publications lists.

  virtual void close (CORBA::Environment& env);
  // Disconnect to the EC.

  int handle_input (ACE_SOCK_Dgram& dgram);
  // The Event_Handlers call this method when data is available at the
  // socket, the <dgram> must be ready for reading and contain a full
  // event.

  int handle_timeout (const ACE_Time_Value& tv,
                      const void* act);
  // The timer has expired, must update all the unreceived
  // messages...

  // The PushSupplier method.
  virtual void disconnect_push_supplier (CORBA::Environment &);


  void get_addr (const RtecEventComm::EventHeader& header,
                 RtecUDPAdmin::UDP_Addr_out addr,
                 CORBA::Environment& env);
  // Call the RtecUDPAdmin::AddrServer

private:
  RtecEventChannelAdmin::EventChannel_var lcl_ec_;
  // The remote and the local EC, so we can reconnect when the list changes.

 RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // We talk to the EC (as a consumer) using this proxy.

  TAO_ECG_UDP_Out_Endpoint* ignore_from_;
  // Ignore any events coming from this IP addres.

  RtecUDPAdmin::AddrServer_var addr_server_;
  // The server used to map event types into multicast groups.

  typedef ACE_Hash_Map_Manager<TAO_ECG_UDP_Request_Index,
                               TAO_ECG_UDP_Request_Entry*,
                               ACE_SYNCH_MUTEX> Request_Map;
  typedef ACE_Hash_Map_Entry<TAO_ECG_UDP_Request_Index,
                             TAO_ECG_UDP_Request_Entry*> Request_Map_Entry;

  Request_Map request_map_;
  // The map containing all the incoming requests which have been
  // partially received.

  TAO_ECG_UDP_TH timeout_handler_;
  // To receive the timeouts..

  ACE_Reactor* reactor_;
  // The reactor we are using for the timeout handler...

  int max_timeout_;
  // How many timeouts before we expire a message...
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_ECG_UDP_EH : public ACE_Event_Handler
{
  //
  // = TITLE
  //   Event Handler for UDP messages.
  //
  // = DESCRIPTION
  //   This object receives callbacks from the Reactor when data is
  //   available on a UDP socket, it forwards to the ECG_UDP_Receiver
  //   which reads the events and transform it into an event.
  //
public:
  TAO_ECG_UDP_EH (TAO_ECG_UDP_Receiver *recv);

  int open (const ACE_INET_Addr& ipaddr);
  // Open the datagram and register with this->reactor()

  int close (void);
  // Close the datagram and unregister with the reactor.

  // Reactor callbacks
  virtual int handle_input (ACE_HANDLE fd);
  virtual ACE_HANDLE get_handle (void) const;

private:
  ACE_SOCK_Dgram dgram_;
  // The datagram used to receive the data.

  TAO_ECG_UDP_Receiver* receiver_;
  // We callback to this object when a message arrives.
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_ECG_Mcast_EH : public ACE_Event_Handler
{
  //
  // = TITLE
  //   Event Handler for UDP messages.
  //
  // = DESCRIPTION
  //   This object receives callbacks from the Reactor when data is
  //   available on the mcast socket, it forwards to the UDP_Receive
  //   gateway which reads the events and transform it into an event.
  //
public:
  TAO_ECG_Mcast_EH (TAO_ECG_UDP_Receiver *recv);

  int open (RtecEventChannelAdmin::EventChannel_ptr ec,
            CORBA::Environment& _env);
  // Register for changes in the EC subscription list.
  // When the subscription list becomes non-empty we join the proper
  // multicast groups (using the receiver to translate between event
  // types and mcast groups) and the class registers itself with the
  // reactor.

  int close (CORBA::Environment& _env);
  // Remove ourselves from the event channel, unsubscribe from the
  // multicast groups, close the sockets and unsubscribe from the
  // reactor.

  virtual int handle_input (ACE_HANDLE fd);
  virtual ACE_HANDLE get_handle (void) const;
  // Reactor callbacks

  void update_consumer (const RtecEventChannelAdmin::ConsumerQOS& sub,
                        CORBA::Environment& _env);
  void update_supplier (const RtecEventChannelAdmin::SupplierQOS& pub,
                        CORBA::Environment& _env);
  // The Observer methods

  class Observer : public POA_RtecEventChannelAdmin::Observer
  {
    // = TITLE
    //   Observe changes in the EC subscriptions.
    //
    // = DESCRIPTION
    //   As the subscriptions on the EC change we also change the
    //   mcast groups that we join.
    //   We could use the TIE classes, but they don't work in all
    //   compilers.
  public:
    Observer (TAO_ECG_Mcast_EH* eh);
    // We report changes in the EC subscriptions to the event
    // handler.

    // The Observer methods
    virtual void update_consumer (const RtecEventChannelAdmin::ConsumerQOS& sub,
                                  CORBA::Environment& _env);
    virtual void update_supplier (const RtecEventChannelAdmin::SupplierQOS& pub,
                                  CORBA::Environment& _env);

  private:
    TAO_ECG_Mcast_EH* eh_;
    // Our callback object.
  };

private:
  int subscribe (const ACE_INET_Addr &mcast_addr);
  int unsubscribe (const ACE_INET_Addr &mcast_addr);
  // Control the multicast group subscriptions

private:
  ACE_SOCK_Dgram_Mcast dgram_;
  // The datagram used to receive the data.

  TAO_ECG_UDP_Receiver* receiver_;
  // We callback to this object when a message arrives.

  Observer observer_;
  // This object will call us back when the subscription list
  // changes.

  RtecEventChannelAdmin::Observer_Handle handle_;
  // Keep the handle of the observer so we can unregister later.

  RtecEventChannelAdmin::EventChannel_var ec_;
  // The Event Channel.
};

#if defined(__ACE_INLINE__)
#include "EC_Gateway_UDP.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_EVENT_CHANNEL_UDP_H */
