/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   This test attempts to communicate several Event Channels.
//   The test hardcodes all the objects involved (consumers,
//   suppliers, proxies, etc.); the objective is to gain understanding
//   on the architecture needed to exploit locality in the Event
//   cycle, not to provide a definite solution.
//
// ============================================================================

#if !defined (EC_MULTIPLE_H)
#define EC_MULTIPLE_H

#include "ace/SString.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Channel_Clients_T.h"


class EC_Proxy
// = TITLE
//   Event Channel Proxy.
//
// = DESCRIPTION
//   This class mediates among two event channels, it connects as a
//   consumer of events with a remote event channel, and as a supplier
//   of events with the local EC.
//   As a consumer it gives a QoS designed to only accept the events
//   in which *local* consumers are interested.
//   Eventually the local EC should create this object and compute its
//   QoS in an automated manner; but this requires some way to filter
//   out the peers registered as consumers, otherwise we will get
//   loops in the QoS graph.
//   It uses exactly the same set of events in the publications list
//   when connected as a supplier.
//
// = NOTES
//   An alternative implementation would be to register with the
//   remote EC as a supplier, and then filter on the remote EC, but
//   one of the objectives is to minimize network traffic.
//   On the other hand the events will be pushed to remote consumers,
//   event though they will be dropped upon receipt (due to the TTL
//   field); IMHO this is another suggestion that the EC needs to know
//   (somehow) which consumers are truly its peers in disguise.
//
// = ALTERNATIVES
//   Check http://www.cs.wustl.edu/~coryan/Multiple_EC.html for a
//   discussion on that topic.
//
{
public:
  EC_Proxy (void);
  ~EC_Proxy (void);

  int open (RtecEventChannelAdmin::EventChannel_ptr remote_ec,
	    RtecEventChannelAdmin::EventChannel_ptr local_ec,
	    const RtecEventChannelAdmin::ConsumerQOS& subscriptions,
	    const RtecEventChannelAdmin::SupplierQOS& publications,
	    CORBA::Environment &_env);
  // Establish the connections.
  
  void disconnect_push_supplier (CORBA::Environment &);
  // The channel is disconnecting.

  void disconnect_push_consumer (CORBA::Environment &);
  // The channel is disconnecting.

  void push (const RtecEventComm::EventSet &events,
	     CORBA::Environment &);
  // This is the Consumer side behavior, it pushes the events to the
  // local event channel.

private:
  ACE_PushConsumer_Adapter<EC_Proxy> consumer_;
  ACE_PushSupplier_Adapter<EC_Proxy> supplier_;

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
};

class Test_ECP
//
// = TITLE
//   A simple test for the EC_Proxy class.
//
// = DESCRIPTION
//   This class is design to exercise several features of the EC_Proxy
//   class and the multiple EC architecture.
//   We want to create two EC, each one having a single supplier and a
//   single consumer.
//    + To test the remote facilities the consumer register for both a
//    local event and a remote one.
//    + To test the remote filtering features the remote consumer only
//    wants one of the local events, and this event is generated less
//    frequently.
//
//   This class creates the local ECP a consumer and a supplier, it
//   uses the command line to figure the 
//
{
public:
  Test_ECP (void);

  int run (int argc, char* argv[]);
  // Execute the test.

  void disconnect_push_supplier (CORBA::Environment &);
  void disconnect_push_consumer (CORBA::Environment &);
  void push (const RtecEventComm::EventSet &events,
	     CORBA::Environment &);
  // Implement the consumer and supplier upcalls.


private:
  int parse_args (int argc, char* argv[]);

  RtecEventChannelAdmin::EventChannel_ptr
    get_ec (CosNaming::NamingContext_ptr naming_context,
	    const char* ec_name,
	    CORBA::Environment &_env);

  int connect_supplier (RtecEventChannelAdmin::EventChannel_ptr local_ec,
			CORBA::Environment &_env);
  int connect_consumer (RtecEventChannelAdmin::EventChannel_ptr local_ec,
			CORBA::Environment &_env);
  int connect_ecp (RtecEventChannelAdmin::EventChannel_ptr local_ec,
		   RtecEventChannelAdmin::EventChannel_ptr remote_ec,
		   CORBA::Environment &_env);

private:
  ACE_PushConsumer_Adapter<Test_ECP> consumer_;
  ACE_PushSupplier_Adapter<Test_ECP> supplier_;

  EC_Proxy ecp_;

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  RtecEventComm::EventSourceID supplier_id_;

  char* rmt_ec_name_;
  char* lcl_ec_name_;
  
  int event_a_;
  int event_b_;
  int event_c_;

  int interval_;
};


#endif /* EC_MULTIPLE_H */
