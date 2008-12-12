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

struct ScheduleResult
{
  Task task;
  Processor processor;
  double wcrt;
};

typedef std::vector <ScheduleResult> SCHEDULE_RESULT_LIST;

/// This method will order the result_list based on the
/// worst-case response times of the entries
/// @return the number of backups that could actually be ranked
unsigned int rank_backups (SCHEDULE_RESULT_LIST & result_list);

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

#endif /* SCHEDULE_H_ */
