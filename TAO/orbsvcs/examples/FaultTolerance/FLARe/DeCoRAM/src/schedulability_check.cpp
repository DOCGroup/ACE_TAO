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
bool count_tasks = false;
bool utilization = false;
unsigned int consistency_level = 0;

typedef std::map <Processor, FailureAwareWCRT> WCRT_MAP;

class UtilizationProcessor : public std::binary_function <double,
                                                          Task,
                                                          double>
{
public:
  double operator () (double util, const Task & task)
  {
    double result = util;

    if (extract_rank (task.name) == 0)
      result += task.execution_time / task.period;

    return result;
  }
};

class PrimaryCounter : public std::binary_function <unsigned int,
                                                    Task,
                                                    unsigned int>
{
public:
  unsigned int operator () (unsigned int sum,
                            const Task & task)
  {
    unsigned int result = sum;

    if (extract_rank (task.name) == 0)
      ++result;

    return result;
  }
};

class TaskSorter : public std::binary_function <TASK_POSITION,
                                                TASK_POSITION,
                                                bool>
{
public:
  bool operator () (const TASK_POSITION & p1,
                    const TASK_POSITION & p2)
  {
    return (extract_rank (p1.second.name) < extract_rank (p2.second.name));
  }
};

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

        for (REPLICA_GROUPS::iterator rg_it = replica_groups_.begin ();
             rg_it != replica_groups_.end ();
             ++rg_it)
          {
            std::sort (rg_it->second.begin (),
                       rg_it->second.end (),
                       TaskSorter ());
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

    if (all_processors.size () > max_rank_)
      {
        PROCESSOR_LIST combination;
        PROCESSOR_LIST::iterator it = all_processors.begin ();
        for (unsigned int c_index = 0; 
             c_index < max_rank_;
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

                TRACE ("checking " << wcrt_it->first << " with " << set);

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
      }
    else 
      {
        DBG_OUT ("There are not enough processors for " << max_rank_ << " failures.");
        return 1;
      }
        
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
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("acf:hotu"));

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
    case 't':
      count_tasks = true;
      break;
    case 'u':
      utilization = true;
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
  else if (count_tasks)
    {
      for (SCHEDULE::iterator it = schedule.begin ();
           it != schedule.end ();
           ++it)
        {
          unsigned int primary_count =
            std::accumulate (it->second.begin (),
                             it->second.end (),
                             0,
                             PrimaryCounter ());

          unsigned int backup_count = it->second.size () - primary_count;

          std::cout << it->first << ": " 
                    << primary_count << " " 
                    << backup_count << std::endl;
        }
    }
  else if (utilization)
    {
      for (SCHEDULE::iterator it = schedule.begin ();
           it != schedule.end ();
           ++it)
        {
          TASK_LIST primaries;
          
          double primary_utilization =
            std::accumulate (it->second.begin (),
                             it->second.end (),
                             0.0,
                             UtilizationProcessor ());

          std::cout << it->first << ": " 
                    << primary_utilization << std::endl;
        }
      
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
