/* -*- C++ -*- */
// $Id$

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

#ifndef EVENT_LATENCY_H
#define EVENT_LATENCY_H

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"

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
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  RtecEventChannelAdmin::ProxyPushSupplier_var suppliers_;

private:
  RtecScheduler::handle_t rt_info_;

  int measure_jitter_;

  ACE_Time_Value min_latency_;
  ACE_Time_Value max_latency_;
  ACE_Time_Value total_latency_;
  u_long total_pushes_;
  // Registers and counters for keeping track of latency statistics.

  ACE_Time_Value min_to_ec_;
  ACE_Time_Value max_to_ec_;
  ACE_Time_Value sum_to_ec_;
  // Statitics on time to get to the EC.

  ACE_Time_Value min_in_ec_;
  ACE_Time_Value max_in_ec_;
  ACE_Time_Value sum_in_ec_;
  // Statitics on time spent in the EC.

  ACE_Time_Value min_from_ec_;
  ACE_Time_Value max_from_ec_;
  ACE_Time_Value sum_from_ec_;
  // Statitics on time spent since the EC put the event on the wire
  // and it gets here.

  ACE_CString entry_point_;
};

// ************************************************************

class Latency_Supplier
// = TITLE
//    Latency Supplier
//
// = DESCRIPTION
//    Generates event nofications and a shutdown message.
{
public:

  //
  // This class provides IS-A Consumer and Supplier of events. But
  // inheritance from two skeleton classes is non-complaint (or at
  // least won't work with TAO). We use smaller implementation classes
  // that delegate on Latency_Supplier to do the job.
  //
  class Supplier : public POA_RtecEventComm::PushSupplier {
  public:
    virtual void disconnect_push_supplier (CORBA::Environment &);
    // The channel is disconnecting.

  private:
    Supplier (Latency_Supplier* impl);
    friend class Latency_Supplier;

  private:
    Latency_Supplier* impl_;
  };

  class Consumer : public POA_RtecEventComm::PushConsumer {
  public:
    virtual void push (const RtecEventComm::EventSet &events,
                       CORBA::Environment &);
    // The channel pushed some events to us.

    virtual void disconnect_push_consumer (CORBA::Environment &);
    // The channel is disconnecting.

  private:
    Consumer (Latency_Supplier* impl);
    friend class Latency_Supplier;

  private:
    Latency_Supplier* impl_;
  };

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

  void disconnect_push_supplier (CORBA::Environment &);
  // The channel is disconnecting.

  void disconnect_push_consumer (CORBA::Environment &);
  // The channel is disconnecting.

  void push (const RtecEventComm::EventSet &events,
             CORBA::Environment &);
  // Takes a timestamp and then pushes event_ to all consumers, either
  // directly, or via a channel.

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

  Supplier supplier_;
  Consumer consumer_;
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

#endif /* EVENT_LATENCY_H */
