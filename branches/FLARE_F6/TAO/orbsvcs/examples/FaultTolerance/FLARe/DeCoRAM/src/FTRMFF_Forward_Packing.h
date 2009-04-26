// -*- C++ -*-

//=============================================================================
/**
 *  @file     FTRMFF_Forward_Packing.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_FORWARD_PACKING_ALGORITHM_H_
#define FTRMFF_FORWARD_PACKING_ALGORITHM_H_

#include "Schedule.h"
#include "Packing_Scheduler.h"

class FTRMFF_Forward_Packing : public FTRMFF_Algorithm
{
public:
  virtual ~FTRMFF_Forward_Packing ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
};

class FTRMFF_Forward_Packing_Algorithm : 
  public std::unary_function <TASK_LIST,
                              SCHEDULING_MAP>
{
public:
  FTRMFF_Forward_Packing_Algorithm (const PROCESSOR_LIST & processors,
                                    unsigned int consistency_level);

  virtual ~FTRMFF_Forward_Packing_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  SCHEDULE_PROGRESS_LIST get_unschedulable ();

  const SCHEDULE & schedule () const;
private:
  TASK_LIST create_tasks (const Task & task);

private:
  SCHEDULE schedule_;
  SCHEDULE_PROGRESS_LIST unschedulable_;
  unsigned int consistency_level_;
  Packing_Scheduler scheduler_;
};

#endif /* FTRMFF_FORWARD_PACKING_ALGORITHM_H_ */
