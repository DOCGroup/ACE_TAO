/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    ACE Event Service Benchmarks
// 
// = FILENAME
//    Latency.h
//
// = AUTHOR
//    David Levine (levine@cs.wustl.edu) and
//    Tim Harrison (harrison@cs.wustl.edu)
//
// = DESCRIPTION
//
// ============================================================================

#if !defined (LATENCY_H)
#define LATENCY_H

#include "ace/SString.h"
#include "RtecEventChannelAdminC.h"
#include "RtecEventCommS.h"

class Latency_Consumer : public POA_RtecEventComm::PushConsumer
// = TITLE
//    Latency Consumer
//
// = DESCRIPTION
//    Simple example of a consumer that registers for supplier
//    notifications.
{
public:
  Latency_Consumer (const int measure_jitter = 0);
  // Construction.  The default of not measuring jitter provides
  // a "null" push routine, for measuring aggregate timing.

  int open_consumer (RtecEventChannelAdmin::EventChannel_ptr ec,
		     const char *my_name);
  // Uses the name server to obtain a reference to the <supplier_name>
  // and registers with channel to receive notifications from the
  // supplier.  Also registers to receive shutdown messages from the
  // supplier.  Stores <my_name> for printing out messages.  Returns 0
  // on success, -1 on failure.

  virtual void disconnect_push_consumer (CORBA::Environment &);
  // The channel is disconnecting.

  void entry_point (const char*);
  const char *entry_point () const;

  void print_stats () /* const */;
  // Print timing statistics.

// (not protected to allow short-circuiting) protected:
  virtual void push (const RtecEventComm::EventSet &events,
		     CORBA::Environment &);
  // If the <events>[0] is a notification, prints out the data from
  // the supplier.  If its a shutdown message, the consumer
  // disconnects from the channel.

protected:
  void shutdown (void);
  // Disconnect from the Event Service.

  // = Event channel adminstration references.
  RtecEventChannelAdmin::EventChannel_var channel_admin_;
  RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin_;
  RtecEventChannelAdmin::ProxyPushSupplier_ptr suppliers_;

private:
  RtecScheduler::handle_t rt_info_;

  int measure_jitter_;

  ACE_Time_Value min_latency_;
  ACE_Time_Value max_latency_;
  ACE_Time_Value total_latency_;
  u_long total_pushes_;
  // Registers and counters for keeping track of latency statistics.

  ACE_CString entry_point_;
};

// ************************************************************

class Latency_Supplier : public POA_RtecEventComm::PushSupplier, public POA_RtecEventComm::PushConsumer
// = TITLE
//    Latency Supplier
//
// = DESCRIPTION
//    Generates event nofications and a shutdown message.
{
public:
  Latency_Supplier (const u_int total_messages,
		    CORBA::Long supplier_id,
                    const int timestamp = 0);
  // Construction.  Requires the total number of messages to be
  // sent.  If the timestamp flag is enabled, then events are
  // timestamped, e.g., for use in measuring jitter.
  
  int open_supplier (RtecEventChannelAdmin::EventChannel_ptr event_channel,
		     const char *name, int master);
  // Registers with the name server under the given <name>.  Also
  // connects to the Event Channel as a supplier of notifications and
  // shutdown messages.  If <master> != 0, then the supplier will
  // destroy the channel upon shutting down. Returns 0 on success, -1
  // on failure.

  virtual void disconnect_push_supplier (CORBA::Environment &);
  // The channel is disconnecting.

  virtual void disconnect_push_consumer (CORBA::Environment &);
  // The channel is disconnecting.

  int start_generating_events (void);
  // Called when the supplier should start generating events.
  // Registers with the Event Channel to receive timeouts every .25
  // seconds.  Will generate some number of events and then send a
  // shutdown message.  Returns 0 on success, -1 on failure.

  void entry_point (const char *);
  const char *entry_point () const;

  void print_stats () /* const */;
  // Print timing statistics.

protected:
  void push (const RtecEventComm::EventSet &events,
	     CORBA::Environment &);
  // Takes a timestamp and then pushes event_ to all consumers, either
  // directly, or via a channel.

  void shutdown (void);
  // Disconnect from the Event Service.

  // = Event Channel adminstration references.
  RtecEventChannelAdmin::EventChannel_var channel_admin_;
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  RtecEventChannelAdmin::ProxyPushConsumer_var consumers_;
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_;
  RtecEventChannelAdmin::ProxyPushSupplier_var suppliers_;

private:
  RtecScheduler::handle_t rt_info_;

  u_int total_messages_;
  // How many events to push before ending the test.

  CORBA::Long supplier_id_;
  // Our supplier ID.

  int timestamp_;
  // Flag indicating whether or not to timestamp outgoing events.

  u_int total_sent_;
  // How many events we've pushed.

  ACE_Time_Value test_start_time_;
  ACE_Time_Value test_stop_time_;
  // Start/stop times, marking the time period when events are
  // sent to consumers.

  int master_;

  ACE_CString entry_point_;
};

void Latency_Consumer::entry_point(const char* s)
{
  entry_point_ = s;
}

const char* Latency_Consumer::entry_point (void) const
{
  return entry_point_.fast_rep ();
}

void Latency_Supplier::entry_point(const char* s)
{
  entry_point_ = s;
}

const char* Latency_Supplier::entry_point (void) const
{
  return entry_point_.fast_rep ();
}

#endif /* LATENCY_H */
