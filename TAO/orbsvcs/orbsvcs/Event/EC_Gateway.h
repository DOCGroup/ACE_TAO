/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   EC_Gateway
//
// = AUTHOR
//   Carlos O'Ryan
//
// = DESCRIPTION
//   This class can be used to connect two event channels; the class
//   connects to a "remote" EC as a consumer, it also connects to the
//   <local> EC as a supplier of events, this later EC is usually
//   collocated.
//   The QoS parameters for both connections must be provided by the
//   user.
//   To avoid infinite loops of events the Gateway descreases the TTL
//   field of the events and will not deliver any events with TTL less
//   than or equal to 0.
//
// = TODO
//   The class makes an extra copy of the events, we need to
//   investigate if closer collaboration with its collocated EC could
//   be used to remove that copy.
//
// ============================================================================

#ifndef TAO_EC_GATEWAY_H
#define TAO_EC_GATEWAY_H

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Channel_Clients_T.h"

class TAO_Export TAO_EC_Gateway
//
// = TITLE
//   Event Channel Gateway.
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
//
{
public:
  TAO_EC_Gateway (void);
  ~TAO_EC_Gateway (void);

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

  int shutdown (CORBA::Environment&);

private:
  ACE_PushConsumer_Adapter<TAO_EC_Gateway> consumer_;
  // Our consumer personality....

  ACE_PushSupplier_Adapter<TAO_EC_Gateway> supplier_;
  // Our supplier personality....

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // We talk to the EC (as a supplier) using this proxy.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC (as a consumer) using this proxy.
};

#endif /* ACE_EVENT_CHANNEL_H */
