// -*- C++ -*-

//=============================================================================
/**
 *  @file    RankedWCRT.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <numeric>
#include "RankedWCRT.h"
#include "OptimizedWCRT.h"
#include "CTT_Enhanced.h"
#include "Combination_T.h"

typedef std::pair <Processor, Task> TASK_POSITION;
typedef std::vector <TASK_POSITION> TASK_POSITIONS;
typedef std::map <Taskname, TASK_POSITIONS> BACKUP_GROUPS;

struct TaskPositionPredicate : public std::binary_function <TASK_POSITION,
                                                            TASK_POSITION,
                                                            bool>
{
public:
  bool operator () (const TASK_POSITION & pos1,
                    const TASK_POSITION & pos2)
  {
    return pos1.second.rank < pos2.second.rank;
  }
};

struct TaskPositionToProcessorName : public std::unary_function <TASK_POSITION,
                                                                 Processor>
{
public:
  Processor operator () (const TASK_POSITION & pos)
  {
    return pos.first;
  }
};

RankedWCRT::RankedWCRT (const SCHEDULE & schedule,
                        unsigned int consistency_level)
  : ctt_ (new CTT_Enhanced ()),
    consistency_level_ (consistency_level)
{
  // construct failure map from schedule:

  // create groups of backups that belong together
  BACKUP_GROUPS backup_groups;
  Taskname group_id;

  // for each task in each processor
  for (SCHEDULE::const_iterator s_it = schedule.begin ();
       s_it != schedule.end ();
       ++s_it)
    {
      for (TASK_LIST::const_iterator t_it = s_it->second.begin ();
           t_it != s_it->second.end ();
           ++t_it)
        {
          // deterimine base name of the task
          group_id = 
            t_it->name.substr (0, 
                               t_it->name.find_first_of ('_'));

          // if group does not exist, create group
          if (backup_groups.find (group_id) == backup_groups.end ())
            backup_groups[group_id] = TASK_POSITIONS ();

          // enter TASK
          backup_groups[group_id].push_back (TASK_POSITION (s_it->first, 
                                                            *t_it));
        } // end for each task
    } // end for each processor

  // for each backup group
  for (BACKUP_GROUPS::iterator b_it = backup_groups.begin ();
       b_it != backup_groups.end ();
       ++b_it)
    {
      // sort each task group by its task ranks
      std::sort (b_it->second.begin (),
                 b_it->second.end (),
                 TaskPositionPredicate ());

      // for each task entry
      for (TASK_POSITIONS::iterator p_it = b_it->second.begin ();
           p_it != b_it->second.end ();
           ++p_it)
        {
          // we are only interested in backups here
          if (p_it->second.role == BACKUP)
            {
              // build the processor set by adding all processors with
              // lower rank
              PROCESSOR_SET relevant_processors;
              std::transform (b_it->second.begin (),
                              p_it,
                              std::inserter (relevant_processors,
                                             relevant_processors.begin ()),
                              TaskPositionToProcessorName ());

              // add the processor set to the failure map
              failure_map_ [p_it->second.name] = relevant_processors;
              TRACE (p_it->second.name << ":" << relevant_processors);
            } // end if role == BACKUP
        } // end for each task in the group
    } // end for each task group
}

struct UnionAccumulator : public std::unary_function <Task,
                                                      void>
{
public:
  UnionAccumulator (FAILURE_MAP & map,
                    PROCESSOR_SET & un)
    : map_ (map),
      union_ (un)
  {
  }

  void operator () (const Task & task)
  {
    PROCESSOR_SET failure_set = map_ [task.name];
    PROCESSOR_SET result;

    /*    
    std::set_union (failure_set.begin (),
                    failure_set.end (),
                    union_.begin (),
                    union_.end (),
                    std::inserter (result,
                                   result.begin ()));

                                   union_ = result;
    */

    union_.insert (failure_set.begin (), failure_set.end ());

    TRACE("union(" << task.name << ")=" << union_);
  }

  PROCESSOR_LIST get_union (void) const
  {
    PROCESSOR_LIST list;
   
    TRACE("union=" << union_);

    std::copy (union_.begin (),
               union_.end (),
               std::inserter (list,
                              list.begin ()));

    TRACE("list=" << list);

    return list;
  }

private:
  FAILURE_MAP & map_;
  PROCESSOR_SET & union_;
};

// helper functor that looks at a certain set of tasks and determines
// whether which tasks become active if the set of processors fails
struct GetActiveBackups : public std::unary_function <PROCESSOR_SET,
                                                      TASKNAME_SET>
{
public:
  GetActiveBackups (const TASK_LIST & tasks,
                    FAILURE_MAP & map)
    : tasks_ (tasks),
      map_ (map)
  {
  }

  TASKNAME_SET operator () (const PROCESSOR_SET & failed_processors)
  {
    TASKNAME_SET active_backups;

    if (failed_processors.empty ())
      return active_backups;

    // for each task in the list
    for (TASK_LIST::iterator it = tasks_.begin ();
         it != tasks_.end ();
         ++it)
      {
        if (it->role == BACKUP)
          {
            // if it is a subset of the failed processors
            if ((std::includes (failed_processors.begin (),
                               failed_processors.end (),
                               map_[it->name].begin (),
                               map_[it->name].end ())))
              {
                // add the task to the taskname set
                active_backups.insert (it->name);
              }            
          } // end if role == BACKUP
      } // end for each task

    // return the set
    return active_backups;
  }

private:
  TASK_LIST tasks_;
  FAILURE_MAP & map_;
};

double 
RankedWCRT::operator () (const TASK_LIST & scheduled_tasks)
{
  // do a union of all the relevant task sets in the failure map
  PROCESSOR_SET un;
  UnionAccumulator union_accumulator (failure_map_, un);

  std::for_each (scheduled_tasks.begin (),
                 scheduled_tasks.end (),
                 union_accumulator);

  // create all possible combinations with size of the number of
  // backups from the processor set union
  PROCESSOR_LIST failure_elements = union_accumulator.get_union ();

  PROCESSOR_LIST combination;

  unsigned int combination_length =
    std::min <unsigned int> (failure_elements.size (),
                             consistency_level_);

  PROCESSOR_LIST::iterator it = failure_elements.begin ();
  for (unsigned int c_index = 0; 
       c_index < combination_length; 
       ++c_index, ++it)
    {
      combination.push_back (*it);
    }

  std::list <PROCESSOR_SET> failure_sets;
  do
    {
      PROCESSOR_SET set;
      std::copy (combination.begin (),
                 combination.end (),
                 std::inserter (set,
                                set.begin ()));

      failure_sets.push_back (set);      
    }
  while (next_combination (failure_elements.begin (),
                           failure_elements.end (),
                           combination.begin (),
                           combination.end ()));

  // create a taskname set for each of the combinations, by looking
  // which tasks belong to which failed processor.
  GetActiveBackups get_active_backups (scheduled_tasks,
                                       failure_map_);

  std::list <TASKNAME_SET> failover_scenarios;
  
  std::transform (failure_sets.begin (),
                  failure_sets.end (),
                  std::inserter (failover_scenarios,
                                 failover_scenarios.begin ()),
                  get_active_backups);

  TRACE ("failover scenarios: ");
  for (std::list<TASKNAME_SET>::iterator it = failure_sets.begin ();
       it != failure_sets.end ();
       ++it)
    {
      TRACE ("(" << *it << ")");
    }

  // handle the non failure case
  if (failover_scenarios.empty ())
    failover_scenarios.push_back (TASKNAME_SET ());

  // Calculate the worst case response time for each of these sets and
  // take the maximum, using the OptimizedWCRT functor.
  OptimizedWCRT wcrt_algorithm (scheduled_tasks);

  double wcrt = std::accumulate (failover_scenarios.begin (),
                                 failover_scenarios.end (),
                                 -1.0, // this value is just for
                                 // initialization, and cannot
                                 // be 0 since this would mean
                                 // that there is an
                                 // unschedulable task
                                 wcrt_algorithm);

  // Return the maximum or 0 if there was a combination that did not
  // allow scheduling
  return wcrt;
}
