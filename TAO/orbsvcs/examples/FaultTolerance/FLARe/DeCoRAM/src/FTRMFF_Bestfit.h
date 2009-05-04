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
  public FTRMFF_Algorithm_Impl
{
public:
  FTRMFF_Bestfit_Algorithm (const PROCESSOR_LIST & processors,
                            unsigned int consistency_level,
                            bool bestfit = true);

  virtual ~FTRMFF_Bestfit_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  virtual const SCHEDULE & schedule () const;

private:
  PROCESSOR_LIST best_processors (void);

private:
  RESULT_MAP last_results_;
  Forward_Ranking_Scheduler scheduler_;
  bool bestfit_;
};

#endif /* FTRMFF_BEST_FIT_ALGORITHM_H_ */
