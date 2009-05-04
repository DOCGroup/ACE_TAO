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
#include "FTRMFF_Bestfit.h"
#include "CTT_Enhanced.h"
#include "CTT_Basic.h"

FTRMFF_Binary_Search::FTRMFF_Binary_Search (
  const std::string & scheduling_algorithm)
  : scheduling_algorithm_ (scheduling_algorithm)
{
}

FTRMFF_Binary_Search::~FTRMFF_Binary_Search (void)
{
}

FTRMFF_Output
FTRMFF_Binary_Search::operator () (const FTRMFF_Input & input)
{
  FTRMFF_Binary_Search_Algorithm algorithm (input.processors,
                                            input.backup_count,
                                            scheduling_algorithm_);

  FTRMFF_Output output;
  output.schedule = algorithm (input.tasks);
  output.unscheduled_tasks = algorithm.get_unschedulable ();

  DBG_OUT (remove_empty_processors (algorithm.schedule ()));

  return output;
}

FTRMFF_Binary_Search_Algorithm::FTRMFF_Binary_Search_Algorithm (
  const PROCESSOR_LIST & processors,
  unsigned int consistency_level,
  const std::string & scheduling_algorithm)
  : FTRMFF_Algorithm_Impl (consistency_level),
    processors_ (processors),
    schedule_ (create_schedule (processors)),
    scheduling_algorithm_ (scheduling_algorithm)
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
      std::auto_ptr <FTRMFF_Algorithm_Impl> ftrmff ( 
        this->create_scheduling_algorithm (
                create_processors (min + ((max - min)/2))));
     
      (*ftrmff) (tasks);

      // determine number of used processors
      processors = processor_usage (ftrmff->schedule ());

      // if successful schedule
      if (ftrmff->get_unschedulable ().empty ())
        {
          // store number of processors as new max
          max = processors;
          schedule_ = ftrmff->schedule ();
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
      std::auto_ptr <FTRMFF_Algorithm_Impl> ftrmff (
        this->create_scheduling_algorithm (create_processors (max)));
      
      SCHEDULING_MAP result = (*ftrmff) (tasks);

      unschedulable_ = ftrmff->get_unschedulable ();

      schedule_ = ftrmff->schedule ();

      return result;
    }

  return transform_schedule (schedule_);
}

const SCHEDULE & 
FTRMFF_Binary_Search_Algorithm::schedule (void) const
{
  return schedule_;
}

FTRMFF_Algorithm_Impl *
FTRMFF_Binary_Search_Algorithm::create_scheduling_algorithm (
  const PROCESSOR_LIST & processors)
{
  if (scheduling_algorithm_ == "bestfit")
    return new FTRMFF_Bestfit_Algorithm (processors,
                                         consistency_level_);
  else
    return new FTRMFF_Worstfit_Algorithm (processors,
                                          consistency_level_);
}

