// $Id$

#ifndef METRICS_UPCALL_MONITOR_H
#define METRICS_UPCALL_MONITOR_H

#if defined (ACE_METRICS_COLLECTION)

#if defined (ACE_ENABLE_TIMEPROBES) && defined (ACE_COMPILE_TIMEPROBES)

#if ! defined (VXWORKS)

// Comment out both of these to disable output
#  define METRICS_MONITOR_ERROR_OUTPUT_ENABLED
#  define METRICS_MONITOR_DEBUG_OUTPUT_ENABLED

#endif /* ! VXWORKS */

#include "ace/Singleton.h"
#include "ace/Hash_Map_Manager.h"

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/MetricsS.h"

#include "Metrics_LocalCache.h"
#include "Metrics_FrameManager.h"
#include "Metrics_Utils.h"

class TAO_Metrics_Export TAO_Metrics_UpcallMonitor
  : public POA_Metrics::QoSMonitor
{
  // = TITLE
  //   This class implements a Metrics QoS monitor.
  //
  // = DESCRIPTION
  //   This class allows clients to monitor QoS, collected
  //   by the rest of the metrics framework.
public:

  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  CORBA::ULong,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_SYNCH_MUTEX> MONITOR_MAP;
  // Type of map used for O(1) lookup of missed/made deadline counts
  // by operation handles.

  typedef ACE_Hash_Map_Entry<RtecScheduler::handle_t, CORBA::ULong>
    MONITOR_MAP_ENTRY;
  // Type of map used for O(1) lookup of RT_Infos by their handles.

  typedef ACE_Hash_Map_Iterator_Ex<RtecScheduler::handle_t,
                                  CORBA::ULong,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_SYNCH_MUTEX> MONITOR_MAP_ITERATOR;
  // Type of map used for O(1) lookup of missed/made deadline counts
  // by operation handles.


  TAO_Metrics_UpcallMonitor ();
  // Default constructor.

  ~TAO_Metrics_UpcallMonitor ();
  // Destructor.

  virtual void
  report_made_deadline (RtecScheduler::handle_t handle
                        ACE_ENV_ARG_DECL
                          )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_0));
  // Reports a successful upcall.

  virtual void
  report_missed_deadline (RtecScheduler::handle_t handle
                          ACE_ENV_ARG_DECL
                          )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_1));
  // Reports an unsuccessful upcall.

  virtual void
  get_aggregate_QoS (RtecScheduler::handle_t handle,
                     Metrics::QoSParameter_t_out qos
                     ACE_ENV_ARG_DECL
                     )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_2,
                     Metrics::INTERNAL_3));
  // Get the aggregate QoS statistics collected for the passed handle.

  virtual void get_aggregate_QoS_set (
      const Metrics::Handle_Set_t & handle_set,
      Metrics::QoSParameter_Set_out qos_set
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_4,
                     Metrics::INTERNAL_5));
  // Get the aggregate QoS statistics collected for each handle in the
  // passed set.

  virtual void
  reset_statistics (RtecScheduler::handle_t handle
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_7,
                     Metrics::INTERNAL_8));
  // Resets the aggregate QoS information for the given operation.

  MONITOR_MAP &missed_map ();
  // Accessor for map of operation handle to a count of deadlines missed.

  MONITOR_MAP &made_map ();
  // Accessor for map of operation handle to a count of deadlines made.

private:

  // = Implementation members.

  MONITOR_MAP missed_map_;
  // Maps operation handle to a count of deadlines missed.

  MONITOR_MAP made_map_;
  // Maps operation handle to a count of deadlines made.

};


class TAO_Metrics_Export TAO_Metrics_ReportingUpcallMonitor
  : public POA_Metrics::ReportingQoSMonitor
{
  // = TITLE
  //   This class implements a reporting extension for a Metrics QoS monitor.
  //
  // = DESCRIPTION
  //   This class uses delegation to avoid the ugly consequences of
  //   multiple inheritance from a common IDL base class along two
  //   inheritance paths.
public:

  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  CORBA::ULong,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_SYNCH_MUTEX> MONITOR_MAP;
  // Type of map used for O(1) lookup of missed/made deadline counts
  // by operation handles.

  typedef ACE_Hash_Map_Entry<RtecScheduler::handle_t, CORBA::ULong>
    MONITOR_MAP_ENTRY;
  // Type of map used for O(1) lookup of RT_Infos by their handles.

  typedef ACE_Hash_Map_Iterator_Ex<RtecScheduler::handle_t,
                                  CORBA::ULong,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_SYNCH_MUTEX> MONITOR_MAP_ITERATOR;
  // Type of map used for O(1) lookup of missed/made deadline counts
  // by operation handles.


  TAO_Metrics_ReportingUpcallMonitor ();
  // Default constructor.

  ~TAO_Metrics_ReportingUpcallMonitor ();
  // Destructor.

  virtual void
  report_made_deadline (RtecScheduler::handle_t handle
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_0));
  // Reports a successful upcall.

  virtual void
  report_missed_deadline (RtecScheduler::handle_t handle
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_1));
  // Reports an unsuccessful upcall.

  virtual void
  get_aggregate_QoS (RtecScheduler::handle_t handle,
                     Metrics::QoSParameter_t_out qos
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_2,
                     Metrics::INTERNAL_3));
  // Get the aggregate QoS statistics collected for the passed handle.

  virtual void get_aggregate_QoS_set (
      const Metrics::Handle_Set_t & handle_set,
      Metrics::QoSParameter_Set_out qos_set
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_4,
                     Metrics::INTERNAL_5));
  // Get the aggregate QoS statistics collected for each handle in the
  // passed set.

  virtual void report_aggregate_QoS (
      Metrics::QoSLogger_ptr logger,
      Metrics::Time interval
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::NIL_LOGGER,
                     Metrics::INTERNAL_6));
  // Report the aggregate QoS statistics to the logger.

  virtual void
  reset_statistics (RtecScheduler::handle_t handle
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_7,
                     Metrics::INTERNAL_8));
  // Resets the aggregate QoS information for the given operation.

private:

  TAO_Metrics_UpcallMonitor monitor_;
  // The monitor we're wrapping.
};


#if defined (__ACE_INLINE__)
#include "Metrics_UpcallMonitor.i"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Metrics/Metrics_UpcallMonitor_T.h"

typedef TAO_Metrics_UpcallMonitorAdapter<ACE_SYNCH_MUTEX, ACE_New_Allocator>
TAO_METRICS_MONITOR_ADAPTER_TYPE;

typedef TAO_Metrics_ReportingUpcallMonitorAdapter<ACE_SYNCH_MUTEX, ACE_New_Allocator>
TAO_METRICS_REPORTING_MONITOR_ADAPTER_TYPE;

typedef ACE_Singleton<TAO_Metrics_UpcallMonitor, ACE_SYNCH_MUTEX>
TAO_METRICS_MONITOR_SINGLETON_TYPE;

typedef ACE_Singleton<TAO_Metrics_ReportingUpcallMonitor, ACE_SYNCH_MUTEX>
TAO_METRICS_REPORTING_MONITOR_SINGLETON_TYPE;

#if defined (TAO_USES_REPORTING_METRICS)
typedef TAO_METRICS_REPORTING_MONITOR_ADAPTER_TYPE TAO_METRICS_MONITOR_ADAPTER;
typedef TAO_METRICS_REPORTING_MONITOR_SINGLETON_TYPE TAO_METRICS_MONITOR_SINGLETON;
#else
typedef TAO_METRICS_MONITOR_ADAPTER_TYPE TAO_METRICS_MONITOR_ADAPTER;
typedef TAO_METRICS_MONITOR_SINGLETON_TYPE TAO_METRICS_MONITOR_SINGLETON;
#endif

#endif /* ACE_ENABLE_TIMEPROBES & ACE_COMPILE_TIMEPROBES */
#endif /* ACE_METRICS_COLLECTION */

#endif /* METRICS_UPCALL_MONITOR_H */
