/* -*- C++ -*- */
// $Id$

/**
 * @file Event_Latency.h
 *
 * @author David Levine (levine@cs.wustl.edu)
 * @author Tim Harrison (harrison@cs.wustl.edu)
 */

#ifndef EVENT_LATENCY_H
#define EVENT_LATENCY_H

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// Latency Consumer
/**
 * Simple example of a consumer that registers for supplier
 * notifications.
 */
class Latency_Consumer
  : public POA_RtecEventComm::PushConsumer
  , public PortableServer::RefCountServantBase
{
public:
  /// Default constructor
  /**
   * Construction.  The default of not measuring jitter provides
   * a "null" push routine, for measuring aggregate timing.
   */
  Latency_Consumer (const int measure_jitter = 0);

  /**
   * Uses the name server to obtain a reference to the <supplier_name>
   * and registers with channel to receive notifications from the
   * supplier.  Also registers to receive shutdown messages from the
   * supplier.  Stores <my_name> for printing out messages.  Returns 0
   * on success, -1 on failure.
   */
  int open_consumer (RtecEventChannelAdmin::EventChannel_ptr ec,
                     const char *my_name);

  /// Disconnect from the Event Service and deactivate the object
  void close (CORBA::Environment &);

  void entry_point (const char*);
  const char *entry_point () const;

  /// Print timing statistics.
  void print_stats () /* const */;

  virtual void disconnect_push_consumer (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * If the <events>[0] is a notification, prints out the data from
   * the supplier.  If its a shutdown message, the consumer
   * disconnects from the channel.
   */
  virtual void push (const RtecEventComm::EventSet &events,
                     CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  void shutdown (void);

  //@{
  /// Event channel adminstration references.
  RtecEventChannelAdmin::EventChannel_var channel_admin_;
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  RtecEventChannelAdmin::ProxyPushSupplier_var suppliers_;
  //@}

private:
  RtecBase::handle_t rt_info_;

  int measure_jitter_;

  //@{
  /// Registers and counters for keeping track of latency statistics.
  ACE_Time_Value min_latency_;
  ACE_Time_Value max_latency_;
  ACE_Time_Value total_latency_;
  u_long total_pushes_;
  //@}

  //@{
  /// Statitics on time to get to the EC.
  ACE_Time_Value min_to_ec_;
  ACE_Time_Value max_to_ec_;
  ACE_Time_Value sum_to_ec_;
  //@}

  //@{
  /// Statitics on time spent in the EC.
  ACE_Time_Value min_in_ec_;
  ACE_Time_Value max_in_ec_;
  ACE_Time_Value sum_in_ec_;
  //@}

  //@{
  /// Statitics on time spent since the EC put the event on the wire
  /// and it gets here.
  ACE_Time_Value min_from_ec_;
  ACE_Time_Value max_from_ec_;
  ACE_Time_Value sum_from_ec_;
  //@}

  /// The name for the Scheduling service
  ACE_CString entry_point_;
};

// ************************************************************

/// Latency Supplier
/**
 * Generates event nofications and a shutdown message.
 */
class Latency_Supplier
{
public:

  /**
   * This class provides IS-A Consumer and Supplier of events. But
   * inheritance from two skeleton classes is non-complaint (or at
   * least won't work with TAO). We use smaller implementation classes
   * that delegate on Latency_Supplier to do the job.
   */
  class Supplier
    : public POA_RtecEventComm::PushSupplier
    , public PortableServer::RefCountServantBase
  {
  public:
    virtual void disconnect_push_supplier (CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    /// Constructor
    Supplier (Latency_Supplier* impl);

    /// Only the Latency_Supplier can create instances of this class
    friend class Latency_Supplier;

  private:
    Latency_Supplier* impl_;
  };

  class Consumer
    : public POA_RtecEventComm::PushConsumer
    , public PortableServer::RefCountServantBase
  {
  public:
    virtual void push (const RtecEventComm::EventSet &events,
                       CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void disconnect_push_consumer (CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    /// Constructor
    Consumer (Latency_Supplier* impl);

    /// Only the Latency_Supplier can create instances of this class
    friend class Latency_Supplier;

  private:
    Latency_Supplier* impl_;
  };

  /**
   * Construction.  Requires the total number of messages to be
   * sent.  If the timestamp flag is enabled, then events are
   * timestamped, e.g., for use in measuring jitter.
   */
  Latency_Supplier (const u_int total_messages,
                    CORBA::Long supplier_id,
                    const int timestamp = 0);

  /**
   * Registers with the name server under the given <name>.  Also
   * connects to the Event Channel as a supplier of notifications and
   * shutdown messages.  If <master> != 0, then the supplier will
   * destroy the channel upon shutting down. Returns 0 on success, -1
   * on failure.
   */
  int open_supplier (RtecEventChannelAdmin::EventChannel_ptr event_channel,
                     const char *name, int master);

  /// Disconnect from the Event Service, deactivate the supplier and
  /// consumer personalities
  void close (CORBA::Environment &ACE_TRY_ENV);

  /// The channel is disconnecting.
  void disconnect_push_supplier (CORBA::Environment &);

  /// The channel is disconnecting.
  void disconnect_push_consumer (CORBA::Environment &);

  /// Takes a timestamp and then pushes event_ to all consumers, either
  /// directly, or via a channel.
  void push (const RtecEventComm::EventSet &events,
             CORBA::Environment &);

  /**
   * Called when the supplier should start generating events.
   * Registers with the Event Channel to receive timeouts every .25
   * seconds.  Will generate some number of events and then send a
   * shutdown message.  Returns 0 on success, -1 on failure.
   */
  int start_generating_events (void);

  void entry_point (const char *);
  const char *entry_point () const;

  /// Print timing statistics.
  void print_stats () /* const */;

protected:
  void shutdown (void);

  //@{
  /// Event Channel adminstration references.
  RtecEventChannelAdmin::EventChannel_var channel_admin_;
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  RtecEventChannelAdmin::ProxyPushConsumer_var consumers_;
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_;
  RtecEventChannelAdmin::ProxyPushSupplier_var suppliers_;
  //@}

private:
  RtecBase::handle_t rt_info_;

  /// How many events to push before ending the test.
  u_int total_messages_;

  /// Our supplier ID.
  CORBA::Long supplier_id_;

  /// Flag indicating whether or not to timestamp outgoing events.
  int timestamp_;

  /// How many events we've pushed.
  u_int total_sent_;

  //@{
  /// Start/stop times, marking the time period when events are
  /// sent to consumers.
  ACE_Time_Value test_start_time_;
  ACE_Time_Value test_stop_time_;
  //@}

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

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* EVENT_LATENCY_H */
