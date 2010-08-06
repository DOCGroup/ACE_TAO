// -*- C++ -*-

//=============================================================================
/**
 *  @file    CTT_Basic.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef CTT_BASIC_H_
#define CTT_BASIC_H_

#include "Algorithms.h"

class CTT_Basic : public CTT_Algorithm
{
 public:
  struct AddExecutionTimes : public std::binary_function <double,
                                                          Task,
                                                          double>
  {
    double operator () (double time, const Task & task);
  };

  struct WCET_Heuristic_Step : public std::binary_function <double,
                                                            Task,
                                                            double>
  {
    WCET_Heuristic_Step (double R);
    double operator () (double time, const Task & task);

  private:
    double R_;
  };

 public:
  virtual ~CTT_Basic ();

  virtual double operator () (const TASK_LIST & tasks);

  // @return 0 if not schedulable, the WCRT otherwise
  double worst_case_response_time_check (const Task & task, 
                                         const TASK_LIST & higher_prio_tasks);
};

#endif /* CTT_BASIC_H_ */
