// -*- C++ -*-

//=============================================================================
/**
 *  @file     FTRMFF_Ranked.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_RANKED_ALGORITHM_H_
#define FTRMFF_RANKED_ALGORITHM_H_

#include "Schedule.h"

class FTRMFF_Ranked : public FTRMFF_Algorithm
{
public:
  virtual ~FTRMFF_Ranked ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
};

class FTRMFF_Ranked_Algorithm : 
  public std::unary_function <TASK_LIST,
                              SCHEDULING_MAP>
{
public:
  FTRMFF_Ranked_Algorithm (const PROCESSOR_LIST & processors,
                             unsigned int consistency_level);

  virtual ~FTRMFF_Ranked_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  SCHEDULE_PROGRESS_LIST get_unschedulable ();

private:
  SCHEDULE schedule_;
  SCHEDULE_PROGRESS_LIST unschedulable_;
  unsigned int consistency_level_;
};

#endif /* FTRMFF_RANKED_ALGORITHM_H_ */
