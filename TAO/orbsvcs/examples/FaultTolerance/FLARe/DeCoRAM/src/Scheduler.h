// -*- C++ -*-

//=============================================================================
/**
 *  @file    Scheduler.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "Schedule.h"

/// this map groups all tasks that belong to one replica group
typedef std::pair <Processor, Task> TASK_POSITION;
typedef std::vector <TASK_POSITION> TASK_POSITIONS;
typedef std::map <Taskname, TASK_POSITIONS> REPLICA_GROUPS;
typedef std::list<PROCESSOR_SET> PROCESSOR_SETS;
typedef std::list<TASK_LIST> TASK_SCENARIOS;

class Scheduler : public std::unary_function <Task, 
                                              ScheduleResult>
{
public:
  /// default ctor
  Scheduler (const PROCESSOR_LIST & processors,
             unsigned int max_failures);

  /// destructor
  virtual ~Scheduler (void);

  // returns where the task has been placed and which WCRT it has.
  // If the WCRT response time is 0, it is not schedulable.
  ScheduleResult operator () (const Task & task);

  // this needs to be implemented by each scheduling algorithm
  virtual double schedule_task (const Task & task, 
                                const Processor & processor) = 0;

  // getter method for the schedule
  SCHEDULE schedule () const;

protected:
  virtual void update_schedule (const Task & task,
                                const Processor & processor);

  void update_replica_groups (const Task & task,
                              const Processor & processor);

protected:
  SCHEDULE schedule_;
  unsigned int max_failures_;
  REPLICA_GROUPS replica_groups_;
};

/*** Helper classes and operators ***/

/**
 * @class ProcessorNameComparison 
 *
 * @brief Can be used with the accumulate algorithm to find out if a
 *        processor name belongs to a list of task positions
 */
class ProcessorNameComparison : public std::binary_function <
  bool,
  TASK_POSITIONS::value_type,
  bool>
{
public:
  ProcessorNameComparison (const Processor & p);

  bool operator () (bool equal, const TASK_POSITIONS::value_type & pos);
private:
  Processor p_;
};

/**
 * @class ProcessorPicker
 * 
 * @brief can be used within transform to extract the 
 *        processor names from a TASK_POSITIONS structure.
 */
class ProcessorPicker : public std::unary_function<REPLICA_GROUPS::value_type,
                                                   Processor>
{
public:
  Processor operator () (const TASK_POSITIONS::value_type & entry);
};

/**
 * @class ReplicaFinder
 *
 * @brief Functor that uses the replica group to determine the
 *        processors that need to fail in order for a given backup
 *        task to become active
 */
class ReplicaFinder : public std::unary_function <Task,
                                                  PROCESSOR_SET>
{
public:
  ReplicaFinder (const REPLICA_GROUPS & rep_groups);

  virtual ~ReplicaFinder (void);

  virtual PROCESSOR_SET operator () (const Task & task) const;

private:
  const REPLICA_GROUPS & rep_groups_;
  ProcessorPicker processor_picker_;
};

// streaming operators for used data structures
std::ostream & operator<< (std::ostream & ostr, 
                           const TASK_POSITION & tp);

std::ostream & operator<< (std::ostream & ostr, 
                           const TASK_POSITIONS & tps);

std::ostream & operator<< (std::ostream & ostr, 
                           const REPLICA_GROUPS & rg);

std::ostream & operator<< (std::ostream & ostr, 
                           const PROCESSOR_SETS & ps);

std::ostream & operator<< (std::ostream & ostr, 
                           const TASK_SCENARIOS & ts);

#endif /* SCHEDULER_H_ */
