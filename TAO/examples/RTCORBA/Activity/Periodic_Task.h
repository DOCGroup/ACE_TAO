/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Periodic_Task.h
 *
 *  $Id$
 *
 *  Base class for Periodic Tasks
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================
#ifndef PERIODIC_TASK_H
#define PERIODIC_TASK_H

#include "tao/RTCORBA/RTCORBA.h"
#include "ace/Task.h"
#include "JobC.h"

class ACE_Barrier;
class ACE_Arg_Shifter;
class Task_Stats;

/**
 * @class Periodic_Task
 *
 * @brief Periodic_Task executes jobs.
 *
 */
class Periodic_Task : public ACE_Task <ACE_SYNCH>
{
 public:
  /// = Initialization and termination code.
  Periodic_Task (void);
  ~Periodic_Task ();

  /// Init the state of this object.
  int init_task (ACE_Arg_Shifter& arg_shifter);

  /// Activate this task, synch on the given barrier.
  virtual int activate_task (ACE_Barrier* barrier) = 0;

  /// Dump the stats collected.
  void dump_stats (ACE_TCHAR* msg);

  /// = Job get/set
  /// Returns the name of the Job exec'ed by this Task.
  const char* job (void);

  /// Sets the Job to exec.
  void job (Job_ptr job);

 protected:
  /// All tasks synch at this barrier.
  ACE_Barrier* barrier_;

  /// The Job to execute.
  Job_var job_;

  /// Name of the Job.
  ACE_CString name_;

  /// = Task parameters
  /// see http://www.cis.ksu.edu/~neilsen/classes/cis721/lectures/lecture2/sld009.htm

  /// The priority of this task.
  RTCORBA::Priority task_priority_;

  /// Period
  unsigned long period_;

  /// Worst case exec. time.
  unsigned long exec_time_;

  /// Phase
  long phase_;

  /// Number of times to exec. Job
  int iter_;

  /// A load factor supplied to each Job.
  int load_;

  /// = Stats house keeping
  Task_Stats* task_stats_;
};

#endif /* PERIODIC_TASK_H */
