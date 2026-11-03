/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Periodic_Task.h
 *
 *  Base class for Periodic Tasks
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================
#ifndef PERIODIC_TASK_H
#define PERIODIC_TASK_H

#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "ace/Task.h"
#include "ace/SString.h"
#include "ace/Arg_Shifter.h"
#include "JobC.h"
#include "Task_Stats.h"

#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Barrier;
ACE_END_VERSIONED_NAMESPACE_DECL

/**
 * @class Periodic_Task
 *
 * @brief Periodic_Task executes jobs.
 */
class Periodic_Task : public ACE_Task <ACE_SYNCH>
{
 public:
  Periodic_Task () = default;
  ~Periodic_Task () override = default;

  Periodic_Task (Periodic_Task const &) = delete;
  Periodic_Task (Periodic_Task &&) = delete;
  Periodic_Task &operator= (Periodic_Task const &) = delete;
  Periodic_Task &operator= (Periodic_Task &&) = delete;

  /// Init the state of this object.
  int init_task (ACE_Arg_Shifter& arg_shifter);

  /// Activate this task, synch on the given barrier.
  virtual int activate_task (ACE_Barrier *barrier,
                             RTCORBA::PriorityMapping *priority_mapping) = 0;

  /// Dump the stats collected.
  void dump_stats (ACE_TCHAR const *msg);

  /// = Job get/set
  /// Returns the name of the Job exec'ed by this Task.
  char const *job () const;

  /// Sets the Job to exec.
  void job (Job_ptr job);

 protected:
  /// All tasks synch at this barrier.
  ACE_Barrier *barrier_ {};

  /// The Job to execute.
  Job_var job_;

  /// Name of the Job.
  ACE_CString name_;

  /// = Task parameters
  /// see http://www.cis.ksu.edu/~neilsen/classes/cis721/lectures/lecture2/sld009.htm

  /// The priority of this task.
  RTCORBA::Priority task_priority_ {};

  /// Period
  unsigned long period_ {};

  /// Worst case exec. time.
  unsigned long exec_time_ {};

  /// Phase
  long phase_ {};

  /// Number of times to exec. Job
  int iter_ {};

  /// A load factor supplied to each Job.
  int load_ {};

  /// = Stats house keeping
  std::unique_ptr<Task_Stats> task_stats_;
};

#endif /* PERIODIC_TASK_H */
