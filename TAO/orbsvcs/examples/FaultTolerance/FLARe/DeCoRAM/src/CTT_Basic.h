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
  virtual ~CTT_Basic ();

  virtual double operator () (const TASK_LIST & tasks);

  // @return 0 if not schedulable, the WCRT otherwise
  double worst_case_response_time_check (const Task & task, 
                                         const TASK_LIST & higher_prio_tasks);
};

#endif /* CTT_BASIC_H_ */
