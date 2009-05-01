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

#include "Scheduler.h"
#include <memory>

class FTRMFF_Forward_Packing : public FTRMFF_Algorithm
{
public:
  FTRMFF_Forward_Packing (const std::string & algorithm = "Forward_Packing");

  virtual ~FTRMFF_Forward_Packing ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
private:
  std::string algorithm_;
};

class FTRMFF_Forward_Packing_Algorithm : 
  public std::unary_function <TASK_LIST,
                              SCHEDULING_MAP>
{
public:
  FTRMFF_Forward_Packing_Algorithm (const PROCESSOR_LIST & processors,
                                    unsigned int consistency_level,
                                    const std::string & scheduler);

  virtual ~FTRMFF_Forward_Packing_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  SCHEDULE_PROGRESS_LIST get_unschedulable ();

  SCHEDULE schedule () const;

private:
  SCHEDULE_PROGRESS_LIST unschedulable_;
  unsigned int consistency_level_;
  std::auto_ptr<Scheduler> scheduler_;
};

#endif /* FTRMFF_FORWARD_PACKING_ALGORITHM_H_ */
