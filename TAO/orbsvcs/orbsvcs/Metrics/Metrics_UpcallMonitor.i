// $Id$

// @METRICS@

#ifndef METRICS_UPCALL_MONITOR_I
#define METRICS_UPCALL_MONITOR_I

//# if defined (METRICS_USES_OP_CANCELLATION)
//# undef METRICS_USES_OP_CANCELLATION
//# endif /* METRICS_USES_OP_CANCELLATION */

# if ! defined (METRICS_USES_OP_CANCELLATION)
#   define METRICS_USES_OP_CANCELLATION
# endif /* ! METRICS_USES_OP_CANCELLATION */


/////////////////////////////////////
// Class TAO_Metrics_UpcallMonitor //
/////////////////////////////////////

// Default constructor.

ACE_INLINE
TAO_Metrics_UpcallMonitor::TAO_Metrics_UpcallMonitor ()
{
}

// Destructor.

ACE_INLINE
TAO_Metrics_UpcallMonitor::~TAO_Metrics_UpcallMonitor ()
{
}

// Reports a successful upcall.

ACE_INLINE void
TAO_Metrics_UpcallMonitor::
report_made_deadline (
        RtecScheduler::handle_t handle
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , Metrics::INTERNAL_0
      ))
{
  CORBA::ULong count = 0;
  MONITOR_MAP_ENTRY *entry;

  if (made_map_.trybind (handle, count, entry) < 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Failed trybind for handle %u in made map\n", handle));
      ACE_THROW (Metrics::INTERNAL_0 ());
    }

  ++entry->int_id_;
}

// Reports an unsuccessful upcall.

ACE_INLINE void
TAO_Metrics_UpcallMonitor::
report_missed_deadline (
        RtecScheduler::handle_t handle
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , Metrics::INTERNAL_1
      ))
{
  CORBA::ULong count = 0;
  MONITOR_MAP_ENTRY *entry;

  if (missed_map_.trybind (handle, count, entry) < 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Failed trybind for handle %u in missed map\n", handle));
      ACE_THROW (Metrics::INTERNAL_1 ());
    }

  ++entry->int_id_;
}


// Get the aggregate QoS statistics collected so far by the monitor.

ACE_INLINE void
TAO_Metrics_UpcallMonitor::
get_aggregate_QoS (
        RtecScheduler::handle_t handle,
        Metrics::QoSParameter_t_out qos
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , Metrics::INTERNAL_2
        , Metrics::INTERNAL_3
      ))
{
  CORBA::ULong made_count = 0;
  CORBA::ULong missed_count = 0;
  MONITOR_MAP_ENTRY *made_entry;
  MONITOR_MAP_ENTRY *missed_entry;

  if (made_map_.trybind (handle, made_count, made_entry) < 0)
    {
      ACE_DEBUG ((LM_DEBUG, "TAO_Metrics_UpcallMonitor::get_aggregate_QoS: "
                  "Failed trybind for handle %u in made map\n", handle));
      ACE_THROW (Metrics::INTERNAL_2 ());
    }

  if (missed_map_.trybind (handle, missed_count, missed_entry) < 0)
    {
      ACE_DEBUG ((LM_DEBUG, "TAO_Metrics_UpcallMonitor::get_aggregate_QoS: "
                  "Failed trybind for handle %u in missed map\n", handle));
      ACE_THROW (Metrics::INTERNAL_3 ());
    }

  ACE_NEW_THROW_EX (qos,
                    Metrics::QoSParameter_t,
                    CORBA::NO_MEMORY ());

  qos->handle = handle;
  qos->deadlines_made = made_entry->int_id_;
  qos->deadlines_missed = missed_entry->int_id_;
  qos->operations_cancelled = 0;
  qos->is_hrt = 0;
}


// Get the aggregate QoS statistics collected for each handle in the
// passed set.

ACE_INLINE void
TAO_Metrics_UpcallMonitor::
get_aggregate_QoS_set (
        const Metrics::Handle_Set_t & handle_set,
        Metrics::QoSParameter_Set_out qos_set
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , Metrics::INTERNAL_4
        , Metrics::INTERNAL_5
      ))
{
  CORBA::ULong made_count = 0;
  CORBA::ULong missed_count = 0;
  MONITOR_MAP_ENTRY *made_entry = 0;
  MONITOR_MAP_ENTRY *missed_entry = 0;

  ACE_NEW_THROW_EX (qos_set,
                    Metrics::QoSParameter_Set (handle_set.length ()),
                    CORBA::NO_MEMORY ());
  qos_set->length (handle_set.length ());


  for (u_int i = 0; i < handle_set.length (); ++i)
    {
      made_count = 0;
      missed_count = 0;
      made_entry = 0;
      missed_entry = 0;

      if (made_map_.trybind (handle_set [i], made_count, made_entry) < 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO_Metrics_UpcallMonitor::get_aggregate_QoS_set: "
                      "Failed trybind for handle_set [%d] = %u in made map\n",
                      i, handle_set [i]));
          ACE_THROW (Metrics::INTERNAL_4 ());
        }

      if (missed_map_.trybind (handle_set [i], missed_count, missed_entry) < 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO_Metrics_UpcallMonitor::get_aggregate_QoS_set: "
                      "Failed trybind for handle_set[%d]=%u in missed map\n",
                      i, handle_set [i]));
          ACE_THROW (Metrics::INTERNAL_5 ());
        }

      qos_set [i].handle = handle_set [i];
      qos_set [i].deadlines_made = made_entry->int_id_;
      qos_set [i].deadlines_missed = missed_entry->int_id_;
      qos_set [i].operations_cancelled = 0;
      qos_set [i].is_hrt = 0;
    }
}



// Resets the aggregate QoS information for the given operation.

ACE_INLINE void
TAO_Metrics_UpcallMonitor::
reset_statistics (
        RtecScheduler::handle_t handle
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , Metrics::INTERNAL_7
        , Metrics::INTERNAL_8
      ))
{
  CORBA::ULong count = 0;

  if (made_map_.rebind (handle, count) < 0)
    {
      ACE_THROW (Metrics::INTERNAL_7 ());
    }

  if (missed_map_.rebind (handle, count) < 0)
    {
      ACE_THROW (Metrics::INTERNAL_8 ());
    }
}


// Accessor for map of operation handle to a count of deadlines missed.

ACE_INLINE
TAO_Metrics_UpcallMonitor::MONITOR_MAP &
TAO_Metrics_UpcallMonitor::missed_map ()
{
  return this->missed_map_;
}

// Accessor for map of operation handle to a count of deadlines made.

ACE_INLINE
TAO_Metrics_UpcallMonitor::MONITOR_MAP &
TAO_Metrics_UpcallMonitor::made_map ()
{
  return this->made_map_;
}

//////////////////////////////////////////////
// Class TAO_Metrics_ReportingUpcallMonitor //
//////////////////////////////////////////////

// Default constructor.

ACE_INLINE
TAO_Metrics_ReportingUpcallMonitor::TAO_Metrics_ReportingUpcallMonitor ()
{
}

// Destructor.

ACE_INLINE
TAO_Metrics_ReportingUpcallMonitor::~TAO_Metrics_ReportingUpcallMonitor ()
{
}

// Reports a successful upcall.

ACE_INLINE void
TAO_Metrics_ReportingUpcallMonitor::
report_made_deadline (RtecScheduler::handle_t handle,
                      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_0))
{
  monitor_.report_made_deadline (handle);
}

// Reports an unsuccessful upcall.

ACE_INLINE void
TAO_Metrics_ReportingUpcallMonitor::
report_missed_deadline (RtecScheduler::handle_t handle,
                        CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_1))
{
  monitor_.report_missed_deadline (handle);
}


// Get the aggregate QoS statistics collected so far by the monitor.

ACE_INLINE void
TAO_Metrics_ReportingUpcallMonitor::get_aggregate_QoS (RtecScheduler::handle_t handle,
                                              Metrics::QoSParameter_t_out qos,
                                              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_2,
                     Metrics::INTERNAL_3))
{
  monitor_.get_aggregate_QoS (handle, qos);
}


// Get the aggregate QoS statistics collected for each handle in the
// passed set.

ACE_INLINE void
TAO_Metrics_ReportingUpcallMonitor::get_aggregate_QoS_set (
      const Metrics::Handle_Set_t & handle_set,
      Metrics::QoSParameter_Set_out qos_set,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_4,
                     Metrics::INTERNAL_5))
{
  monitor_.get_aggregate_QoS_set (handle_set, qos_set);
}



// Resets the aggregate QoS information for the given operation.

ACE_INLINE void
TAO_Metrics_ReportingUpcallMonitor::reset_statistics (RtecScheduler::handle_t handle,
                                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL_7,
                     Metrics::INTERNAL_8))
{
  monitor_.reset_statistics (handle);
}


// Reports the aggregate QoS statistics to the logger.

ACE_INLINE void
TAO_Metrics_ReportingUpcallMonitor::report_aggregate_QoS (
      Metrics::QoSLogger_ptr logger,
      Metrics::Time interval,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::NIL_LOGGER,
                     Metrics::INTERNAL_6))
{
  if (CORBA::is_nil (logger))
    {
      ACE_THROW (Metrics::NIL_LOGGER ());
    }

  Metrics::QoSParameter_Set qos_set;
  u_int set_length = 1;

  // First, look at made map handles
  MONITOR_MAP_ITERATOR iter (monitor_.made_map ());
  for (; iter.done () == 0; ++iter, ++set_length)
    {
      CORBA::ULong missed_count;
      if (monitor_.missed_map ().find ((*iter).ext_id_, missed_count) < 0)
        {
          missed_count = 0;
        }

      qos_set.length (set_length);

      qos_set [set_length - 1].entry_point = (const char *) "MONITOR MADE_MAP_OP";
      qos_set [set_length - 1].handle = (*iter).ext_id_;
      qos_set [set_length - 1].deadlines_made = (*iter).int_id_;
      qos_set [set_length - 1].deadlines_missed = missed_count;
      qos_set [set_length - 1].operations_cancelled = 0;
      qos_set [set_length - 1].is_hrt = 0;
    }

  // Then, find missed map handles with no made map entry.
  MONITOR_MAP_ITERATOR missed_iter (monitor_.missed_map ());
  for (; missed_iter.done () == 0; ++missed_iter)
    {
      CORBA::ULong made_count;
      if (monitor_.made_map ().find ((*missed_iter).ext_id_, made_count) < 0)
        {
          qos_set.length (set_length);

          qos_set [set_length - 1].entry_point = (const char *) "MONITOR MISS_MAP_OP";
          qos_set [set_length - 1].handle = (*missed_iter).ext_id_;
          qos_set [set_length - 1].deadlines_made = 0;
          qos_set [set_length - 1].deadlines_missed = (*missed_iter).int_id_;
          qos_set [set_length - 1].operations_cancelled = 0;
          qos_set [set_length - 1].is_hrt = 0;

          ++set_length;
        }
    }

  logger->log_aggregate_QoS (qos_set, interval);
}

#endif /* METRICS_UPCALL_MONITOR_I */


