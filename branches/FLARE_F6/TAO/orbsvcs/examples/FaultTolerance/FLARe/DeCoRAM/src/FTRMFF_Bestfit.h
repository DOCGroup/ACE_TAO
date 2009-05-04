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

// This abstract base class allows for different strategies to weight
// the processors for best-fit
class ResultUpdater : public std::unary_function <ScheduleResult,
                                                  void>
{
public:
  ResultUpdater (RESULT_MAP & result_map);

  virtual ~ResultUpdater (void);

  virtual void operator () (const ScheduleResult & result) = 0;

protected:
  RESULT_MAP & result_map_;
};

class FTRMFF_Bestfit_Algorithm : 
  public FTRMFF_Algorithm_Impl
{
public:
  FTRMFF_Bestfit_Algorithm (const PROCESSOR_LIST & processors,
                            unsigned int consistency_level,
                            bool bestfit = true,
                            bool weight_for_failure_case = true);

  virtual ~FTRMFF_Bestfit_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  virtual const SCHEDULE & schedule () const;

private:
  PROCESSOR_LIST best_processors (void);

private:
  RESULT_MAP last_results_;
  Forward_Ranking_Scheduler scheduler_;
  bool bestfit_;
  std::auto_ptr<ResultUpdater> result_updater_;
};

#endif /* FTRMFF_BEST_FIT_ALGORITHM_H_ */
