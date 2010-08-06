// -*- C++ -*-

//=============================================================================
/**
 *  @file    Schedule.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <sstream>
#include "Schedule.h"

SCHEDULE remove_empty_processors (const SCHEDULE & schedule)
{
  SCHEDULE new_schedule;

  // use a new list of processor names to assign the scheduled entries
  PROCESSOR_LIST new_processors = create_processors (schedule.size ());
  PROCESSOR_LIST::const_iterator next_processor = new_processors.begin ();

  for (SCHEDULE::const_iterator it = schedule.begin ();
       it != schedule.end ();
       ++it)
    {
      // only copy the entry into the new schedule if the processor
      // contains tasks
      if (!it->second.empty ())
        {
          // use a new processor name as key
          new_schedule[*next_processor] = it->second;
          ++next_processor;
        }
    }

  return new_schedule;
}

SCHEDULING_MAP 
transform_schedule (const SCHEDULE & schedule)
{
  SCHEDULING_MAP output;
  TaskPlacement p;
  p.priority = 0;

  for (SCHEDULE::const_iterator s_it = schedule.begin ();
       s_it != schedule.end ();
       ++s_it)
    {
      p.priority = 0;

      for (TASK_LIST::const_iterator t_it = s_it->second.begin ();
           t_it != s_it->second.end ();
           ++t_it)
        {
          ++(p.priority);
          p.processor = s_it->first;
          output[t_it->name] = p;
        }
    }

  //DBG_OUT (schedule);

  return output;
}

void
add_schedule_results (
  const SCHEDULE_RESULT_LIST & result_list,
  SCHEDULE & schedule)
{
  for (SCHEDULE_RESULT_LIST::const_iterator r_it = 
         result_list.begin ();
       r_it != result_list.end ();
       ++r_it)
    {
      schedule[r_it->processor].push_back (r_it->task);
    }
}

TASK_LIST
create_tasks (const Task & task,
              unsigned int backup_number)
{
  TASK_LIST output;
  Task t = task;
  t.role = PRIMARY;
  t.rank = 0;

  // add a primary and the appropriate number of backup tasks
  for (unsigned int i = 0; 
       i <= backup_number;
       ++i)
    {
      output.push_back (t);
    }

  return output;
}

TASK_LIST create_ranked_tasks (const Task & task,
                               unsigned int backup_number)
{
  TASK_LIST tasks;

  for (unsigned int i = 0; i <= backup_number; ++i)
    {
      Task t = task;
      t.rank = i;
      if (t.rank > 0)
        {
          t.role = BACKUP;

          std::stringstream ss;
          ss << t.name << "_" << i;
          t.name = ss.str ();
        }
      else
        {
          t.role = PRIMARY;
        }

      tasks.push_back (t);
    }

  return tasks;
}

bool
WCRTComparison::operator () (const ScheduleResult & r1,
                             const ScheduleResult & r2)
{
  return (r1.wcrt < r2.wcrt);
}


std::ostream & operator<< (std::ostream & ostr, const TASKNAME_SET & s)
{
  if (s.empty ())
    ostr << "empty";

  for (TASKNAME_SET::const_iterator it = s.begin ();
       it != s.end ();
       ++it)
    {
      ostr << *it << " ";
    }

  return ostr;
}

std::ostream & operator<< (std::ostream & ostr, const SCHEDULE & s)
{
  for (SCHEDULE::const_iterator it = s.begin ();
       it != s.end ();
       ++it)
    {
      if (it != s.begin ())
        ostr << std::endl;

      ostr << "<" << it->first << ">";
      
      for (TASK_LIST::const_iterator t_it = it->second.begin ();
           t_it != it->second.end ();
           ++t_it)
        {
          ostr << std::endl << *t_it;
        }
    }

  return ostr;
}

SCHEDULE read_schedule (std::istream & istr)
{
  std::string input;
  string_to_task task_reader;
  SCHEDULE s;
  Processor p = "Unknown";
  while (!istr.eof ())
    {
      std::getline (istr, input);

      if (input.empty ())
        continue;

      if (input.find ("<") != std::string::npos)
        {
          p = input.substr (input.find_first_of ('<') + 1,
                            input.find_first_of ('>') - 1);
        }
      else
          s[p].push_back (task_reader (input));
    }
  
  return s;
}

SCHEDULE create_schedule (const PROCESSOR_LIST & processors)
{
  SCHEDULE result;

  for (PROCESSOR_LIST::const_iterator it = processors.begin ();
       it != processors.end ();
       ++it)
    {
      result[*it] = Schedule_Entry ();
    }

  return result;
}

PROCESSOR_LIST 
get_processors (const SCHEDULE & schedule,
                bool only_utilized)
{
  PROCESSOR_LIST processors;

  for (SCHEDULE::const_iterator it = schedule.begin ();
       it != schedule.end ();
       ++it)
    {
      if (!(it->second.empty () && only_utilized))
        processors.push_back (it->first);
    }

  return processors;
}

unsigned long 
processor_usage (const SCHEDULE & schedule)
{
  unsigned long count = 0;
  
  for (SCHEDULE::const_iterator it = schedule.begin ();
       it != schedule.end ();
       ++it)
    {
      if (it->second.size () > 0)
        count++;
    }

  return count;
}

Ranking_Algorithm::~Ranking_Algorithm ()
{
}

std::ostream & 
operator<< (std::ostream & ostr, const ScheduleResult & r)
{
  ostr << "<" << r.task << "|" << r.processor 
       << "|" << r.wcrt << ">";

  return ostr;
}

FTRMFF_Algorithm_Impl::~FTRMFF_Algorithm_Impl (void)
{
}

FTRMFF_Algorithm_Impl::FTRMFF_Algorithm_Impl (unsigned int consistency_level)
  : consistency_level_ (consistency_level)
{
}

const SCHEDULE_PROGRESS_LIST &
FTRMFF_Algorithm_Impl::get_unschedulable () const
{
  return unschedulable_;
}
