/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   This test attempts to communicate several Event Channels UDP
//   using multicast.
//   The test reads a configuration file that describe what events are
//   received by each "VirtualConsumer".   The user must provide, on the
//   command line, which virtual consumers are present on each process.
//   The test also creates one supplier for each consumer, the
//   supplier can send an event of any possible type described in the
//   file.

// = HOW
//   The test creates a local consumer for each remote consumer, this
//   is necessary to send the event with the right port number; it
//   then sends the event using multicast.
//   Notice that there is still a win in using multicast because
//   multiple copies of the virtual consumer may be available.
//   To receive the event the test creates one local supplier for each
//   local "Virtual Consumer".
//
// = TODO
//   The class names in this test are *way* too artificial, I should
//   use the RTI names.
//
//   It is unfortunate that the test must know before-hand the remote
//   consumer interests.  It would be really simple to use a better
//   strategy: the test could "observe" changes in the remote EC
//   subscription list, it could then modify its local consumers
//   subscriptions.
//   Similarly the suppliers that supply Mcast packets as local events
//   could observe the changes in the local subscriptions and use that
//   to join or leave the multicast groups.
//   To demostrate this the test will need to reconfigure its
//   subscription list every so often (a few seconds seems like a good
//   idea).
//
// ============================================================================

#if !defined (EC_MCAST_H)
#define EC_MCAST_H

#include "ace/SString.h"
#include "ace/High_Res_Timer.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Channel_Clients_T.h"
#include "orbsvcs/Event/EC_Gateway_UDP.h"
#include "orbsvcs/Event/EC_UDP_Admin.h"

class ECM_Driver;

class ECM_Federation
{
  // = DESCRIPTION
  //   The test reads a configuration file where it obtains the data
  //   about each "federation". A federation is some application,
  //   distributed over several processes.  The potential set of
  //   publications and the potential set of subscriptions is known
  //   beforehand, but the actual publications (or subscriptions) may
  //   change dynamically.
  //   As stated above the federation may be present in more than one
  //   process, but also a process may participate in more than one
  //   federation.
  //
public:
  ECM_Federation (char* name,
		  CORBA::UShort mcast_port,
		  int supplier_types,
		  char** supplier_names,
		  int consumer_types,
		  char** consumer_names);
  // Constructor, it assumes ownership of the buffers, strings must be
  // allocated using CORBA::string_alloc(), buffers using operator new.

  ~ECM_Federation (void);
  // Dtor

  const char* name (void) const;
  // The name of the federation....

  CORBA::UShort mcast_port (void) const;
  // The port used by this federation to receive mcast messages.

  int supplier_types (void) const;
  // The number of different event types published by this federation.

  const char* supplier_name (CORBA::ULong i) const;
  // The name (mcast addr in A.B.C.D format) of the event type <i>

  CORBA::ULong supplier_ipaddr (CORBA::ULong i) const;
  // The ipaddr (in host byte order) of the event type <i>

  int consumer_types (void) const;
  // The number of different event types consumed by this federation.

  const char* consumer_name (CORBA::ULong i) const;
  // The name (mcast addr in A.B.C.D format) of the event type <i>

  CORBA::ULong consumer_ipaddr (CORBA::ULong i) const;
  // The ipaddr (in host byte order) of the event type <i>

  void open (ACE_SOCK_Dgram *dgram,
	     RtecEventChannelAdmin::EventChannel_ptr ec,
	     RtecScheduler::Scheduler_ptr scheduler,
	     CORBA::Environment &_env);
  // Connect the UDP sender to the EC.

  void close (CORBA::Environment &_env);
  // Close the UDP sender, disconnect from the EC

  int sender_local_addr (ACE_INET_Addr& addr);
  // Return the sender local address

private:
  char* name_;
  CORBA::UShort mcast_port_;

  int supplier_types_;
  char** supplier_names_;
  CORBA::ULong* supplier_ipaddr_;

  int consumer_types_;
  char** consumer_names_;
  CORBA::ULong* consumer_ipaddr_;

  TAO_ECG_UDP_Sender sender_;
  // The sender

  TAO_EC_Simple_AddrServer addr_server_;
  // Resolve event headers (type,source) to UDP addresses
  // (ipaddr,port)
};

class ECM_Local_Federation;

class ECM_Supplier : public POA_RtecEventComm::PushSupplier
{
  //
  // = TITLE
  //   Helper class to simulate an application acting as an event
  //   supplier.
  //
  // = DESCRIPTION
  //   This class connects as a consumer for timeouts in the EC. On
  //   every timeout it delegates on the ECM_Local_Federation class,
  //   usually this results in some reconfiguration and/or some events
  //   sent.
  //
public:
  ECM_Supplier (ECM_Local_Federation* federation);

  void open (const char* name,
	     RtecScheduler::Period period,
	     RtecEventChannelAdmin::EventChannel_ptr event_channel,
	     RtecScheduler::Scheduler_ptr scheduler,
	     CORBA::Environment& _env);
  // This method connects the supplier to the EC.

  void close (CORBA::Environment &_env);
  // Disconnect from the EC.

  void activate (const char* name,
		 RtecScheduler::Period period,
		 RtecEventChannelAdmin::EventChannel_ptr event_channel,
		 RtecScheduler::Scheduler_ptr scheduler,
		 CORBA::Environment& _env);
  // Connect as a consumer to start receiving events.

  RtecEventComm::EventSourceID supplier_id (void) const;
  // The supplier ID.

  void push (const RtecEventComm::EventSet& events,
	     CORBA::Environment &_env);
  void disconnect_push_consumer (CORBA::Environment &);
  // Implement the callbacks for our consumer personality.

  // = The POA_RtecEventComm::PushSupplier methods.
  virtual void disconnect_push_supplier (CORBA::Environment &);

private:
  ECM_Local_Federation* federation_;
  // To callback the federation.

  RtecEventComm::EventSourceID supplier_id_;
  // We generate an id based on the name....

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_; 
  // We talk to the EC (as a supplier) using this proxy.

  ACE_PushConsumer_Adapter<ECM_Supplier> consumer_;
  // We also connect to the EC as a consumer so we can receive the
  // timeout events.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_; 
  // We talk to the EC (as a supplier) using this proxy.
};

class ECM_Consumer : public POA_RtecEventComm::PushConsumer
{
  //
  // = TITLE
  //   Helper class to simulate an application acting as an event
  //   consumer.
  //
  // = DESCRIPTION
  //   This class connects as an event consumer to the EC.  The events
  //   are actually handled by the ECM_Local_Federation.
public:
  ECM_Consumer (ECM_Local_Federation* federation);

  void open (const char* name,
	     RtecEventChannelAdmin::EventChannel_ptr event_channel,
	     RtecScheduler::Scheduler_ptr scheduler,
	     CORBA::Environment& _env);
  // This method connects the consumer to the EC.

  void close (CORBA::Environment &_env);
  // Disconnect from the EC.

  // = The POA_RtecEventComm::PushComsumer methods.
  virtual void push (const RtecEventComm::EventSet& events,
		     CORBA::Environment &_env);
  virtual void disconnect_push_consumer (CORBA::Environment &);

private:
  ECM_Local_Federation* federation_;
  // To callback.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC using this proxy.
};

class ECM_Local_Federation
{
  // = DESCRIPTION
  //   This class is used to represent a federation that is actually
  //   running in this process.
  // 
public:
  ECM_Local_Federation (ECM_Federation *federation,
			ECM_Driver *driver);
  // Constructor.

  void open (int event_count,
	     RtecScheduler::Period period,
	     RtecEventChannelAdmin::EventChannel_ptr event_channel,
	     RtecScheduler::Scheduler_ptr scheduler,
	     CORBA::Environment& _env);
  // Connect both the supplier and the consumer.

  void close (CORBA::Environment& _env);
  // Disconnect everybody from the EC

  void activate (RtecScheduler::Period period,
		 RtecEventChannelAdmin::EventChannel_ptr event_channel,
		 RtecScheduler::Scheduler_ptr scheduler,
		 CORBA::Environment& _env);
  // Activate the supplier

  void supplier_timeout (RtecEventComm::PushConsumer_ptr consumer,
			 CORBA::Environment& _env);
  // The supplier is ready to send a new event.

  void consumer_push (ACE_hrtime_t arrival,
		      const RtecEventComm::EventSet& event,
		      CORBA::Environment& _env);
  // The consumer just received an event.

  const ECM_Federation *federation (void) const;
  // The federation description.

  void open_receiver (RtecEventChannelAdmin::EventChannel_ptr ec,
		      RtecScheduler::Scheduler_ptr scheduler,
		      CORBA::Environment &_env);
  // Connect the UDP receiver to the EC.

  void close_receiver (CORBA::Environment &_env);
  // Close the UDP receiver, disconnect from the EC

  void dump_results (void) const;
  // Report the results back to the user...

  // = Delegate on the federation description
  const char* name (void) const;
  CORBA::UShort mcast_port (void) const;
  int supplier_types (void) const;
  const char* supplier_name (CORBA::ULong i) const;
  CORBA::ULong supplier_ipaddr (CORBA::ULong i) const;
  int consumer_types (void) const;
  const char* consumer_name (CORBA::ULong i) const;
  CORBA::ULong consumer_ipaddr (CORBA::ULong i) const;

private:
  ECM_Federation *federation_;
  // The description of the events we send and receive.

  ECM_Driver *driver_;
  // The test driver.

  ECM_Consumer consumer_;
  ECM_Supplier supplier_;
  // The supplier and consumer helper classes, other than
  // initialization this classes only forward events to the
  // Federation.

  // Collect statistics

  CORBA::ULong recv_count_;
  // Messages received.

  CORBA::ULong unfiltered_count_;
  // Messages received that were not properly filtered.

  CORBA::ULong invalid_count_;
  // Message received that could *not* be destined to this federation,
  // yet they were received.

  CORBA::ULong send_count_;
  // Messages sent.

  int event_count_;
  // How many messages will we send before stop the simulation.

  ACE_Time_Value last_publication_change_;
  // The last time we changed our publication list, we don't change it
  // too often.

  ACE_Time_Value last_subscription_change_;
  // The last time we changed our publication, so we don't change too
  // often.

  TAO_ECG_UDP_Receiver receiver_;
  // This object reads the events and pushes them into the EC. Notice
  // that it can receive events from multiple Event Handlers.

  TAO_ECG_Mcast_EH mcast_eh_;
  // The event handler, it receives callbacks from the reactor
  // whenever an event is available in some of the multicast groups,
  // it then forwards to the <mcast_recv_> object for processing and
  // dispatching of the event.
  // @@ TODO Eventually we may need several of this objects to handle
  // OS limitations on the number of multicast groups per socket.
};

class ECM_Driver
{
  //
  // = TITLE
  //   Demonstrate the use of the UDP Gateways.
  //
  // = DESCRIPTION
  //   This class is design to exercise several features of the UDP
  //   Gateways and its companion classes.
  //   We create a set of processes, each running one EC, with
  //   multiple consumers and suppliers colocated with the EC.
  //   The ECs communicate among themselves using multicast.
  //   The test thus show how to use multicast, change the local
  //   ECG_UDP_Receiver and ECG_UDP_Sender QoS specifications
  //   dynamically, how to economically use the OS resources to
  //   receive and send multicast messages, etc.
  //
public:
  ECM_Driver (void);

  enum {
    MAX_EVENTS = 1024,
    // Maximum number of events to send on each Federation.

    MAX_LOCAL_FEDERATIONS = 16,
    // Maximum number of federations running on a single process

    MAX_FEDERATIONS = 128
    // Maximum number of federations in the simulation
  };

  int run (int argc, char* argv[]);
  // Run the test, read all the configuration files, etc.

  void federation_has_shutdown (ECM_Local_Federation *federation,
				CORBA::Environment& _env);
  // One of the federations has completed its simulation, once all of
  // them finish the test exists.
			  

private:
  void open_federations (RtecEventChannelAdmin::EventChannel_ptr ec,
			 RtecScheduler::Scheduler_ptr scheduler,
			 CORBA::Environment &_env);
  // Connect the federations to the EC.

  void activate_federations (RtecEventChannelAdmin::EventChannel_ptr ec,
			     RtecScheduler::Scheduler_ptr scheduler,
			     CORBA::Environment &_env);
  // Activate all the federations

  void close_federations (CORBA::Environment &_env);
  // Close the federations, i.e. disconnect from the EC, deactivate
  // the objects, etc.

  void open_senders (RtecEventChannelAdmin::EventChannel_ptr ec,
		     RtecScheduler::Scheduler_ptr scheduler,
		     CORBA::Environment &_env);
  // Connect all the senders, so we can start multicasting events.

  void open_receivers (RtecEventChannelAdmin::EventChannel_ptr ec,
		       RtecScheduler::Scheduler_ptr scheduler,
		       CORBA::Environment &_env);
  // Connect all the receivers, thus we accept events arriving through
  // multicast.

  void close_senders (CORBA::Environment &_env);
  // Close all the senders to cleanup resources.

  void close_receivers (CORBA::Environment &_env);
  // Close all the receivers to cleanup resources.

  int shutdown (CORBA::Environment&);
  // Called when the main thread.

  int parse_args (int argc, char* argv[]);
  // parse the command line arguments

  int parse_config_file (void);
  // parse the command line arguments

  int parse_name_list (FILE* file, int n, char** names,
		       const char* error_msg);
  // parse one of the lists of names in the federation definition.

  int skip_blanks (FILE* file,
		   const char* error_msg);
  // skip the blanks in the file.

  void dump_results (void);
  // Dump the results to the standard output.
			  
private:
  char* lcl_name_;
  // The name of the "local" EC.

  int event_period_;
  // The events are generated using this interval.

  int event_count_;
  // How many events will the suppliers send

  char* config_filename_;
  // The name of the file where we read the configuration.

  const char* pid_filename_;
  // The name of a file where the process stores its pid

  int local_federations_count_;
  // How many federations are running in this process (or, if you
  // prefer, in how many federations does this process participate).

  ECM_Local_Federation* local_federations_[MAX_LOCAL_FEDERATIONS];
  // The local federations.

  char* local_names_[MAX_LOCAL_FEDERATIONS];
  // The names of the local federations.

  int all_federations_count_;
  // The total number of federations we belong to.

  ECM_Federation* all_federations_[MAX_FEDERATIONS]; 
  // All the federations.

  ACE_Atomic_Op<ACE_SYNCH_MUTEX,int> federations_running_;
  // Keep track of how many federations are active so we can shutdown
  // once they are all destroyed.

  ACE_hrtime_t test_start_;
  ACE_hrtime_t test_stop_;
  // Measure the test elapsed time as well as mark the beginning of
  // the frames.

  CORBA::ORB_var orb_;
  // The ORB, so we can shut it down.

  ACE_SOCK_Dgram send_dgram_;
  // This socket is shared by all the federations to send the
  // multicast events.
};

#if defined (__ACE_INLINE__)
#include "EC_Mcast.i"
#endif /* __ACE_INLINE__ */

#endif /* EC_MCAST_H */
