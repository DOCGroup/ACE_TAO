// -*- C++ -*-

/// @file EC_Mcast.h
/// Test communication between UDP Event Channels using multicast.
///
/// The test reads a configuration file describing the events received by
/// each federation. Each process is given its local federations on the
/// command line and creates one supplier for each federation.
///
/// @par How it works
/// A UDP sender for each remote federation sends events by multicast as a
/// PushConsumer. A UDP receiver for each local federation joins the relevant
/// multicast groups and pushes received events as a PushSupplier. Multicast
/// avoids sending multiple copies when a federation has multiple processes.
///
/// Local subscription changes could be used to join or leave multicast
/// groups. Demonstrating this would require periodically reconfiguring the
/// subscription list.
///
/// @todo The test currently needs advance knowledge of remote consumer
/// interests. It could instead observe changes to remote Event Channel
/// subscriptions and update its local consumer subscriptions.

#ifndef EC_MCAST_H
#define EC_MCAST_H

#include <array>
#include <cstddef>
#include "ace/SString.h"
#include <atomic>
#include <memory>
#include <string>
#include <vector>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/High_Res_Timer.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Channel_Clients_T.h"
#include "orbsvcs/Event/ECG_UDP_Sender.h"
#include "orbsvcs/Event/EC_UDP_Admin.h"
#include "orbsvcs/Event/ECG_Mcast_EH.h"
#include "orbsvcs/Event/ECG_UDP_Out_Endpoint.h"
#include "orbsvcs/Event/ECG_UDP_Receiver.h"
#include "orbsvcs/Event/ECG_UDP_Sender.h"

class ECM_Driver;

/// Configuration and addresses for a federation in the test.
/// A federation is an application distributed across processes. Its possible
/// publications and subscriptions are known in advance, though the active
/// publications and subscriptions may change dynamically. A federation may
/// run in multiple processes, and a process may participate in multiple
/// federations.
class ECM_Federation
{
public:
  ECM_Federation (std::string name,
                  CORBA::UShort mcast_port,
                  std::vector<std::string> supplier_names,
                  std::vector<std::string> consumer_names);
  ECM_Federation (ECM_Federation const&) = delete;
  ECM_Federation (ECM_Federation&&) = delete;
  ECM_Federation& operator= (ECM_Federation const&) = delete;
  ECM_Federation& operator= (ECM_Federation&&) = delete;

  /// Returns the federation name.
  char const* name () const;

  /// Returns the multicast port used to receive messages.
  CORBA::UShort mcast_port () const;

  /// Returns the number of event types published by this federation.
  size_t supplier_types () const;

  /// Returns the multicast address name for supplier event type @a i.
  char const* supplier_name (size_t i) const;

  /// Returns the host-byte-order IP address for supplier event type @a i.
  CORBA::ULong supplier_ipaddr (size_t i) const;

  /// Returns the number of event types consumed by this federation.
  size_t consumer_types () const;

  /// Returns the multicast address name for consumer event type @a i.
  char const* consumer_name (size_t i) const;

  /// Returns the host-byte-order IP address for consumer event type @a i.
  CORBA::ULong consumer_ipaddr (size_t i) const;

  /// Connects the UDP sender to the Event Channel.
  void open (TAO_ECG_UDP_Out_Endpoint *endoint,
             RtecEventChannelAdmin::EventChannel_ptr ec);

  /// Closes the UDP sender and disconnects it from the Event Channel.
  void close ();

  /// Returns the sender's local address.
  int sender_local_addr (ACE_INET_Addr& addr);

  /// Returns the address server that maps event headers (type, source) to
  /// UDP addresses (IP address, port).
  RtecUDPAdmin::AddrServer_ptr addr_server ();

private:
  std::string name_;
  CORBA::UShort mcast_port_;

  std::vector<std::string> supplier_names_;
  std::vector<CORBA::ULong> supplier_ipaddr_;

  std::vector<std::string> consumer_names_;
  std::vector<CORBA::ULong> consumer_ipaddr_;

  /// Sender servant.
  PortableServer::Servant_var<TAO_ECG_UDP_Sender> sender_;

  /// Resolves event headers (type, source) to UDP addresses (IP address,
  /// port).
  TAO_EC_Simple_AddrServer addr_server_;
};

class ECM_Local_Federation;

/// Helper that simulates an application acting as an event supplier.
/// It receives Event Channel timeout events and delegates to
/// ECM_Local_Federation, which may reconfigure subscriptions or send events.
class ECM_Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  explicit ECM_Supplier (ECM_Local_Federation* federation);
  ECM_Supplier (ECM_Supplier const&) = delete;
  ECM_Supplier (ECM_Supplier&&) = delete;
  ECM_Supplier& operator= (ECM_Supplier const&) = delete;
  ECM_Supplier& operator= (ECM_Supplier&&) = delete;

  /// Connects the supplier to the Event Channel.
  void open (char const* name,
             RtecEventChannelAdmin::EventChannel_ptr event_channel);

  /// Disconnects from the Event Channel.
  void close ();

  /// Connects as a consumer to start receiving events.
  void activate (RtecEventChannelAdmin::EventChannel_ptr event_channel,
                 RtecEventComm::Time interval);

  /// Returns the supplier ID.
  RtecEventComm::EventSourceID supplier_id () const;

  /// Handles callbacks for the consumer role.
  void push (RtecEventComm::EventSet const& events);
  void disconnect_push_consumer ();

  /// Receives disconnect callbacks for the supplier role.
  void disconnect_push_supplier () override;

private:
  /// Borrowed; ECM_Driver owns the local federation.
  ECM_Local_Federation* federation_;

  /// ID generated from the supplier name.
  RtecEventComm::EventSourceID supplier_id_;

  /// Proxy used to supply events to the Event Channel.
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;

  /// Consumer adapter used to receive Event Channel timeout events.
  ACE_PushConsumer_Adapter<ECM_Supplier> consumer_;

  /// Proxy used to receive events from the Event Channel.
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
};

/// Helper that simulates an application acting as an event consumer.
/// The Event Channel delegates received events to ECM_Local_Federation.
class ECM_Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  explicit ECM_Consumer (ECM_Local_Federation* federation);
  ECM_Consumer (ECM_Consumer const&) = delete;
  ECM_Consumer (ECM_Consumer&&) = delete;
  ECM_Consumer& operator= (ECM_Consumer const&) = delete;
  ECM_Consumer& operator= (ECM_Consumer&&) = delete;

  /// Connects the consumer to the Event Channel.
  void open (char const* name,
             RtecEventChannelAdmin::EventChannel_ptr event_channel,
             unsigned int *seed);

  /// Disconnects from the Event Channel.
  void close ();

  void connect (unsigned int *seed);
  /// Disconnects from the supplier without closing or forgetting it.
  void disconnect ();

  /// Receives callbacks for the PushConsumer role.
  void push (RtecEventComm::EventSet const& events) override;
  void disconnect_push_consumer () override;

private:
  /// Borrowed; ECM_Driver owns the local federation.
  ECM_Local_Federation* federation_;

  /// Proxy used to receive events from the Event Channel.
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /// Consumer admin used to connect to the Event Channel.
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
};

/// Represents a federation running in this process.
class ECM_Local_Federation
{
public:
  ECM_Local_Federation (ECM_Federation *federation,
                        ECM_Driver *driver);
  ECM_Local_Federation (ECM_Local_Federation const&) = delete;
  ECM_Local_Federation (ECM_Local_Federation&&) = delete;
  ECM_Local_Federation& operator= (ECM_Local_Federation const&) = delete;
  ECM_Local_Federation& operator= (ECM_Local_Federation&&) = delete;
  ~ECM_Local_Federation () = default;

  /// Connects the supplier and consumer.
  void open (int event_count,
             RtecEventChannelAdmin::EventChannel_ptr event_channel);

  /// Disconnects the supplier and consumer from the Event Channel.
  void close ();

  /// Activates the supplier.
  void activate (RtecEventChannelAdmin::EventChannel_ptr event_channel,
                 RtecEventComm::Time interval);

  /// Called when the supplier is ready to send another event.
  void supplier_timeout (RtecEventComm::PushConsumer_ptr consumer);

  /// Called when the consumer receives an event.
  void consumer_push (ACE_hrtime_t arrival,
                      RtecEventComm::EventSet const& event);

  /// Returns the federation description.
  ECM_Federation const* federation () const;

  /// Connects the UDP receiver to the Event Channel.
  void open_receiver (RtecEventChannelAdmin::EventChannel_ptr ec,
                      TAO_ECG_Refcounted_Endpoint ignore_from);

  /// Closes the UDP receiver and disconnects it from the Event Channel.
  void close_receiver ();

  /// Reports the results to the user.
  void dump_results () const;

  /// Sets the subscription bit for event type @a i.
  void subscribed_bit (size_t i, CORBA::Boolean x);
  /// Returns the subscription bit for event type @a i.
  CORBA::Boolean subscribed_bit (size_t i) const;

  /// @name Accessors delegated to the federation description
  /// @{
  char const* name () const;
  CORBA::UShort mcast_port () const;
  size_t supplier_types () const;
  char const* supplier_name (size_t i) const;
  CORBA::ULong supplier_ipaddr (size_t i) const;
  size_t consumer_types () const;
  char const* consumer_name (size_t i) const;
  CORBA::ULong consumer_ipaddr (size_t i) const;
  /// @}

private:
  /// Borrowed; ECM_Driver owns the federation description.
  ECM_Federation* federation_;

  /// Borrowed; ECM_Driver owns this local federation.
  ECM_Driver *driver_;

  /// Supplier and consumer helpers that forward events to the federation.
  ECM_Consumer consumer_;
  ECM_Supplier supplier_;

  /// @name Collected statistics
  /// @{

  /// Number of messages received.
  CORBA::ULong recv_count_;

  /// Number of received messages that were not properly filtered.
  CORBA::ULong unfiltered_count_;

  /// Number of received messages not destined for this federation.
  CORBA::ULong invalid_count_;

  /// Number of messages sent.
  CORBA::ULong send_count_;
  /// @}

  /// Number of messages to send before stopping the simulation.
  int event_count_;

  /// Last time the publication changed, to avoid changing it too often.
  ACE_Time_Value last_subscription_change_;

  /// Reads events and pushes them into the Event Channel; it can receive
  /// events from multiple Event Handlers.
  PortableServer::Servant_var<TAO_ECG_UDP_Receiver> receiver_;

  /// Receives reactor callbacks when multicast events arrive and forwards
  /// them to the receiver for processing and dispatch.
  /// @todo More event handlers may be needed to handle operating-system
  /// limits on the number of multicast groups per socket.
  std::unique_ptr<TAO_ECG_Mcast_EH> mcast_eh_;

  /// Seed for the random number generator.
  unsigned int seed_;

  /// Average period between subscription changes, in microseconds.
  CORBA::ULong subscription_change_period_;

  /// Event types to which this federation is subscribed.
  std::vector<CORBA::Boolean> subscription_subset_;
};

/// Demonstrates the UDP Gateways and their companion classes.
/// The test runs several processes, each with an Event Channel and multiple
/// colocated consumers and suppliers. The Event Channels communicate using
/// multicast. The test exercises multicast, dynamic QoS changes for the UDP
/// receiver and sender, and efficient use of operating-system resources.
class ECM_Driver
{
public:
  ECM_Driver () = default;
  ~ECM_Driver () = default;
  ECM_Driver (ECM_Driver const&) = delete;
  ECM_Driver (ECM_Driver&&) = delete;
  ECM_Driver& operator= (ECM_Driver const&) = delete;
  ECM_Driver& operator= (ECM_Driver&&) = delete;

  enum {
    /// Maximum number of events to send on each federation.
    MAX_EVENTS = 1024,

    /// Maximum number of federations running on one process.
    MAX_LOCAL_FEDERATIONS = 16,

    /// Maximum number of federations in the simulation.
    MAX_FEDERATIONS = 128
  };

  /// Runs the test and reads its configuration files.
  int run (int argc, ACE_TCHAR* argv[]);

  /// Called when a federation completes; exits the test when all have
  /// finished.
  void federation_has_shutdown (ECM_Local_Federation *federation);

private:
  /// Connects the federations to the Event Channel.
  void open_federations (RtecEventChannelAdmin::EventChannel_ptr ec);

  /// Activates all federations.
  void activate_federations (RtecEventChannelAdmin::EventChannel_ptr ec);

  /// Disconnects federations from the Event Channel and deactivates them.
  void close_federations ();

  /// Connects all senders so they can start multicasting events.
  void open_senders (RtecEventChannelAdmin::EventChannel_ptr ec);

  /// Connects all receivers so they can accept multicast events.
  void open_receivers (RtecEventChannelAdmin::EventChannel_ptr ec);

  /// Closes all senders and cleans up their resources.
  void close_senders ();

  /// Closes all receivers and cleans up their resources.
  void close_receivers ();

  /// Parses command-line arguments.
  int parse_args (int argc, ACE_TCHAR* argv[]);

  /// Parses the configuration file.
  int parse_config_file ();

  /// Parses one list of names in the federation definition.
  int parse_name_list (FILE *file,
                       size_t count,
                       std::vector<std::string> &names,
                       char const *error_msg);

  /// Skips blank characters in the file.
  int skip_blanks (FILE* file,
                   char const* error_msg);

  /// Dumps results to standard output.
  void dump_results ();

private:
  /// Interval between generated events, in microseconds.
  int event_period_ {250000};

  /// Number of events suppliers send.
  int event_count_ {100};

  /// Name of the configuration file.
  ACE_TString config_filename_ {};

  /// Name of the file where the process stores its process ID.
  ACE_TString pid_filename_ {};

  /// ORB kept alive longer than the federations that use it.
  CORBA::ORB_var orb_ {};

  /// Socket shared by all federations to send multicast events.
  TAO_ECG_UDP_Out_Endpoint endpoint_;

  /// Total number of federations in which this process participates.
  size_t all_federations_count_ {};

  /// All federations outlive the local federations that borrow them.
  std::array<std::unique_ptr<ECM_Federation>, MAX_FEDERATIONS> all_federations_ {};

  /// Number of federations running in this process.
  size_t local_federations_count_ {};

  /// Local federations.
  std::array<std::unique_ptr<ECM_Local_Federation>, MAX_LOCAL_FEDERATIONS> local_federations_ {};

  /// Names of the local federations.
  std::array<std::string, MAX_LOCAL_FEDERATIONS> local_names_ {};

  /// Tracks active federations so the test can shut down after all finish.
  std::atomic<CORBA::ULong> federations_running_ {0};
};

#if defined (__ACE_INLINE__)
#include "EC_Mcast.inl"
#endif /* __ACE_INLINE__ */

#endif /* EC_MCAST_H */
