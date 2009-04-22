// -*- C++ -*-

//=============================================================================
/**
 *  @file    Schedule.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include <set>
#include "Algorithms.h"

typedef std::set <Processor> PROCESSOR_SET;
typedef std::set <Taskname> TASKNAME_SET;

std::ostream & operator<< (std::ostream & ostr, const TASKNAME_SET & s);

// the following data structures are used within FTRMFF to keep track
// of temporary task placement results

typedef TASK_LIST Schedule_Entry;
typedef std::map <Processor, Schedule_Entry> SCHEDULE;

std::ostream & operator<< (std::ostream & ostr, const SCHEDULE & s);

SCHEDULE read_schedule (std::istream & istr);

SCHEDULE create_schedule (const PROCESSOR_LIST & processors);

PROCESSOR_LIST get_processors (const SCHEDULE & schedule);

unsigned long processor_usage (const SCHEDULE & schedule);

struct ScheduleResult
{
  Task task;
  Processor processor;
  double wcrt;
};

typedef std::vector <ScheduleResult> SCHEDULE_RESULT_LIST;

/// This abtract base class represents a class of algorithms that will
/// determine the rank of scheduled replicas
/// @return the number of backups that could actually be ranked
class Ranking_Algorithm : public std::binary_function <SCHEDULE_RESULT_LIST,
                                                       SCHEDULE, 
                                                       unsigned long>
{
 public:
  virtual ~Ranking_Algorithm ();

  virtual unsigned long operator () (SCHEDULE_RESULT_LIST & result_list,
                                     const SCHEDULE & schedule) = 0;
};

/// @param schedule in form of Processor -> TASK_LIST
/// @return schedule in the form Task -> Processor
SCHEDULING_MAP transform_schedule (const SCHEDULE & schedule);

/// enters a list of tasks into the schedule according to its
/// ScheduleResult entry.
void add_schedule_results (const SCHEDULE_RESULT_LIST & result_list,
                           SCHEDULE & schedule);

/// simple helper functions that creates the correct number of backup
/// tasks from a single task template
TASK_LIST create_tasks (const Task & task,
                        unsigned int backup_number);

struct WCRTComparison : public std::binary_function <ScheduleResult,
                                                     ScheduleResult,
                                                     bool>
{
  bool operator () (const ScheduleResult & r1,
                    const ScheduleResult & r2);
};

std::ostream & operator<< (std::ostream & ostr, const ScheduleResult & r);

#endif /* SCHEDULE_H_ */
