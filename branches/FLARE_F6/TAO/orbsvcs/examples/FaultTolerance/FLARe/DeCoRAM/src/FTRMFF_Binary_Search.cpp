// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Binary_Search.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include "FTRMFF_Binary_Search.h"
#include "FTRMFF_Basic.h"
#include "FTRMFF_Worstfit.h"
#include "CTT_Enhanced.h"
#include "CTT_Basic.h"

FTRMFF_Binary_Search::~FTRMFF_Binary_Search ()
{
}

FTRMFF_Output
FTRMFF_Binary_Search::operator () (const FTRMFF_Input & input)
{
  FTRMFF_Binary_Search_Algorithm algorithm (input.processors,
                                      input.backup_count);

  FTRMFF_Output output;
  output.schedule = algorithm (input.tasks);
  output.unscheduled_tasks = algorithm.get_unschedulable ();

  DBG_OUT (algorithm.schedule ());

  return output;
}

FTRMFF_Binary_Search_Algorithm::FTRMFF_Binary_Search_Algorithm (
  const PROCESSOR_LIST & processors,
  unsigned int consistency_level)
  : processors_ (processors),
    schedule_ (create_schedule (processors)),
    consistency_level_ (consistency_level)
{
}

FTRMFF_Binary_Search_Algorithm::~FTRMFF_Binary_Search_Algorithm ()
{
}

SCHEDULING_MAP
FTRMFF_Binary_Search_Algorithm::operator () (const TASK_LIST & tasks)
{
#if 0
  // step one, only schedule primaries to get lower bound
  FTRMFF_Primary_Algorithm primary_ftrmff (processors_);

  primary_ftrmff (tasks);

  unsigned long minimum = 
    processor_usage (primary_ftrmff.schedule ());
#endif

  // step two, schedule replicas with only state synchronization times
  CTT_Enhanced ctt_enhanced;
  FTRMFF_Basic_Algorithm only_ss_ftrmff (processors_,
                                         consistency_level_,
                                         ctt_enhanced);

  only_ss_ftrmff (tasks);
  
  unsigned long lower_bound = 
    processor_usage (only_ss_ftrmff.schedule ());

  // step three, schedule all replicas as active
  CTT_Basic ctt_basic;
  FTRMFF_Basic_Algorithm active_ftrmff (processors_,
                                        consistency_level_,
                                        ctt_basic);

  active_ftrmff (tasks);
  
  unsigned long upper_bound = 
    processor_usage (active_ftrmff.schedule ());

  unsigned long min = lower_bound;
  unsigned long max = upper_bound;
  unsigned long processors = upper_bound;

  while (min < max)
    {
      TRACE ("Binary Search: max = " << max << " min = " << min);

      // schedule with the average value between minimum and maximum
      FTRMFF_Worstfit_Algorithm worstfit_ftrmff (
        create_processors (min + ((max - min)/2)),
        consistency_level_);
     
      worstfit_ftrmff (tasks);

      // determine number of used processors
      processors = processor_usage (worstfit_ftrmff.schedule ());

      // if successful schedule
      if (worstfit_ftrmff.get_unschedulable ().empty ())
        {
          // store number of processors as new max
          max = processors;
          schedule_ = worstfit_ftrmff.schedule ();
        }
      else // not schedulable
        {
          // don't look for solutions with smaller numbers of
          // processors
          min = processors + 1;
        }
    }

  if ((min < upper_bound) && (processor_usage (schedule_) > 0))
    {
      return transform_schedule (schedule_);
    }
  else
    {
      // in failure case try maximum number of processors and then
      // give up.
      FTRMFF_Worstfit_Algorithm worstfit_ftrmff (
        create_processors (max),
        consistency_level_);
      
      SCHEDULING_MAP result = worstfit_ftrmff (tasks);

      unschedulable_ = worstfit_ftrmff.get_unschedulable ();

      schedule_ = worstfit_ftrmff.schedule ();

      return result;
    }

  return transform_schedule (schedule_);
}

SCHEDULE_PROGRESS_LIST
FTRMFF_Binary_Search_Algorithm::get_unschedulable ()
{
  return unschedulable_;
}

const SCHEDULE & 
FTRMFF_Binary_Search_Algorithm::schedule () const
{
  return schedule_;
}
