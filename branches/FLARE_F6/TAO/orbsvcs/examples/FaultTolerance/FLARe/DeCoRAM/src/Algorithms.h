// -*- C++ -*-

//=============================================================================
/**
 *  @file    Algorithms.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_ALGORITHMS_H_
#define FTRMFF_ALGORITHMS_H_

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <functional>

#ifdef DO_TRACE
#  define TRACE(X) std::cout << __PRETTY_FUNCTION__ << ": " << X << std::endl
#else
#  define TRACE(X)
#endif

#ifdef DO_DEBUG
#  define DBG_OUT(X) std::cout << X << std::endl
#else
#  define DBG_OUT(X)
#endif

// Algorithm 1 : Completion Time Test

// The following data structures and the CTT_Algorithm represent the
// external interface for a Completion Time Test. This algorithm
// determines whether a given set of tasks is schedulable on a
// processor, based on rate monotonic scheduling.

typedef std::string Taskname;

enum TaskRole
  {
    PRIMARY,
    BACKUP
  };

struct Task
{
  Taskname name;
  double execution_time;
  double period;
  double sync_time;
  TaskRole role;
  int rank;
};

typedef std::vector <Task> TASK_LIST;

class CTT_Algorithm : public std::unary_function <TASK_LIST, bool>
{
 public:
  virtual ~CTT_Algorithm ();

  virtual double operator () (const TASK_LIST & tasks) = 0;
};

// Algorithm 2 : Rate Monotonic First Fit Scheduling

// The following data structures and the FTRMFF_Algorithm class
// represent the external interface of a RMFF algorithm that creates a
// schedule (association of tasks to processors) based on a first fit
// placement. This algorithm uses CTT internally.

typedef std::string Processor;
typedef std::list <Processor> PROCESSOR_LIST;
typedef unsigned long Priority;

PROCESSOR_LIST create_processors (unsigned long count);

struct TaskPlacement
{
  Processor processor;
  Priority  priority;
};

typedef std::map <Taskname, TaskPlacement> SCHEDULING_MAP;

struct FTRMFF_Input
{
  TASK_LIST tasks;
  PROCESSOR_LIST processors;
  unsigned int backup_count;
};

struct ScheduleProgress
{
  Task task;
  unsigned int scheduled_replicas;
};

typedef std::list <ScheduleProgress> SCHEDULE_PROGRESS_LIST;

struct FTRMFF_Output
{
  // resulting schedule from algorithm
  SCHEDULING_MAP schedule;
  // this entry contains tasks that could not be scheduled
  SCHEDULE_PROGRESS_LIST unscheduled_tasks; 
};

class FTRMFF_Algorithm : public std::unary_function <FTRMFF_Input,
                                                     FTRMFF_Output>
{
 public:
  virtual ~FTRMFF_Algorithm ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input) = 0;
};

// Helper functions for input and output for algorithm input and
// results, and for sorting and comparison of tasks.

std::ostream & operator<< (std::ostream & ostr, const Task & t);

std::ostream & operator<< (std::ostream & ostr, const TASK_LIST & t);

std::ostream & operator<< (std::ostream & ostr, const PROCESSOR_LIST & p);

std::ostream & operator<< (std::ostream & ostr, 
                           const SCHEDULE_PROGRESS_LIST & pl);

struct string_to_task : std::unary_function <std::string, Task>
{
  Task operator () (const std::string & s);
};

/**
 *  @struct TaskNamePredicate
 *
 *  @brief  Predicate functor for task name comparison
 */
struct TaskNamePredicate : public std::unary_function <Task,
                                                       bool>
{
public:
  TaskNamePredicate (const std::string & task_name);

  bool operator () (const Task & task);

private:
  std::string task_name_;
};

template <typename TASK>
struct PeriodComparison : public std::binary_function <TASK, 
                                                       TASK, 
                                                       bool>
{
  bool operator () (const TASK & t1, 
                    const TASK & t2)
  {
    return (t1.period < t2.period);
  }
};

struct double_equal : public std::binary_function <double, double, bool>
{
  bool operator () (double d1, double d2);
};

Taskname primary_name (const Task & task);

#endif /* FTRMFF_ALGORITHMS_H_ */
