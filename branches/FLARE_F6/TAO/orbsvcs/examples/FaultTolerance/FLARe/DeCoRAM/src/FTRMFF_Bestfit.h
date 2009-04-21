// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Bestfit.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_BESTFIT_ALGORITHM_H_
#define FTRMFF_BESTFIT_ALGORITHM_H_

#include "Schedule.h"

class FTRMFF_Bestfit : public FTRMFF_Algorithm
{
public:
  virtual ~FTRMFF_Bestfit ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
};

class FTRMFF_Bestfit_Algorithm : 
  public std::unary_function <TASK_LIST,
                              SCHEDULING_MAP>
{
public:
  FTRMFF_Bestfit_Algorithm (const PROCESSOR_LIST & processors,
                           unsigned int consistency_level);

  virtual ~FTRMFF_Bestfit_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  SCHEDULE_PROGRESS_LIST get_unschedulable ();

private:
  const PROCESSOR_LIST & processors_;
  SCHEDULE_PROGRESS_LIST unschedulable_;
  unsigned int consistency_level_;
};

#endif /* FTRMFF_BESTFIT_ALGORITHM_H_ */
