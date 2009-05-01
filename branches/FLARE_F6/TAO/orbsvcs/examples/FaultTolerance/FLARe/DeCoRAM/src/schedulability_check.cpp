// -*- C++ -*-

//=============================================================================
/**
 *  @file    schedulability_check.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <fstream>
#include <sstream>
#include <iostream>
#include <numeric>
#include "Scheduler.h"
#include "CTT_Enhanced.h"
#include "CTT_Basic.h"
#include "FailureAwareWCRT.h"
#include "Combination_T.h"
#include <ace/Get_Opt.h>

std::string filename = "test.sd"; // filename of task list input
bool counting_mode = false;
bool average_mode = false;
bool check_overbooking = false;
unsigned int consistency_level = 0;

typedef std::map <Processor, FailureAwareWCRT> WCRT_MAP;

class ScheduleChecker
{
public:
  ScheduleChecker (const SCHEDULE & schedule)
    : schedule_ (schedule),
      replica_finder_ (replica_groups_)
  {
    max_rank_ = this->max_rank ();

    // build replica_groups data structure
    for (SCHEDULE::const_iterator sched_it = schedule.begin ();
         sched_it != schedule.end ();
         ++sched_it)
      {
        Processor processor = sched_it->first;
        for (TASK_LIST::const_iterator task_it = sched_it->second.begin ();
             task_it != sched_it->second.end ();
             ++task_it)
          {
            REPLICA_GROUPS::iterator rep_it = 
              replica_groups_.find (primary_name (*task_it));

            TASK_POSITION position (processor, *task_it);
            if (rep_it == replica_groups_.end ())
              {
                TASK_POSITIONS entry;
                entry.push_back (position);
                replica_groups_[primary_name (*task_it)] = entry;
              }
            else
              {
                replica_groups_[
                  primary_name (*task_it)].push_back (position);
              }
          }

        // fill wcrt_map
        wcrt_map_.insert (std::pair <Processor, FailureAwareWCRT> (
                             processor, 
                             FailureAwareWCRT (sched_it->second,
                                               replica_finder_)));
      }


    
  }

  int check_schedulability (void)
  {
    PROCESSOR_LIST all_processors = get_processors (schedule_, true);

    unsigned int tupel_size = std::min (max_rank_,
                                        (unsigned int) all_processors.size ());

    PROCESSOR_LIST combination;
    PROCESSOR_LIST::iterator it = all_processors.begin ();
    for (unsigned int c_index = 0; 
         c_index < tupel_size; 
         ++c_index, ++it)
      {
        combination.push_back (*it);
      }

    do
      {
        PROCESSOR_SET set;
        std::copy (combination.begin (),
                   combination.end (),
                   std::inserter (set,
                                  set.begin ()));
        
        // check combination against each task;
        for (WCRT_MAP::iterator wcrt_it = wcrt_map_.begin ();
             wcrt_it != wcrt_map_.end ();
             ++wcrt_it)
          {
            // ignore failure sets that contain the own processor
            if (set.find (wcrt_it->first) != set.end ())              
              continue;

            DBG_OUT ("checking " << wcrt_it->first << " with " << set);

            double wcrt = (wcrt_it->second) (-1.0, set);
            if (!(wcrt > .0))
              {
                DBG_OUT (wcrt_it->first << " not schedulable with " << set << ": " << wcrt);
                return 1;
              }
          }
      }
    while (next_combination (all_processors.begin (),
                             all_processors.end (),
                             combination.begin (),
                             combination.end ()));

    return 0;
  }

private:
  unsigned int max_rank (void)
  {
    unsigned int max_rank = 0;

    for (SCHEDULE::const_iterator sched_it = schedule_.begin ();
         sched_it != schedule_.end ();
         ++sched_it)
      {
        for (TASK_LIST::const_iterator task_it = sched_it->second.begin ();
             task_it != sched_it->second.end ();
             ++task_it)
          {
            max_rank = std::max (max_rank,
                                 extract_rank (task_it->name));
          }
      }

    return max_rank;
  }

private:
  const SCHEDULE & schedule_;
  REPLICA_GROUPS replica_groups_;
  ReplicaFinder replica_finder_;
  WCRT_MAP wcrt_map_;
  unsigned int max_rank_;
};

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("acf:ho"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'f':
      filename = get_opt.opt_arg ();
      break;
    case 'c':
      counting_mode = true;
      break;
    case 'a':
      average_mode = true;
      break;
    case 'o':
      check_overbooking = true;
      break;
    case 'h':
    default:
      std::cerr << "usage: scheck "
                << "\t-a" << std::endl
                << "\t-c" << std::endl
                << "\t-f <filename>" << std::endl;
      return 1;
  }

  return 0;
}


int main (int argc, char *argv[])
{
  if (parse_args (argc, argv) != 0)
    return 1;

  std::ifstream ifile;
  ifile.open (filename.c_str ());

  SCHEDULE schedule = read_schedule (ifile);
  
  ifile.close ();

  if (counting_mode)
    {
      std::cout << processor_usage (schedule) << std::endl;
    }
  else if (average_mode)
    {
      for (SCHEDULE::iterator it = schedule.begin ();
           it != schedule.end ();
           ++it)
        {
          if (it->first.compare ("Unknown") != 0)
            std::cout << it->first << ":" << std::endl;

          std::list <double> utilization;

          for (TASK_LIST::iterator t_it = it->second.begin ();
               t_it != it->second.end ();
               ++t_it)
            {
              utilization.push_back (t_it->execution_time / t_it->period);
            }

          std::cout << "minimum utilization = " 
                    << *std::min_element (utilization.begin (),
                                          utilization.end ()) * 100 << " %"
                    << std::endl;

          std::cout << "maximum utilization = " 
                    << *std::max_element (utilization.begin (),
                                          utilization.end ()) * 100 << " %"
                    << std::endl;

          std::cout << "average utilization = " 
                    << (std::accumulate (utilization.begin (),
                                         utilization.end (),
                                         0.0) / utilization.size ()) * 100 << " %"
                    << std::endl;
          
        }
    }
  else
    {
      unsigned long usage = processor_usage (schedule);

      SCHEDULE min_schedule;
      SCHEDULE::const_iterator schedule_it = schedule.begin ();
      for (unsigned long i = 0; i < usage; ++i, ++schedule_it)
        {
          min_schedule.insert (*schedule_it);
        }

      ScheduleChecker checker (min_schedule);
           
      return checker.check_schedulability ();
    } // end else counting_mode not true

  return 0;
}
