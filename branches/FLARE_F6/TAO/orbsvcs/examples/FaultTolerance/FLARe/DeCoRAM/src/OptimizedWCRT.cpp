// -*- C++ -*-

//=============================================================================
/**
 *  @file    OptimizedWCRT.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include "OptimizedWCRT.h"
#include "CTT_Enhanced.h"

/**
 *  @struct TaskRoleModifier
 *  
 *  @brief copies a task and changes it's role to Primary if it can
 *         find it in it's set of active backup tasks.  
 */
struct TaskRoleModifier : public std::unary_function <Task, Task> 
{
public:
  TaskRoleModifier (const TASKNAME_SET & active_backups)
    : active_backups_ (active_backups)
  {
  }

  Task operator () (const Task & task)
  {
    Task copy = task;
    if (active_backups_.count (copy.name) == 1)
      {
        copy.role = PRIMARY;
      }

    return copy;
  }

private:
  const TASKNAME_SET & active_backups_;
};

OptimizedWCRT::OptimizedWCRT (const TASK_LIST & tasks)
  : tasks_ (tasks),
    ctt_ (new CTT_Enhanced ())
{
  TRACE ("(" << tasks << ")");
}

double
OptimizedWCRT::operator () (double current_wcrt,
                           const TASKNAME_SET & active_backups)
{
  TRACE ("(" << current_wcrt << ", " << active_backups << ")");

  if (current_wcrt != .0)
    {
      // create modified task list where all backups that are not
      // active get treated as primaries
      TASK_LIST modified_tasks;

      std::cout << "Original: " << tasks_ << std::endl
                << "Active: " << active_backups << std::endl;
      
      std::transform (tasks_.begin (),
                      tasks_.end (),
                      std::inserter (modified_tasks,
                                     modified_tasks.begin ()),
                      TaskRoleModifier (active_backups));

      // calculate wcrt using CTT algorithm and 
      // compare it to the existing on
      double wcrt = (*ctt_) (modified_tasks);

      TRACE ("WCRT: " << modified_tasks << " = " << wcrt);

      // if we found a schedulable result return maximum wcrt
      if (wcrt != .0)
        return std::max <double> (current_wcrt,
                                  wcrt);
    }

  // if we found any unschedulable result on the way, keep this and
  // return 0, which means, it is unschedulable
  return .0;
}
