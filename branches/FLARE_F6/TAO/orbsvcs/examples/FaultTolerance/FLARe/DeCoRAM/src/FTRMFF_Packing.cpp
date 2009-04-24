// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Packing.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <sstream>
#include "CTT_Enhanced.h"
#include "FTRMFF_Packing.h"
#include "FTRMFF_Primary.h"

FTRMFF_Packing::~FTRMFF_Packing ()
{
}

FTRMFF_Output
FTRMFF_Packing::operator () (const FTRMFF_Input & input)
{
  FTRMFF_Packing_Algorithm algorithm (input.processors,
                                      input.backup_count);

  FTRMFF_Output output;
  output.schedule = algorithm (input.tasks);
  output.unscheduled_tasks = algorithm.get_unschedulable ();

  DBG_OUT (algorithm.schedule ());

  return output;
}

FTRMFF_Packing_Algorithm::FTRMFF_Packing_Algorithm (
  const PROCESSOR_LIST & processors,
  unsigned int consistency_level)
  : schedule_ (create_schedule (processors)),
    consistency_level_ (consistency_level)
{
}

FTRMFF_Packing_Algorithm::~FTRMFF_Packing_Algorithm ()
{
}

class BackupModifier : public std::unary_function <Task,
                                                   Task>
{
public:
  BackupModifier (unsigned long rank)
    : rank_ (rank)
  {
  }
  
  Task operator () (const Task & task)
  {
    Task backup = task;
    backup.role =  BACKUP;

    std::stringstream ss;
    ss << task.name << "_" << rank_;
    backup.name = ss.str ();

    return backup;
  }

private:
  unsigned long rank_;
};

class EmptyTask
{
};

bool operator== (const SCHEDULE::value_type & entry, 
                 const EmptyTask & /*task*/)
{
  return entry.second.empty ();
}

void 
FTRMFF_Packing_Algorithm::copy_tasks (unsigned long primary_end,
                                      unsigned long number)
{
  // find iterator that points to the last primary processor
  SCHEDULE::iterator end = schedule_.begin ();
  for (unsigned long count = 0;
       count < primary_end;
       ++count,++end);

  // copy primary processors 'number' times
  for (unsigned long i = 0; i < number; ++i)
    {
      // find first empty processor
      SCHEDULE::iterator start = std::find (schedule_.begin (),
                                            schedule_.end (),
                                            EmptyTask ());

      // if there is enough space available
      if (start != schedule_.end ())
        {
          SCHEDULE::iterator copy = start;

          // for each primary processor
          for (SCHEDULE::iterator it = schedule_.begin ();
               it != end;
               ++it)
            {
              // copy tasks and make them backups
              std::transform (it->second.begin (),
                              it->second.end (),
                              std::inserter (copy->second,
                                             copy->second.begin ()),
                              BackupModifier (i+1));
              ++copy;
            }
        }
    }
}

unsigned long 
rank (unsigned long primary_processors, 
      unsigned long processor)
{
  return processor % primary_processors;
}

unsigned long
advance_replica_processor (unsigned long processors,
                           unsigned long primaries,
                           unsigned long current,
                           unsigned long ignore = -1)
{
  unsigned long result = current;
  do
    {
      if (result < primaries)
        {
          result = processors - 1;
        }
      else
        {
          result = result - primaries;

          if (result < primaries)
            if (result == 0)
              result = processors - 1;
            else
              result = processors - 1 - primaries + result;
        }
    }  
  while (result % primaries == ignore);

  return result;
}

void
FTRMFF_Packing_Algorithm::merge_processors (unsigned long primary_processors)
{ 
  // keep a vector of markers, for which processors have been
  // processed already
  std::vector <bool> merge_marker (processor_usage (schedule_));
  std::fill (merge_marker.begin (),
             merge_marker.end (),
             false);

  // keep a vector of processors
  std::vector <Processor> merge_matrix;
  PROCESSOR_LIST processors = get_processors (schedule_, true);
  std::copy (processors.begin (),
             processors.end (),
             std::inserter (merge_matrix,
                            merge_matrix.begin ()));

  unsigned long current_processor_start = 
    advance_replica_processor (merge_matrix.size (),
                               primary_processors,
                               0);
  unsigned long current_processor = current_processor_start;

  do
    {
      DBG_OUT (merge_matrix[current_processor]);

      if (!merge_marker[current_processor])
        {
          merge_marker[current_processor] = true;

          // determine start position of second processor index
          unsigned long match_processor_start = current_processor - 1;
      
          if ((match_processor_start / primary_processors) < 
              (current_processor / primary_processors))
            match_processor_start += primary_processors;

          unsigned long match_processor = match_processor_start;

          do
            {
              if (!merge_marker[match_processor])
                {
                  // check if merge is allowed with respect to the 
                  // number of tolerated failures
                  if (rank (primary_processors, current_processor) +
                      rank (primary_processors, match_processor) >
                      consistency_level_)
                    {
                      TASK_LISTS all_tasks;
                      all_tasks.push_back (
                        schedule_[merge_matrix[current_processor]]);
                      all_tasks.push_back (
                        schedule_[merge_matrix[match_processor]]);

                      // if schedulable no matter which tasks become active
                      if (merge_check (all_tasks))
                        {
                          DBG_OUT ("merge " 
                                   << merge_matrix[current_processor]
                                   << ": " 
                                   << schedule_[merge_matrix[current_processor]]
                                   << " with " 
                                   << merge_matrix[match_processor]
                                   << ": "
                                   << schedule_[merge_matrix[match_processor]]);

                          schedule_[merge_matrix[match_processor]] =
                            merge_lists (all_tasks);

                          schedule_.erase (merge_matrix[current_processor]);

                          merge_marker[match_processor] = true;
                          break;
                        }
                    }
                  else
                    {
                      DBG_OUT ("cannot merge" << merge_matrix[current_processor]
                               << " with " << merge_matrix[match_processor]);
                    }
                }

              match_processor = 
                advance_replica_processor (merge_matrix.size (),
                                           primary_processors,
                                           match_processor,
                                           current_processor % primary_processors);
              
            } while (match_processor_start != match_processor);
        }
      
      current_processor = advance_replica_processor (merge_matrix.size (),
                                                     primary_processors,
                                                     current_processor);
    }  while (current_processor != current_processor_start);
}

SCHEDULING_MAP
FTRMFF_Packing_Algorithm::operator () (const TASK_LIST & tasks)
{
  // step one, only schedule primaries to get lower bound
  FTRMFF_Primary_Algorithm primary_ftrmff (get_processors (schedule_));

  primary_ftrmff (tasks);

  schedule_ = primary_ftrmff.schedule ();

  unsigned long primary_processors = processor_usage (schedule_);

  // check wether there are enough tasks    
  unsigned long needed_tasks = primary_processors * \
                               (consistency_level_ + 1);

  if ((schedule_.size () < needed_tasks) || 
      (!primary_ftrmff.get_unschedulable ().empty ()))
    {
      std::cerr << schedule_.size () << " tasks are not enough. We need " 
                << needed_tasks << "." << std::endl;
      return transform_schedule (schedule_);
    }

  this->copy_tasks (primary_processors,
                    consistency_level_);
 
  this->merge_processors (primary_processors);
 
  return transform_schedule (schedule_);
}

SCHEDULE_PROGRESS_LIST
FTRMFF_Packing_Algorithm::get_unschedulable ()
{
  return unschedulable_;
}

const SCHEDULE & 
FTRMFF_Packing_Algorithm::schedule () const
{
  return schedule_;
}

class PrimaryConversion : public std::unary_function<Task,
                                                     Task>
{
public:
  Task operator () (const Task & task)
  {
    Task t = task;
    t.role = PRIMARY;

    return t;
  }
};

bool
FTRMFF_Packing_Algorithm::merge_check (const TASK_LISTS & lists)
{
  bool schedulable = true;
  CTT_Enhanced ctt;

  for (size_t i = 0;
       i < lists.size ();
       ++i)
    {
      TASK_LISTS tl = lists;
      TASK_LIST active_tasks;
      
      std::transform (tl[i].begin (),
                      tl[i].end (),
                      std::inserter (active_tasks,
                                     active_tasks.begin ()),
                      PrimaryConversion ());

      tl[i] = active_tasks;

      if (.0 >= ctt (this->merge_lists (tl)))
        {
          schedulable = false;
          break;
        }
    }

  return schedulable;
}

TASK_LIST
FTRMFF_Packing_Algorithm::merge_lists (const TASK_LISTS & lists)
{
  TASK_LIST result;

  // merge all lists
  for (TASK_LISTS::const_iterator it = lists.begin ();
       it != lists.end ();
       ++it)
    {
      std::copy (it->begin (),
                 it->end (),
                 std::inserter (result,
                                result.begin ()));
    }

  // sort list by period
  std::sort (result.begin (),
             result.end (),
             PeriodComparison<Task> ());

  return result;
}
