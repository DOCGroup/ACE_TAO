// -*- C++ -*-

//=============================================================================
/**
 *  @file    CTT_Enhanced.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef CTT_ENHANCED_H_
#define CTT_ENHANCED_H_

#include "Algorithms.h"

/***
 * @class CTT_Enhanced
 *
 * @brief This class implements an optimized completion time test.
 *
 * This algorithm takes into account whether a task is a backup or a
 * primary.  In case it is a backup, the state synchronization time is
 * used as execution time.
 */
class CTT_Enhanced : public CTT_Algorithm
{
public:
  /**
   *  @struct AddExecutionTimes
   *
   *  @brief helper functor for adding task execution times
   */
  struct AddExecutionTimes : public std::binary_function <double,
                                                          Task,
                                                          double>
  {
    /// simple sums up the task execution times
    double operator () (double time, const Task & task);
  };

  /**
   *  @struct AddExecutionTimes
   *
   *  @brief helper functor for calculating a step of the wcrt result
   *  with a heuristic.
   */
  struct WCET_Heuristic_Step : public std::binary_function <double,
                                                            Task,
                                                            double>
  {
    WCET_Heuristic_Step (double R);

    /// adds the execution time for one task times its reoccurence in
    /// one period R_ to the wcrt
    double operator () (double time, const Task & task);

  private:
    double R_;
  };
  
 public:
  virtual ~CTT_Enhanced ();

  virtual double operator () (const TASK_LIST & tasks);

 private:
  // @return 0 if not schedulable, the WCRT otherwise
  double worst_case_response_time_check (const Task & task, 
                                         const TASK_LIST & higher_prio_tasks);
};


#endif /* CTT_ENHANCED_H_ */
