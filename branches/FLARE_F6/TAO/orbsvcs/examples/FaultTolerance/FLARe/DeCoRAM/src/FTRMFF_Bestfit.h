// -*- C++ -*-

//=============================================================================
/**
 *  @file     FTRMFF_Bestfit.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_BEST_FIT_ALGORITHM_H_
#define FTRMFF_BEST_FIT_ALGORITHM_H_

#include "Forward_Ranking_Scheduler.h"

class FTRMFF_Bestfit : public FTRMFF_Algorithm
{
public:
  virtual ~FTRMFF_Bestfit ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
};

typedef std::map <Processor, ScheduleResult> RESULT_MAP;

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

  const SCHEDULE & schedule () const;

private:
  PROCESSOR_LIST best_processors (void);

private:
  SCHEDULE_PROGRESS_LIST unschedulable_;
  RESULT_MAP last_results_;
  unsigned int consistency_level_;
  Forward_Ranking_Scheduler scheduler_;
};

#endif /* FTRMFF_BEST_FIT_ALGORITHM_H_ */
