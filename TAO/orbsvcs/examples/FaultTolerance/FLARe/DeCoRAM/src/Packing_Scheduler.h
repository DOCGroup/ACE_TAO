// -*- C++ -*-

//=============================================================================
/**
 *  @file    Packing_Scheduler.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef PACKING_SCHEDULER_H_
#define PACKING_SCHEDULER_H_

#include "Scheduler.h"

typedef std::vector<TASK_LIST> TASK_LISTS;
typedef std::map <Processor, TASK_LISTS> TASK_GROUPS;

std::ostream & operator<< (std::ostream & ostr, 
                           const TASK_LISTS & tl);

std::ostream & operator<< (std::ostream & ostr, 
                           const TASK_GROUPS & tg);

struct Packing_Scheduler : public Scheduler
{
public:
  /// default ctor
  Packing_Scheduler (const PROCESSOR_LIST & processors,
                     unsigned int max_failures);

  virtual double schedule_task (const Task & task,
                                const Processor & processor);

protected:
  virtual void update_schedule (const ScheduleResult & result);

private:
  void update_task_groups (const ScheduleResult & result);

  void add_backup (const Task & task,
                   TASK_LISTS & tg);

  bool rank_check (const TASK_LISTS & list);

  unsigned int paired_rank_check (const TASK_LIST & list1,
                                  const TASK_LIST & list2);

  bool merge_check (const TASK_LISTS & lists);

  TASK_LIST merge_lists (const TASK_LISTS & lists);

private:
  TASK_GROUPS task_groups_;
};

#endif /* PACKING_SCHEDULER_H_ */
