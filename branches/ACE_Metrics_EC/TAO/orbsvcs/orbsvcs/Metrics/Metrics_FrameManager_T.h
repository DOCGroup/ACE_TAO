// $Id$

#ifndef METRICS_FRAME_MANAGER_T_H
#define METRICS_FRAME_MANAGER_T_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "ace/High_Res_Timer.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/MetricsS.h"

#include "orbsvcs/Metrics/Metrics_FrameManager.h"


template <class ACE_LOCK>
struct TAO_Metrics_Frame_Manager_Data
{
  // = TITLE
  //   Defines a class for time frame management data.
  //
  // = DESCRIPTION
  //   This class makes use of the <ACE_Time_Value> class to
  //   store period and endpoint data for a given time frame.

public:

  TAO_Metrics_Frame_Manager_Data (RtecScheduler::Period_t p)
    : frame_id_ (0), period_ (0, p / 10) {}
  // Constructor.  Note that p is in hundreds of nanoseconds so we
  // divide by ten to get microseconds.  The start_ and end_ data
  // members are set equal to ACE_Time_Value::zero by their own
  // default constructors.

  CORBA::ULong frame_id_;
  // Stores the number of previous frames for that period.  May roll
  // over, but is intended to provide a locally (for a large range of
  // locality) unique frame identifier.

  ACE_Time_Value period_;
  // Relative time to the next frame, from the start of this one.

  ACE_Time_Value start_;
  // Absolute time of the start of the frame and end of the last.

  ACE_Time_Value end_;
  // Absolute time to the end of the frame and start of the next.

  ACE_LOCK lock_;
  // Lock for synchronizing access to the data instance.
};


template <class ACE_LOCK>
class TAO_Metrics_FrameManager : public POA_Metrics::FrameManager

{
  // = TITLE
  //   Defines a class that implements a local metrics cache.
  //
  // = DESCRIPTION
  //   This class makes use of the <ACE_Metrics_Cache> class to
  //   implement a cache for local use.
public:

  typedef TAO_Metrics_FrameManager<ACE_LOCK> TAO_METRICS_FRAME_MANAGER_TYPE;
  typedef TAO_Metrics_Frame_Manager_Data<ACE_LOCK> TAO_METRICS_FRAME_MANAGER_DATA_TYPE;

  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::Period_t,
	                          TAO_METRICS_FRAME_MANAGER_DATA_TYPE *,
                                  ACE_Hash<RtecScheduler::Period_t>,
                                  ACE_Equal_To<RtecScheduler::Period_t>,
                                  ACE_Null_Mutex> METRICS_FRAME_DATA_MAP;
  // Type of map used for O(1) lookup of monitored data for operations.

  typedef ACE_Hash_Map_Entry<RtecScheduler::Period_t,
                             TAO_METRICS_FRAME_MANAGER_DATA_TYPE *>
    METRICS_FRAME_DATA_MAP_ENTRY;
  // Type of entry in monitor map.

  typedef ACE_Hash_Map_Iterator_Ex<RtecScheduler::Period_t,
	                           TAO_METRICS_FRAME_MANAGER_DATA_TYPE *,
                                   ACE_Hash<RtecScheduler::Period_t>,
                                   ACE_Equal_To<RtecScheduler::Period_t>,
                                   ACE_Null_Mutex>
    METRICS_FRAME_DATA_MAP_ITERATOR;
  // Type of iterator for monitor map.

  TAO_Metrics_FrameManager ();
  // Constructor.

  ~TAO_Metrics_FrameManager ();
  // Destructor.

  CORBA::Short register_period (RtecScheduler::Period_t p,
                                CORBA::Environment &ACE_TRY_ENV
                                  = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION));
  // Binds the period to the internal frame map, with a new frame data
  // structure.  Returns 0 if a new entry is bound successfully,
  // returns 1 if an attempt is made to bind an existing entry, and
  // returns -1 if failures occur.

  CORBA::Short update_all_frames (const ACE_Time_Value& tv);
  // Updates time frames, based on the passed time value.

  CORBA::Short update_all_frames_with_time (Metrics::Time mt,
                                            CORBA::Environment &ACE_TRY_ENV
                                              = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::SYNCHRONIZATION));
  // Updates time frames, based on the passed time value.

  CORBA::Short update_all_frames (CORBA::Environment &ACE_TRY_ENV
                                    = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::SYNCHRONIZATION));
  // Updates time frames, based on the current time as of the call.

  CORBA::Short update_frame (RtecScheduler::Period_t p, const ACE_Time_Value& tv);
  // Updates the passed period's time frame, based on the passed time
  // value.

  CORBA::Short update_frame_with_time (RtecScheduler::Period_t p,
                                       Metrics::Time mt,
                                       CORBA::Environment &ACE_TRY_ENV
                                         = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION));
  // Updates the passed period's time frame, based on the passed time
  // value.

  CORBA::Short update_frame (RtecScheduler::Period_t p,
                             CORBA::Environment &ACE_TRY_ENV
                               = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION));
  // Updates the passed period's time frame, based on the current time
  // as of the call.

  CORBA::Short reset_all_frames (const ACE_Time_Value& tv);
  // Resets time frames, based on the passed time value.

  CORBA::Short reset_all_frames_with_time (Metrics::Time mt,
                                           CORBA::Environment &ACE_TRY_ENV
                                             = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::SYNCHRONIZATION));
  // Resets time frames, based on the passed time value.

  CORBA::Short reset_all_frames (CORBA::Environment &ACE_TRY_ENV
                                   = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::SYNCHRONIZATION));
  // Resets time frames, based on the current time as of the call.

  CORBA::Short reset_frame (RtecScheduler::Period_t p, const ACE_Time_Value& tv);
  // Resets the passed period's start-of-frame, based on the passed time
  // value.

  CORBA::Short reset_frame_with_time (RtecScheduler::Period_t p,
                                      Metrics::Time mt,
                                      CORBA::Environment &ACE_TRY_ENV
                                        = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION));
  // Resets the passed period's start-of-frame, based on the passed time
  // value.

  CORBA::Short reset_frame (RtecScheduler::Period_t p,
                            CORBA::Environment &ACE_TRY_ENV
                              = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION));
  // Resets the passed period's start-of-frame, based on the current time
  // as of the call.

  CORBA::Short get_start_time (RtecScheduler::Period_t p, ACE_Time_Value &start);
  // Gets the start time for the period's current frame.

  CORBA::Short get_start_time (RtecScheduler::Period_t p,
                               Metrics::Time &start,
                               CORBA::Environment &ACE_TRY_ENV
                                 = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION));
  // Gets the start time for the period's current frame.

  CORBA::Short get_end_time (RtecScheduler::Period_t p, ACE_Time_Value &end);
  // Gets the end time for the period's current frame.

  CORBA::Short get_end_time (RtecScheduler::Period_t p,
                             Metrics::Time &end,
                             CORBA::Environment &ACE_TRY_ENV
                               = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION));
  // Gets the end time for the period's current frame.

  CORBA::Short get_frame_id (RtecScheduler::Period_t p,
                             CORBA::ULong &id,
                             CORBA::Environment &ACE_TRY_ENV
                               = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION));
  // Gets the locally unique identifier for the period's current frame.

  CORBA::Short get_frame_data (RtecScheduler::Period_t p, ACE_Time_Value &start,
                      ACE_Time_Value &end, CORBA::ULong &id);
  // Gets the start time, end time, and id period's current frame.

  CORBA::Short get_frame_data (RtecScheduler::Period_t p,
                               Metrics::Time &start,
                               Metrics::Time &end,
                               CORBA::ULong &id,
                               CORBA::Environment &ACE_TRY_ENV
                                 = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION));
  // Gets the start time, end time, and id period's current frame.

protected:

  CORBA::Short update_data (TAO_Metrics_Frame_Manager_Data<ACE_LOCK> & data,
                   const ACE_Time_Value& tv);
  // Updates the passed frame data structure's start and end times,
  // based on the passed time value.  This is a protected internal
  // method, and should only be called with locks already held.


  CORBA::Short reset_data (TAO_Metrics_Frame_Manager_Data<ACE_LOCK> & data,
                  const ACE_Time_Value& tv);
  // Resets the passed frame data structure's start and end times, to
  // the passed time value and one period later, respectively.  This
  // is a protected internal method, and should only be called with
  // locks already held.

  METRICS_FRAME_DATA_MAP frame_data_map_;
  // Maps periods to frame data.

  // Count of the number of registered periods.
  u_long period_count_;

  ACE_LOCK lock_;
  // Lock for synchronizing access to the map.
};

#if defined (__ACE_INLINE__)
#include "Metrics_FrameManager_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Metrics_FrameManager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Metrics_FrameManager_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* METRICS_FRAME_MANAGER_T_H */

