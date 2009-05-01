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
  FTRMFF_Bestfit (const std::string & algorithm = "Bestfit");

  virtual ~FTRMFF_Bestfit ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
private:
  std::string algorithm_;
};

class FTRMFF_Bestfit_Algorithm : 
  public std::unary_function <TASK_LIST,
                              SCHEDULING_MAP>
{
public:
  FTRMFF_Bestfit_Algorithm (const PROCESSOR_LIST & processors,
                            unsigned int consistency_level,
                            const std::string & scheduler);

  virtual ~FTRMFF_Bestfit_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  SCHEDULE_PROGRESS_LIST get_unschedulable ();

  SCHEDULE schedule () const;

private:
  SCHEDULE_PROGRESS_LIST unschedulable_;
  unsigned int consistency_level_;
  Forward_Ranking_Scheduler scheduler_;
};

#endif /* FTRMFF_BEST_FIT_ALGORITHM_H_ */
