// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Enhanced.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_ENHANCED_ALGORITHM_H_
#define FTRMFF_ENHANCED_ALGORITHM_H_

#include "Schedule.h"

class FTRMFF_Enhanced : public FTRMFF_Algorithm
{
public:
  virtual ~FTRMFF_Enhanced ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
};

class FTRMFF_Enhanced_Algorithm : 
  public std::unary_function <TASK_LIST,
                              SCHEDULING_MAP>
{
public:
  FTRMFF_Enhanced_Algorithm (const PROCESSOR_LIST & processors,
                             unsigned int consistency_level);

  virtual ~FTRMFF_Enhanced_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  SCHEDULE_PROGRESS_LIST get_unschedulable ();

private:
  SCHEDULE schedule_;
  SCHEDULE_PROGRESS_LIST unschedulable_;
  unsigned int consistency_level_;
  std::auto_ptr<Ranking_Algorithm> ranking_algorithm_;
};

#endif /* FTRMFF_ENHANCED_ALGORITHM_H_ */
