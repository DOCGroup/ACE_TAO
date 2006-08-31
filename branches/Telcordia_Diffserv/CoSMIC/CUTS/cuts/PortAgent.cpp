// $Id$

#include "cuts/PortAgent.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/PortAgent.inl"
#endif

#include "cuts/ActivationRecord.h"
#include "cuts/Port_Measurement.h"
#include "cuts/Time.h"
#include "ace/Thread_Manager.h"
#include <algorithm>

//=============================================================================
/**
 * struct Record_Record_Entry
 */
//=============================================================================

struct Record_Record_Entry
{
  Record_Record_Entry (CUTS_Port_Measurement * port_measurement)
    : port_measurement_ (port_measurement)
  {

  }

  void operator () (const CUTS_Activation_Record::Entry & entry)
  {
    this->port_measurement_->record_entry (
      entry.reps_, entry.worker_id_, entry.action_id_,
      entry.stop_time_ - entry.start_time_);
  }

private:
  CUTS_Port_Measurement * port_measurement_;
};

//=============================================================================
/**
 * struct Record_Exit_Point
 */
//=============================================================================

struct Record_Exit_Point
{
  Record_Exit_Point (
    CUTS_Port_Measurement * port_measurement,
    const ACE_Time_Value & start_time)
    : port_measurement_ (port_measurement),
      start_time_ (start_time)
  {

  }

  void operator () (
    const CUTS_Activation_Record::Exit_Points::value_type & entry)
  {
    this->port_measurement_->record_exit_point_time (
      entry.first, (entry.second - this->start_time_));
  }

private:
  /// Pointer to the target port measurement.
  CUTS_Port_Measurement * port_measurement_;

  /// Associated start time for all the exit times.
  const ACE_Time_Value & start_time_;
};

//=============================================================================
/*
 * CUTS_Port_Agent
 */
//=============================================================================

static const int DEFAULT_FREE_LIST_SIZE = 5;

//
// CUTS_Port_Agent
//
CUTS_Port_Agent::CUTS_Port_Agent (void)
: name_ ("unknown"),
  active_ (false),
  free_list_ (ACE_FREE_LIST_WITH_POOL, CUTS_INIT_ACTIVATION_RECORD_COUNT),
  measurement_pool_ (2),
  grp_id_ (-1)
{

}

//
// ~CUTS_Port_Agent
//
CUTS_Port_Agent::~CUTS_Port_Agent (void)
{

}

//
// svc
//
ACE_THR_FUNC_RETURN CUTS_Port_Agent::event_loop (void * param)
{
  CUTS_Port_Agent * agent = reinterpret_cast <CUTS_Port_Agent *> (param);

  // Process all the events.
  while (agent->active_)
  {
    CUTS_Activation_Record * record = 0;
    int retval = agent->closed_list_.dequeue_head (record);

    if (retval != -1 && record != 0)
    {
      agent->record_metrics (record);
    }
  }

  return 0;
}

//
// handle_input
//
void CUTS_Port_Agent::record_metrics (CUTS_Activation_Record * record)
{
  // Get the current collection point in the mapping.
  CUTS_Port_Measurement_Map & pmmap = this->measurement_pool_.current ();

  // Locate the measurements for this owner. If we can not
  // find the records then we need to create a new entry for it.
  CUTS_Port_Measurement * measurement = 0;

  if (pmmap.find (record->owner (), measurement) == -1)
  {
    ACE_NEW (measurement, CUTS_Port_Measurement);

    if (measurement != 0)
    {
      if (pmmap.bind (record->owner (), measurement) != 0)
      {
        delete measurement;
        measurement = 0;
      }
    }
  }

  if (measurement != 0)
  {
    // Record the processing time for the activation record.
    measurement->transit_time (record->transit_time ());
    measurement->process_time (record->stop_time () - record->start_time ());

    // Record all the entries in the activation record.
    std::for_each (record->entries ().begin (),
                   record->entries ().end (),
                   Record_Record_Entry (measurement));

    // Record all the exit points in the activation record.
    std::for_each (record->exit_points ().begin (),
                   record->exit_points ().end (),
                   Record_Exit_Point (measurement, record->start_time ()));
  }

  // Insert the activation record into the <free_list_>.
  this->add_to_free_list (record);
}

//
// activate
//
void CUTS_Port_Agent::activate (void)
{
  if (this->active_)
  {
    return;
  }

  this->active_ = true;
  this->grp_id_ =
    ACE_Thread_Manager::instance ()->spawn (&CUTS_Port_Agent::event_loop,
                                            this);

  if (this->grp_id_ == -1)
  {
    ACE_ERROR ((LM_ERROR,
                "[%M] -%T - failed to activate port agent for %s\n",
                this->name_.c_str ()));
    this->active_ = false;
  }
  else
  {
    this->closed_list_.activate ();
  }
}

//
// add_to_free_list
//
void CUTS_Port_Agent::add_to_free_list (CUTS_Activation_Record * record)
{
  try
  {
    CUTS_Cached_Activation_Record * cached_record =
      dynamic_cast <CUTS_Cached_Activation_Record *> (record);

    cached_record->reset ();
    this->free_list_.add (cached_record);
  }
  catch (...)
  {
    delete record;
  }
}

//
// deactivate
//
void CUTS_Port_Agent::deactivate (void)
{
  if (!this->active_)
  {
    return;
  }

  this->active_ = false;

  // Gracefully clean the <closed_list_> and deactivate the
  // <closed_list_> message queue.
  if (!this->closed_list_.is_empty ())
  {
    CUTS_Activation_Record * record = 0;
    while (this->closed_list_.dequeue_head (record) > 0)
    {
      this->add_to_free_list (record);
    }
  }

  this->closed_list_.deactivate ();

  // Wait to for the port agents thread to return.
  ACE_Thread_Manager::instance ()->wait_grp (this->grp_id_);
  this->grp_id_ = -1;
}
