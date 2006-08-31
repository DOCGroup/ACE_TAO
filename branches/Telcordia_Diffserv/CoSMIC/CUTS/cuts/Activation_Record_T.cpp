// $Id$

#ifndef _CUTS_ACTIVATION_RECORD_T_CPP_
#define _CUTS_ACTIVATION_RECORD_T_CPP_

#if !defined (__CUTS_INLINE__)
#include "cuts/Activation_Record_T.inl"
#endif

#include "cuts/Worker.h"
#include "cuts/Action.h"

//
// perform_action_no_logging
//
template <typename ACTION>
void CUTS_Activation_Record::perform_action_no_logging (size_t repetitions,
                                                        const ACTION & action)
{
  for (size_t rep = 0; rep < repetitions; rep ++)
  {
    action.execute ();
  }
}

//
// perform_action
//
template <typename ACTION>
void CUTS_Activation_Record::perform_action (const ACTION & action)
{
  // Save the start time of the action.
  this->action_state_time_ = ACE_OS::gettimeofday ();

  action.execute ();

  // Save the stop time of the action.
  this->action_stop_time_ = ACE_OS::gettimeofday ();

  // Save the timing information into the record.
  this->log_time_measurement (
    1,
    CUTS_Worker_Traits <typename ACTION::Worker_Type>::worker_id_,
    CUTS_Action_Traits <ACTION>::action_id_);
}

//
// perform_action
//
template <typename ACTION>
void CUTS_Activation_Record::perform_action (size_t repetitions,
                                             const ACTION & action)
{
  // Save the start time of the action.
  this->action_state_time_ = ACE_OS::gettimeofday ();

  for (size_t rep = 0; rep < repetitions; rep ++)
  {
    action.execute ();
  }

  // Save the stop time of the action.
  this->action_stop_time_ = ACE_OS::gettimeofday ();

  // Save the timing information into the record.
  this->log_time_measurement (
    repetitions,
    CUTS_Worker_Traits <typename ACTION::Worker_Type>::worker_id_,
    CUTS_Action_Traits <ACTION>::action_id_);
}

//
// record_exit_point
//
template <typename ACTION>
void CUTS_Activation_Record::record_exit_point (const std::string & uuid,
                                                const ACTION & action)
{
  // Perform the action and record its <exit_time>.
  ACE_Time_Value toc;
  action (toc);

  this->exit_points_[uuid] = toc;
}

#endif // !defined _CUTS_ACTIVATION_RECORD_T_CPP_
