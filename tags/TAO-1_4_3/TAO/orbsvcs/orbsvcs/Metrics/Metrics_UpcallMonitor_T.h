// $Id$

#ifndef METRICS_UPCALL_MONITOR_T_H
#define METRICS_UPCALL_MONITOR_T_H

#if ! defined (VXWORKS)

// Comment out both of these to disable output
#  define METRICS_MONITOR_ERROR_OUTPUT_ENABLED
#  define METRICS_MONITOR_DEBUG_OUTPUT_ENABLED

#endif /* ! VXWORKS */

#include "Metrics_LocalCache.h"
#include "Metrics_FrameManager.h"

#include "ace/Hash_Map_Manager.h"

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/MetricsS.h"

#include "orbsvcs/Metrics/Metrics_UpcallMonitor.h"

template <class ACE_LOCK, class ALLOCATOR>
class TAO_Metrics_UpcallMonitorAdapter
  : public RtecEventComm::PushConsumer
{
  // = TITLE
  //   This class implements an adapter for monitoring QoS received by
  //   an event push consumer.
  //
  // = DESCRIPTION
  //   This class wraps the consumer event push upcall with time
  //   probe and monitor reporting calls.
public:

  TAO_Metrics_UpcallMonitorAdapter (
    POA_Metrics::QoSMonitor & monitor,
    TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR> & cache,
    TAO_Metrics_FrameManager<ACE_LOCK> & frame_manager,
    POA_RtecEventComm::PushConsumer & consumer,
    TAO_String_Manager entry_point,
    RtecScheduler::handle_t handle,
    RtecScheduler::Period_t period,
    RtecScheduler::Time wcet,
    RtecScheduler::Criticality_t criticality,
    int adapter_owns_consumer = 1,
    int cache_register = 1);
  // Constructor that takes a reference to the various metrics
  // objects, the actual push consumer we're monitoring, QoS info
  // about the consumer, and an int that indicates whether (non-zero)
  // or not (zero) we need to destroy the consumer in our destructor.

  virtual ~TAO_Metrics_UpcallMonitorAdapter ();
  // Virtual destructor: destroys actual consumer if it owns it.

  RtecScheduler::Period_t get_period ();
  // Get the current period for the adapter.

  void set_period (RtecScheduler::Period_t period);
  // Set the current period for the adapter.

  void push (const RtecEventComm::EventSet & data,
             CORBA::Environment &ACE_TRY_ENV = 
             CORBA::Environment::default_environment ());
  // Do a monitored push to the actual PushConsumer for which we're a proxy

  void disconnect_push_consumer (CORBA::Environment &ACE_TRY_ENV = 
                                 CORBA::Environment::default_environment ());
  // Delegates to the actual PushConsumer for which we're a proxy

protected:

  // = Implementation members.

  POA_Metrics::QoSMonitor & monitor_;
  // A reference to the monitor to which we report.

  TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR> & cache_;
  // A reference to the metrics data cache to which we report.

  TAO_Metrics_FrameManager<ACE_LOCK> & frame_manager_;
  // A reference to the frame manager we use.

  POA_RtecEventComm::PushConsumer & consumer_;
  // A reference to the actual PushConsumer for which we're a proxy

  RtecScheduler::handle_t handle_;
  // Handle of the operation we're wrapping.

  RtecScheduler::Period_t period_;
  // Current period for the adapter.

  ACE_Time_Value  execution_time_;
  // Current execution time for the adapter.

  int is_critical_;
  // Whether or not the adapter is critical.

  int adapter_owns_consumer_;
  // indicates whether (non-zero) or not (zero) we need
  // to destroy the consumer in our destructor.

};

template <class ACE_LOCK, class ALLOCATOR>
class TAO_Metrics_ReportingUpcallMonitorAdapter
  : public TAO_Metrics_UpcallMonitorAdapter <ACE_LOCK, ALLOCATOR>
{
  // = TITLE
  //   This class implements an adapter for monitoring QoS received by
  //   an event push consumer.
  //
  // = DESCRIPTION
  //   This class wraps the consumer event push upcall with time
  //   probe and monitor reporting calls.
public:

  TAO_Metrics_ReportingUpcallMonitorAdapter (
    POA_Metrics::QoSMonitor & monitor,
    TAO_Metrics_ReportingLocalCache<ACE_LOCK, ALLOCATOR> & cache,
    TAO_Metrics_FrameManager<ACE_LOCK> & frame_manager,
    POA_RtecEventComm::PushConsumer & consumer,
    TAO_String_Manager entry_point,
    RtecScheduler::handle_t handle,
    RtecScheduler::Period_t period,
    RtecScheduler::Time wcet,
    RtecScheduler::Criticality_t criticality,
    int adapter_owns_consumer = 1,
    Metrics::QoSLogger_ptr logger = 0);
  // Constructor that takes a reference to a logger object and 
  // info needed to construct or base class.

protected:

  // = Implementation members.

  Metrics::QoSLogger_ptr logger_;
  // A (possibly null) CORBA pointer to a logging object

};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Metrics_UpcallMonitor_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Metrics_UpcallMonitor_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if defined (__ACE_INLINE__)
#include "Metrics_UpcallMonitor_T.i"
#endif /* __ACE_INLINE__ */

#endif /* METRICS_UPCALL_MONITOR_T_H */

